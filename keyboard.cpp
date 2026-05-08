#include <ntddk.h>
#include <ntimage.h>
#include <ntstrsafe.h>
#include "keys.h"

typedef struct _KBD_DATA {
    USHORT UnitId;
    USHORT MakeCode;
    USHORT Flags;
    USHORT Reserved;
    ULONG Extra;
} KBD_DATA, *PKBD_DATA;

typedef struct _CONN_DATA {
    PDEVICE_OBJECT Device;
    PVOID Service;
} CONN_DATA, *PCONN_DATA;

typedef VOID(*KBD_CALLBACK)(
    PDEVICE_OBJECT Device,
    PKBD_DATA Start,
    PKBD_DATA End,
    PULONG Consumed
);

KBD_CALLBACK g_OldCb = NULL;
PFILE_OBJECT g_KbdFile = NULL;
PDEVICE_OBJECT g_KbdDev = NULL;
PCONN_DATA g_Conn = NULL;

typedef struct _KBD_HOOK {
    PFILE_OBJECT File;
    PDEVICE_OBJECT Device;
    PCONN_DATA Conn;
    KBD_CALLBACK OldCb;
} KBD_HOOK, *PKBD_HOOK;

static KBD_HOOK g_Hooks[16] = {};
static ULONG g_HookCount = 0;
static volatile LONG g_Hits = 0;

static SIZE_T MaskLen(const char* Mask) {
    SIZE_T Len = 0;
    while (Mask && Mask[Len]) ++Len;
    return Len;
}

static VOID FreeKbd() {
    for (ULONG i = 0; i < g_HookCount; ++i) {
        if (g_Hooks[i].File) {
            ObDereferenceObject(g_Hooks[i].File);
            g_Hooks[i].File = NULL;
        }
        g_Hooks[i].Device = NULL;
    }
    g_KbdFile = NULL;
    g_KbdDev = NULL;
}

static BOOLEAN IsHooked(PCONN_DATA Conn) {
    for (ULONG i = 0; i < g_HookCount; ++i) {
        if (g_Hooks[i].Conn == Conn) return TRUE;
    }
    return FALSE;
}

static KBD_CALLBACK GetOldCb(PDEVICE_OBJECT Device) {
    for (ULONG i = 0; i < g_HookCount; ++i) {
        if (g_Hooks[i].Device == Device) return g_Hooks[i].OldCb;
    }
    return g_OldCb;
}

static BOOLEAN CompareData(const UCHAR* Data, const UCHAR* Mask, const char* SzMask) {
    for (; *SzMask; ++SzMask, ++Data, ++Mask) {
        if (*SzMask == 'x' && *Data != *Mask) return FALSE;
    }
    return *SzMask == 0;
}

static ULONG_PTR FindPattern(ULONG_PTR Addr, SIZE_T Len, const UCHAR* Pat, const char* Mask) {
    if (!Addr || !Len) return 0;
    const SIZE_T PatLen = MaskLen(Mask);
    if (!PatLen || Len < PatLen) return 0;

    for (SIZE_T i = 0; i <= Len - PatLen; i++) {
        if (MmIsAddressValid((PVOID)(Addr + i))) {
            if (CompareData((UCHAR*)(Addr + i), Pat, Mask)) return Addr + i;
        }
    }
    return 0;
}

static ULONG_PTR ScanModule(ULONG_PTR Base, const UCHAR* Pat, const char* Mask) {
    PIMAGE_DOS_HEADER Dos = (PIMAGE_DOS_HEADER)Base;
    if (Dos->e_magic != IMAGE_DOS_SIGNATURE) return 0;
    PIMAGE_NT_HEADERS64 Nt = (PIMAGE_NT_HEADERS64)(Base + Dos->e_lfanew);
    PIMAGE_SECTION_HEADER Sec = (PIMAGE_SECTION_HEADER)((ULONG_PTR)Nt + sizeof(IMAGE_NT_HEADERS64));
    for (USHORT i = 0; i < Nt->FileHeader.NumberOfSections; i++) {
        if (Sec[i].Characteristics & IMAGE_SCN_CNT_CODE) {
            ULONG_PTR Match = FindPattern(Base + Sec[i].VirtualAddress, Sec[i].Misc.VirtualSize, Pat, Mask);
            if (Match) return Match;
        }
    }
    return 0;
}

VOID OnKey(PDEVICE_OBJECT Device, PKBD_DATA Start, PKBD_DATA End, PULONG Consumed) {
    /*
        https://users.utcluj.ro/~baruch/sie/labor/PS2/Scan_Codes_Set_1.htm
		You can find the make code of each key in this table and the flag is the key state (UP or DOWN)
    */ 
    for (PKBD_DATA Cur = Start; Cur < End; Cur++) {
        const char* State = (Cur->Flags & 1) ? "UP" : "DOWN";
        const char* KeyName = ScanCodes::GetKey(Cur->MakeCode);

        if (Cur->MakeCode == ScanCodes::F5 && !(Cur->Flags & 1)) {
            DbgPrint("F5 detected!!!!\n");
        }

        DbgPrint("[KBD] Key: %s (0x%X) | %s\n", KeyName, Cur->MakeCode, State);
    }

    KBD_CALLBACK Old = GetOldCb(Device);
    if (Old) Old(Device, Start, End, Consumed);
}

static BOOLEAN TryHook(PFILE_OBJECT File, PDEVICE_OBJECT Dev, PDEVICE_OBJECT Self, PCONN_DATA Conn, PVOID Target) {
    if (!Conn || IsHooked(Conn) || g_HookCount >= 16) return FALSE;
    if (!MmIsAddressValid(Conn) || !MmIsAddressValid(&Conn->Service)) return FALSE;
    if (Conn->Service != Target) return FALSE;
    if (Conn->Device != Dev && Conn->Device != Self) return FALSE;
    KBD_CALLBACK Old = (KBD_CALLBACK)InterlockedExchangePointer((volatile PVOID*)&Conn->Service, (PVOID)OnKey);
    ObReferenceObject(File);

    PKBD_HOOK Entry = &g_Hooks[g_HookCount++];
    Entry->File = File;
    Entry->Device = Dev;
    Entry->Conn = Conn;
    Entry->OldCb = Old;

    if (!g_OldCb) {
        g_OldCb = Old;
        g_KbdFile = File;
        g_KbdDev = Dev;
        g_Conn = Conn;
    }

    DbgPrint("[KBD] Hook OK. Dev=%p Conn=%p Old=%p\n", Dev, Conn, Old);
    return TRUE;
}

static BOOLEAN ScanExt(PFILE_OBJECT File, PDEVICE_OBJECT Dev, PDEVICE_OBJECT Self, PDEVICE_OBJECT Cand, PVOID Target) {
    if (!Cand || !Cand->DeviceExtension) return FALSE;

    BOOLEAN Hooked = FALSE;
    PUCHAR Ext = (PUCHAR)Cand->DeviceExtension;

    for (SIZE_T i = sizeof(PVOID); i <= 0x1000 - sizeof(CONN_DATA); i += sizeof(PVOID)) {
        PVOID* Pot = (PVOID*)(Ext + i);
        if (!MmIsAddressValid(Pot)) continue;

        if (*Pot == Target) {
            PCONN_DATA Conn = (PCONN_DATA)(Ext + i - sizeof(PVOID));
            if (TryHook(File, Dev, Self, Conn, Target)) Hooked = TRUE;
        }
    }
    return Hooked;
}

static BOOLEAN ScanLower(PFILE_OBJECT File, PDEVICE_OBJECT Dev, PDEVICE_OBJECT Self, PDEVICE_OBJECT Lower, PVOID Target) {
    if (!Lower || !Lower->DriverObject) return FALSE;

    BOOLEAN Hooked = FALSE;
    for (PDEVICE_OBJECT Cur = Lower->DriverObject->DeviceObject; Cur; Cur = Cur->NextDevice) {
        if (ScanExt(File, Dev, Self, Cur, Target)) Hooked = TRUE;
    }
    return Hooked;
}

VOID DriverUnload(PDRIVER_OBJECT Driver) {
    UNREFERENCED_PARAMETER(Driver);

    if (g_Conn && g_OldCb) {
        InterlockedExchangePointer((volatile PVOID*)&g_Conn->Service, (PVOID)g_OldCb);
    }

    for (ULONG i = 0; i < g_HookCount; ++i) {
        if (g_Hooks[i].Conn && g_Hooks[i].OldCb && g_Hooks[i].Conn != g_Conn) {
            InterlockedExchangePointer((volatile PVOID*)&g_Hooks[i].Conn->Service, (PVOID)g_Hooks[i].OldCb);
        }
    }

    FreeKbd();
    DbgPrint("[KBD] Unloaded.\n");
}

extern "C" NTSTATUS InitializeKeyboard(PDRIVER_OBJECT Driver) {
    if (Driver) Driver->DriverUnload = DriverUnload;

    NTSTATUS LastStatus = STATUS_NOT_FOUND;
    ULONG_PTR TargetCb = 0;

    for (ULONG i = 0; i < 8; ++i) {
        WCHAR NameBuf[64] = {0};
        UNICODE_STRING Name;
        if (!NT_SUCCESS(RtlStringCchPrintfW(NameBuf, 64, L"\\Device\\KeyboardClass%lu", i))) continue;
        RtlInitUnicodeString(&Name, NameBuf);
        /*
		  This for we cannot exactly know which keyboard device is right one 
          so we will try to hook all of them until we find the correct one with 
          the callback address we are looking for. I optimze it so we cant take blue screen
          In most cases there should be only one or two devices so it wont be a problem :)
        */

        PFILE_OBJECT File = NULL;
        PDEVICE_OBJECT Dev = NULL;
        NTSTATUS Status = IoGetDeviceObjectPointer(&Name, FILE_READ_ATTRIBUTES, &File, &Dev);
        if (!NT_SUCCESS(Status)) {
            LastStatus = Status;
            continue;
        }

        PUCHAR Ext = (PUCHAR)Dev->DeviceExtension;
        if (!Ext) {
            ObDereferenceObject(File);
            LastStatus = STATUS_DEVICE_NOT_CONNECTED;
            continue;
        }

        PDEVICE_OBJECT Self = *(PDEVICE_OBJECT*)(Ext + 0x08);
        PDEVICE_OBJECT Lower = *(PDEVICE_OBJECT*)(Ext + 0x10);
        /*
          kbdclass.sys->DeviceExtension->Self
          kbdclass.sys->DeviceExtension->LowerDevice
        */

        if (!Lower || !Lower->DeviceExtension) {
            ObDereferenceObject(File);
            LastStatus = STATUS_DEVICE_NOT_CONNECTED;
            continue;
        }

        if (!TargetCb) {
            ULONG_PTR Base = (ULONG_PTR)Dev->DriverObject->DriverStart;
            // kbdclass.sys->KeyboardClassServiceCallback
            const UCHAR Pat[] = "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x4C\x24\x00\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC";
            const char* Mask = "xxxx?xxxx?xxxx?xxxx?xxxxxxxxxxxx";

            TargetCb = ScanModule(Base, Pat, Mask);
            if (!TargetCb) TargetCb = Base + 0x22E0;
        }

        BOOLEAN Hooked = ScanExt(File, Dev, Self, Lower, (PVOID)TargetCb);
        if (ScanLower(File, Dev, Self, Lower, (PVOID)TargetCb)) Hooked = TRUE;

        ObDereferenceObject(File);
        if (!Hooked) LastStatus = STATUS_NOT_FOUND;
    }

    if (g_HookCount) return STATUS_SUCCESS;

    FreeKbd();
    return LastStatus;
}

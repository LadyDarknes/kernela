#include "common.h"

extern "C" {
    void MouseClassServiceCallbackTrick(QWORD rdi, QWORD rbp, QWORD target);
    MOUSE_OBJECT gMouseObject = { NULL, 0, 0 };
    QWORD g_target_routine = 0;
    QWORD g_mouhid_base = 0;

    extern POBJECT_TYPE* IoDriverObjectType;

    NTSTATUS ObReferenceObjectByName(
        __in PUNICODE_STRING ObjectName, __in ULONG Attributes,
        __in_opt PACCESS_STATE AccessState, __in_opt ACCESS_MASK DesiredAccess,
        __in POBJECT_TYPE ObjectType, __in KPROCESSOR_MODE AccessMode,
        __inout_opt PVOID ParseContext, __out PVOID* Object);
}

#define MOUHID_DEVICE_OFFSET 0xE0
#define MOUHID_CONNECT_OFFSET 0xE8
#define KERNEL_POINTER_MASK 0xFFFF800000000000ULL

static BOOLEAN IsKernelPointer(ULONG_PTR Ptr) {
    return Ptr >= KERNEL_POINTER_MASK;
}

static BOOL bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask) {
    for (; *szMask; ++szMask, ++pData, ++bMask)
        if ((*szMask == 'x') && *pData != *bMask)
            return FALSE;
    return (*szMask) == 0;
}

static QWORD FindPatternEx(UINT64 dwAddress, QWORD dwLen, BYTE* bMask, const char* szMask) {
    if (dwLen <= 0) return 0;
    for (QWORD i = 0; i < dwLen; i++)
        if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
            return (QWORD)(dwAddress + i);
    return 0;
}

static QWORD FindPattern(QWORD module, unsigned char* bMask, const char* szMask, QWORD len) {
    PIMAGE_DOS_HEADER pidh = (PIMAGE_DOS_HEADER)module;
    if (pidh->e_magic != IMAGE_DOS_SIGNATURE) return 0;

    PIMAGE_NT_HEADERS pinh = (PIMAGE_NT_HEADERS)((BYTE*)pidh + pidh->e_lfanew);
    PIMAGE_SECTION_HEADER pish = (PIMAGE_SECTION_HEADER)((BYTE*)pinh + sizeof(IMAGE_NT_HEADERS64));

    for (USHORT sec = 0; sec < pinh->FileHeader.NumberOfSections; sec++) {
        if ((pish[sec].Characteristics & 0x00000020)) {
            QWORD address = FindPatternEx(pish[sec].VirtualAddress + (ULONG_PTR)(module), pish[sec].Misc.VirtualSize - len, bMask, szMask);
            if (address) return address;
        }
    }
    return 0;
}

extern "C" BOOL mouse_open(void) {
    if (gMouseObject.use_mouse) return TRUE;

    UNICODE_STRING class_string, hid_string;
    RtlInitUnicodeString(&class_string, L"\\Driver\\MouClass");
    RtlInitUnicodeString(&hid_string, L"\\Driver\\MouHID");

    PDRIVER_OBJECT class_driver_object = NULL, hid_driver_object = NULL;

    if (!NT_SUCCESS(ObReferenceObjectByName(&class_string, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID*)&class_driver_object)))
        return FALSE;

    if (!NT_SUCCESS(ObReferenceObjectByName(&hid_string, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID*)&hid_driver_object)))
    {
        ObfDereferenceObject(class_driver_object);
        return FALSE;
    }

    g_mouhid_base = (QWORD)hid_driver_object->DriverStart;
    PDEVICE_OBJECT hid_device_object = hid_driver_object->DeviceObject;

    while (hid_device_object && !gMouseObject.service_callback) {
        PULONG_PTR device_extension = (PULONG_PTR)hid_device_object->DeviceExtension;
        if (device_extension) {
            PDEVICE_OBJECT direct_mouse_device = *(PDEVICE_OBJECT*)((PUCHAR)device_extension + MOUHID_DEVICE_OFFSET);
            ULONG_PTR direct_callback = *(PULONG_PTR)((PUCHAR)device_extension + MOUHID_CONNECT_OFFSET);

            if (direct_mouse_device && direct_callback &&
                IsKernelPointer((ULONG_PTR)direct_mouse_device) &&
                IsKernelPointer(direct_callback)) {
                gMouseObject.mouse_device = direct_mouse_device;
                gMouseObject.service_callback = (QWORD)direct_callback;
                break;
            }
        }
        hid_device_object = hid_device_object->NextDevice;
    }

    if (!gMouseObject.mouse_device) {
        PDEVICE_OBJECT target_device_object = class_driver_object->DeviceObject;
        while (target_device_object) {
            if (!target_device_object->NextDevice) {
                gMouseObject.mouse_device = target_device_object;
                break;
            }
            target_device_object = target_device_object->NextDevice;
        }
    }

    if (g_mouhid_base && !g_target_routine) {
        unsigned char mask[] = "\x48\x8B\x8F\xE0\x00\x00\x00\x4C\x8D\x87\x78\x01\x00\x00\x8A\xD8\x48\x8D\x97\x60\x01\x00\x00\x48\x8B\x87\xE8\x00\x00\x00";
        g_target_routine = FindPattern((QWORD)g_mouhid_base, mask, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 30);

        if (g_target_routine) {
            for (int i = 0; i < 32; i++) {
                if (*(unsigned char*)(g_target_routine - i) == 0xB1 &&
                    *(unsigned char*)(g_target_routine - i + 1) == 0x02) {
                    g_target_routine = g_target_routine - i;
                    break;
                }
            }
        }
    }

    if (gMouseObject.mouse_device && gMouseObject.service_callback && g_target_routine) {
        gMouseObject.use_mouse = 1;
    }

    return gMouseObject.use_mouse;
}

static BYTE Shellcode[] = {
    0x55, 0x53, 0x56, 0x57, 0x41, 0x54, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57, // push registers
    0x48, 0x89, 0xD5,       // mov rbp, rdx
    0x48, 0x83, 0xEC, 0x58, // sub rsp, 58h
    0x48, 0x89, 0xCF,       // mov rdi, rcx
    0x48, 0x89, 0xD6,       // mov rsi, rdx
    0x48, 0x31, 0xDB,       // xor rbx, rbx
    0x4D, 0x31, 0xE4,       // xor r12, r12
    0x4D, 0x31, 0xED,       // xor r13, r13
    0x4D, 0x31, 0xF6,       // xor r14, r14
    0x4D, 0x31, 0xFF,       // xor r15, r15
    0x41, 0xFF, 0xE0        // jmp r8
};
typedef void (*Trick_t)(QWORD rdi, QWORD rbp, QWORD target);
#define Trick ((Trick_t)(void *)Shellcode)

extern "C" void mouse_move(long x, long y, unsigned short button_flags) {
    if (!g_target_routine || !gMouseObject.use_mouse) return;

    char rdi[0x500], rbp[0x100];
    RtlZeroMemory(rdi, 0x500);
    RtlZeroMemory(rbp, 0x100);

    PMOUSE_INPUT_DATA mid = (PMOUSE_INPUT_DATA)&rdi[0x160];
    *(QWORD*)&rdi[0x178] = (QWORD)(mid + 1);

    mid->LastX = x;
    mid->LastY = y;
    mid->ButtonFlags = button_flags;
    mid->UnitId = 1;
    
    *(QWORD*)&rdi[0xE0] = (QWORD)gMouseObject.mouse_device;
    *(QWORD*)&rdi[0xE8] = (QWORD)gMouseObject.service_callback;

    Trick((QWORD)rdi, (QWORD)rbp, (QWORD)g_target_routine);
}

static void Sleep(int ms) {
    LARGE_INTEGER interval;
    interval.QuadPart = -((LONGLONG)ms * 10 * 1000);
    KeDelayExecutionThread(KernelMode, FALSE, &interval);
}
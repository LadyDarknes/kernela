#include "types.h"

MOUSE_OBJECT gMouseObject;
QWORD g_mouhid_base = 0;
QWORD g_target_routine = 0;
#define MOUHID_DEVICE_EXTENSION_SIZE 0x1D8
#define MOUHID_CONNECT_DATA_DEVICE_OFFSET 0xE0
#define MOUHID_CONNECT_DATA_CALLBACK_OFFSET 0xE8
#define KERNEL_POINTER_MIN 0xFFFF800000000000ULL

extern "C" void MouseClassServiceCallbackTrick(QWORD rdi, QWORD rbp, QWORD target);

extern POBJECT_TYPE *IoDriverObjectType;
extern "C" NTSTATUS ObReferenceObjectByName(
    __in PUNICODE_STRING ObjectName, __in ULONG Attributes,
    __in_opt PACCESS_STATE AccessState, __in_opt ACCESS_MASK DesiredAccess,
    __in POBJECT_TYPE ObjectType, __in KPROCESSOR_MODE AccessMode,
    __inout_opt PVOID ParseContext, __out PVOID *Object);
extern "C" void *memset(void *dest, int c, size_t count);

static BOOLEAN IsKernelPointer(ULONG_PTR Ptr) {
  return Ptr >= KERNEL_POINTER_MIN;
}

static BOOL bDataCompare(const BYTE *pData, const BYTE *bMask, const char *szMask) {
  for (; *szMask; ++szMask, ++pData, ++bMask)
    if ((*szMask == 'x') && *pData != *bMask)
      return 0;
  return (*szMask) == 0;
}

static QWORD FindPatternEx(UINT64 dwAddress, QWORD dwLen, BYTE *bMask, char *szMask) {
  if (dwLen <= 0)
    return 0;
  for (QWORD i = 0; i < dwLen; i++)
    if (bDataCompare((BYTE *)(dwAddress + i), bMask, szMask))
      return (QWORD)(dwAddress + i);
  return 0;
}

static QWORD FindPattern(QWORD module, unsigned char *bMask, char *szMask, QWORD len) {
  PIMAGE_DOS_HEADER pidh = (PIMAGE_DOS_HEADER)module;
  PIMAGE_NT_HEADERS pinh = (PIMAGE_NT_HEADERS)((BYTE *)pidh + pidh->e_lfanew);
  PIMAGE_SECTION_HEADER pish = (PIMAGE_SECTION_HEADER)((BYTE *)pinh + sizeof(IMAGE_NT_HEADERS64));
  for (USHORT sec = 0; sec < pinh->FileHeader.NumberOfSections; sec++) {
    if ((pish[sec].Characteristics & 0x00000020)) {
      QWORD address = FindPatternEx(pish[sec].VirtualAddress + (ULONG_PTR)(module), pish[sec].Misc.VirtualSize - len, bMask, szMask);
      if (address)
        return address;
    }
  }
  return 0;
}

BOOL mouse_open(void) {
  if (gMouseObject.use_mouse)
    return 1;
  UNICODE_STRING class_string, hid_string;
  RtlInitUnicodeString(&class_string, L"\\Driver\\MouClass");
  RtlInitUnicodeString(&hid_string, L"\\Driver\\MouHID");
  PDRIVER_OBJECT class_driver_object = NULL, hid_driver_object = NULL;
  if (!NT_SUCCESS(ObReferenceObjectByName(&class_string, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID *)&class_driver_object)))
    return 0;
  if (!NT_SUCCESS(ObReferenceObjectByName(&hid_string, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID *)&hid_driver_object))) {
    ObfDereferenceObject(class_driver_object);
    return 0;
  }
  g_mouhid_base = (QWORD)hid_driver_object->DriverStart;
  PDEVICE_OBJECT hid_device_object = hid_driver_object->DeviceObject;
  while (hid_device_object && !gMouseObject.service_callback) {
    PULONG_PTR device_extension = (PULONG_PTR)hid_device_object->DeviceExtension;
    if (device_extension) {
      PDEVICE_OBJECT direct_mouse_device = *(PDEVICE_OBJECT *)((PUCHAR)device_extension + MOUHID_CONNECT_DATA_DEVICE_OFFSET);
      ULONG_PTR direct_callback = *(PULONG_PTR)((PUCHAR)device_extension +MOUHID_CONNECT_DATA_CALLBACK_OFFSET);
      if (direct_mouse_device && direct_callback && IsKernelPointer((ULONG_PTR)direct_mouse_device) && IsKernelPointer(direct_callback)) {
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
  ObfDereferenceObject(class_driver_object);
  ObfDereferenceObject(hid_driver_object);
  if (gMouseObject.mouse_device && gMouseObject.service_callback)
    gMouseObject.use_mouse = 1;
  return gMouseObject.use_mouse;
}

static BYTE Shellcode[] = {0x55, 0x53, 0x56, 0x57, 0x41, 0x54, 0x41, 0x55,
                           0x41, 0x56, 0x41, 0x57, 0x48, 0x89, 0xD5, 0x48,
                           0x83, 0xEC, 0x58, 0x48, 0x89, 0xCF, 0x4D, 0x31,
                           0xE4, 0x4D, 0x31, 0xED, 0x41, 0xFF, 0xE0};
typedef void (*Trick_t)(QWORD rdi, QWORD rbp, QWORD target);
#define Trick ((Trick_t)(void *)Shellcode)

void Sleep(int ms) {
  LARGE_INTEGER interval;
  interval.QuadPart = -((LONGLONG)ms * 10 * 1000);
  KeDelayExecutionThread(KernelMode, FALSE, &interval);
}

void mouse_move(long x, long y, unsigned short button_flags) {
  if (!g_target_routine)
    return;
  char rdi[0x500], rbp[0x100];
  memset(rdi, 0, 0x500);
  memset(rbp, 0, 0x100);
  MOUSE_INPUT_DATA *mid = (MOUSE_INPUT_DATA *)&rdi[0x160];
  *(QWORD *)&rdi[0x178] = (QWORD)(mid + 1);
  mid->LastX = x;
  mid->LastY = y;
  mid->ButtonFlags = button_flags;
  mid->UnitId = 1;
  *(QWORD *)&rdi[0xE0] = (QWORD)gMouseObject.mouse_device;
  *(QWORD *)&rdi[0xE8] = (QWORD)gMouseObject.service_callback;
  MouseClassServiceCallbackTrick((QWORD)rdi, (QWORD)rbp, (QWORD)g_target_routine);
}

void mouse_move_smooth(long x, long y, int steps) {
  if (steps <= 0)
    steps = 1;
  long step_x = x / steps, step_y = y / steps;
  for (int i = 0; i < steps; i++) {
    mouse_move(step_x, step_y, 0);
    Sleep(1);
  }
  long rem_x = x % steps, rem_y = y % steps;
  if (rem_x || rem_y)
    mouse_move(rem_x, rem_y, 0);
}

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
  UNREFERENCED_PARAMETER(DriverObject);
  UNREFERENCED_PARAMETER(RegistryPath);
  if (!mouse_open())
    return STATUS_DRIVER_ENTRYPOINT_NOT_FOUND;
  g_target_routine = FindPattern(
      (QWORD)g_mouhid_base,
      (unsigned char
           *)"\x48\x8B\x8F\xE0\x00\x00\x00\x4C\x8D\x87\x78\x01\x00\x00\x8A\xD8"
             "\x48\x8D\x97\x60\x01\x00\x00\x48\x8B\x87\xE8\x00\x00\x00",
             "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 30);
  if (g_target_routine) {
    for (int i = 0; i < 32; i++) {
      if (*(unsigned char *)(g_target_routine - i) == 0xB1 &&
          *(unsigned char *)(g_target_routine - i + 1) == 0x02) {
        g_target_routine = g_target_routine - i;
        break;
      }
    }
  }
  return g_target_routine ? STATUS_SUCCESS : STATUS_NOT_SUPPORTED;
}

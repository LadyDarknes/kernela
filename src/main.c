#include "utils/common.h"
#include <ntddk.h>

int _fltused = 0;

extern void CheatLoop(PVOID sc);

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
  UNREFERENCED_PARAMETER(RegistryPath);

  DbgPrint("[+] Kernel Driver Loaded\n");


  NTSTATUS kb_status = InitializeKeyboard(DriverObject);
  if (!NT_SUCCESS(kb_status)) {
    DbgPrint("[-] Keyboard initialization failed: 0x%lX\n", kb_status);
  }

  if (!mouse_open()) {
    DbgPrint("[-] Mouse initialization failed\n");
    return STATUS_NOT_SUPPORTED;
  }
  DbgPrint("[+] Mouse initialized\n");

  HANDLE thread_handle;
  NTSTATUS status = PsCreateSystemThread(&thread_handle, THREAD_ALL_ACCESS, NULL, NULL, NULL, (PKSTART_ROUTINE)CheatLoop, NULL);
  if (NT_SUCCESS(status)) {
    ZwClose(thread_handle);
    DbgPrint("[+] Cheat thread started successfully\n");
  } else {
    DbgPrint("[-] Failed to create cheat thread: 0x%lX\n", status);
  }

  return 0;
}
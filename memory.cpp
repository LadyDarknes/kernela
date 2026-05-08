#include "common.h"

extern "C" NTSTATUS MmCopyVirtualMemory(PEPROCESS SourceProcess, PVOID SourceAddress, PEPROCESS TargetProcess, PVOID TargetAddress, SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize);

PEPROCESS cs2 = NULL;
ULONG_PTR g_client_base = 0;

extern "C" NTSTATUS InitProcessAccess(void) {
  cs2 = NULL;
  for (int i = 4; i < 65536; i += 4) {
    PEPROCESS temp_proc;
    if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)i, &temp_proc))) {
      PCHAR process_name = PsGetProcessImageFileName(temp_proc);
      if (process_name && _stricmp(process_name, "cs2.exe") == 0) {
        cs2 = temp_proc;
        DbgPrint("[+] Found CS2 process: %s\n", process_name);
        break;
      }
      ObDereferenceObject(temp_proc);
    }
  }
  return cs2 ? STATUS_SUCCESS : STATUS_NOT_FOUND;
}

extern "C" BOOL ReadMemory(ULONG_PTR address, PVOID buffer, SIZE_T size) {
  if (!address || !buffer || !size || !cs2)
    return FALSE;
  if (address >= 0x00007FFFFFFFFFFF)
    return FALSE;

  SIZE_T bytesRead = 0;
  NTSTATUS status = MmCopyVirtualMemory(cs2, (PVOID)address, PsGetCurrentProcess(), buffer, size, KernelMode, &bytesRead);
  return NT_SUCCESS(status) && bytesRead == size;
}

typedef struct _LDR_DATA_TABLE_ENTRY_INTERNAL {
  LIST_ENTRY InLoadOrderLinks;
  LIST_ENTRY InMemoryOrderLinks;
  LIST_ENTRY InInitializationOrderLinks;
  PVOID DllBase;
  PVOID EntryPoint;
  ULONG SizeOfImage;
  UNICODE_STRING FullDllName;
  UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY_INTERNAL, *PLDR_DATA_TABLE_ENTRY_INTERNAL;

typedef struct _PEB_LDR_DATA_INTERNAL {
  ULONG Length;
  BOOLEAN Initialized;
  HANDLE SsHandle;
  LIST_ENTRY InLoadOrderModuleList;
} PEB_LDR_DATA_INTERNAL, *PPEB_LDR_DATA_INTERNAL;

typedef struct _PEB_INTERNAL {
  UCHAR Reserved1[2];
  UCHAR BeingDebugged;
  UCHAR Reserved2[1];
  PVOID Reserved3[2];
  PPEB_LDR_DATA_INTERNAL Ldr;
} PEB_INTERNAL, *PPEB_INTERNAL;

extern "C" ULONG_PTR GetModuleBase(PEPROCESS process, const wchar_t *module_name) {
  if (!process) return 0;

  PPEB_INTERNAL peb = (PPEB_INTERNAL)PsGetProcessPeb(process);
  if (!peb) return 0;

  PPEB_LDR_DATA_INTERNAL ldr_ptr = NULL;
  if (!ReadMemory((ULONG_PTR)&peb->Ldr, &ldr_ptr, sizeof(PVOID)) || !ldr_ptr)
    return 0;

  LIST_ENTRY list_head;
  if (!ReadMemory((ULONG_PTR)&ldr_ptr->InLoadOrderModuleList, &list_head, sizeof(LIST_ENTRY)))
    return 0;

  PLIST_ENTRY current_entry = list_head.Flink;
  for (int i = 0; i < 512; i++) {
    if (current_entry ==
        (PLIST_ENTRY)((ULONG_PTR)ldr_ptr + OFFSET_OF(PEB_LDR_DATA_INTERNAL, InLoadOrderModuleList)))
      break;

    LDR_DATA_TABLE_ENTRY_INTERNAL ldr_entry;
    if (!ReadMemory((ULONG_PTR)current_entry, &ldr_entry, sizeof(ldr_entry)))
      break;

    WCHAR buffer[256] = {0};
    if (ReadMemory((ULONG_PTR)ldr_entry.BaseDllName.Buffer, buffer, min(ldr_entry.BaseDllName.Length, sizeof(buffer) - sizeof(WCHAR)))) {
      if (_wcsicmp(buffer, module_name) == 0) {
        return (ULONG_PTR)ldr_entry.DllBase;
      }
    }

    current_entry = ldr_entry.InLoadOrderLinks.Flink;
  }

  return 0;
}

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef unsigned long long uintptr_t;
typedef unsigned long long ULONG_PTR;
typedef unsigned long long UINT64;
typedef signed long long LONGLONG;
typedef unsigned long long ULONGLONG;
#ifndef _In_
#define _In_
#endif
#ifndef _In_opt_
#define _In_opt_
#endif
#ifndef _Out_
#define _Out_
#endif
#ifndef __in
#define __in
#endif
#ifndef __in_opt
#define __in_opt
#endif
#ifndef __out
#define __out
#endif
#ifndef __inout_opt
#define __inout_opt
#endif
typedef void VOID;
typedef void *PVOID;
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef int INT;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef uint64_t QWORD;
typedef unsigned long long ULONGLONG;
#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif
typedef long NTSTATUS;

typedef union _LARGE_INTEGER {
  struct {
    ULONG LowPart;
    LONG HighPart;
  };
  LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef unsigned long long size_t;
#define CONTAINING_RECORD(address, type, field)                                \
  ((type *)((char *)(address) - (size_t)(&((type *)0)->field)))
typedef unsigned long ACCESS_MASK;
typedef signed char KPROCESSOR_MODE;
typedef unsigned char KIRQL;
typedef KIRQL *PKIRQL;
typedef unsigned char BOOLEAN;
typedef int BOOL;
typedef ULONG_PTR *PULONG_PTR;
typedef LONG *PLONG;
typedef USHORT *PUSHORT;
typedef UCHAR *PUCHAR;
#ifndef NULL
#define NULL ((void *)0)
#endif
#define KernelMode 0
#define UserMode 1
#define DISPATCH_LEVEL 2
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#define STATUS_NOT_SUPPORTED ((NTSTATUS)0xC00000BBL)
#define STATUS_DRIVER_ENTRYPOINT_NOT_FOUND ((NTSTATUS)0xC0000263L)
#define OBJ_CASE_INSENSITIVE 0x00000040
#define FALSE 0
#define TRUE 1
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#define UNREFERENCED_PARAMETER(P) (void)(P)
#define NTSYSCALLAPI
#define EXCEPTION_EXECUTE_HANDLER 1

// Forward declarations for kernel types
typedef struct _KPROCESS *PEPROCESS;
typedef struct _KTHREAD *PKTHREAD;
typedef struct _KAPC_STATE {
    PVOID ApcListHead[2];
    struct _KPROCESS *Process;
    UCHAR InProgressFlags;
    UCHAR KernelApcInProgress;
    UCHAR UserApcPending;
} KAPC_STATE, *PKAPC_STATE;
typedef struct _LIST_ENTRY {
  struct _LIST_ENTRY *Flink;
  struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY;

typedef struct _UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  wchar_t *Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _DRIVER_OBJECT {
  SHORT Type;
  SHORT Size;
  PVOID DeviceObject;
  ULONG Flags;
  PVOID DriverStart;
  ULONG DriverSize;
  PVOID DriverSection;
  PVOID DriverExtension;
  UNICODE_STRING DriverName;
  PVOID DriverInit;
  PVOID DriverStartIo;
  PVOID DriverUnload;
  PVOID MajorFunction[28];
} DRIVER_OBJECT, *PDRIVER_OBJECT;

typedef struct _DEVICE_OBJECT {
  SHORT Type;
  USHORT Size;
  LONG ReferenceCount;
  PDRIVER_OBJECT DriverObject;
  struct _DEVICE_OBJECT *NextDevice;
  struct _DEVICE_OBJECT *AttachedDevice;
  PVOID CurrentIrp;
  PVOID Timer;
  ULONG Flags;
  ULONG Characteristics;
  PVOID Vpb;
  PVOID DeviceExtension;
  ULONG DeviceType;
  int8_t StackSize;
} DEVICE_OBJECT, *PDEVICE_OBJECT;

typedef struct _OBJECT_TYPE *POBJECT_TYPE;
typedef struct _ACCESS_STATE *PACCESS_STATE;

// PE structures
typedef struct _IMAGE_DOS_HEADER {
  WORD e_magic;
  WORD e_cblp;
  WORD e_cp;
  WORD e_crlc;
  WORD e_cparhdr;
  WORD e_minalloc;
  WORD e_maxalloc;
  WORD e_ss;
  WORD e_sp;
  WORD e_csum;
  WORD e_ip;
  WORD e_cs;
  WORD e_lfarlc;
  WORD e_ovno;
  WORD e_res[4];
  WORD e_oemid;
  WORD e_oeminfo;
  WORD e_res2[10];
  LONG e_lfanew;
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
  WORD Machine;
  WORD NumberOfSections;
  DWORD TimeDateStamp;
  DWORD PointerToSymbolTable;
  DWORD NumberOfSymbols;
  WORD SizeOfOptionalHeader;
  WORD Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
  DWORD VirtualAddress;
  DWORD Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
  WORD Magic;
  BYTE MajorLinkerVersion;
  BYTE MinorLinkerVersion;
  DWORD SizeOfCode;
  DWORD SizeOfInitializedData;
  DWORD SizeOfUninitializedData;
  DWORD AddressOfEntryPoint;
  DWORD BaseOfCode;
  ULONGLONG ImageBase;
  DWORD SectionAlignment;
  DWORD FileAlignment;
  WORD MajorOperatingSystemVersion;
  WORD MinorOperatingSystemVersion;
  WORD MajorImageVersion;
  WORD MinorImageVersion;
  WORD MajorSubsystemVersion;
  WORD MinorSubsystemVersion;
  DWORD Win32VersionValue;
  DWORD SizeOfImage;
  DWORD SizeOfHeaders;
  DWORD CheckSum;
  WORD Subsystem;
  WORD DllCharacteristics;
  ULONGLONG SizeOfStackReserve;
  ULONGLONG SizeOfStackCommit;
  ULONGLONG SizeOfHeapReserve;
  ULONGLONG SizeOfHeapCommit;
  DWORD LoaderFlags;
  DWORD NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY DataDirectory[16];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_NT_HEADERS64 {
  DWORD Signature;
  IMAGE_FILE_HEADER FileHeader;
  IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;

#define IMAGE_NT_HEADERS IMAGE_NT_HEADERS64
#define PIMAGE_NT_HEADERS PIMAGE_NT_HEADERS64

typedef struct _IMAGE_SECTION_HEADER {
  BYTE Name[8];
  union {
    DWORD PhysicalAddress;
    DWORD VirtualSize;
  } Misc;
  DWORD VirtualAddress;
  DWORD SizeOfRawData;
  DWORD PointerToRawData;
  DWORD PointerToRelocations;
  DWORD PointerToLinenumbers;
  WORD NumberOfRelocations;
  WORD NumberOfLinenumbers;
  DWORD Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

// LDR_DATA_TABLE_ENTRY
typedef struct _LDR_DATA_TABLE_ENTRY {
  LIST_ENTRY InLoadOrderLinks;
  LIST_ENTRY InMemoryOrderLinks;
  LIST_ENTRY InInitializationOrderLinks;
  PVOID DllBase;
  PVOID EntryPoint;
  ULONG SizeOfImage;
  UNICODE_STRING FullDllName;
  UNICODE_STRING BaseDllName;
  ULONG Flags;
  SHORT LoadCount;
  SHORT TlsIndex;
  union {
    LIST_ENTRY HashLinks;
    struct {
      PVOID SectionPointer;
      ULONG CheckSum;
    };
  };
  ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

// MOUSE structures
typedef struct _MOUSE_INPUT_DATA {
  USHORT UnitId;
  USHORT Flags;
  union {
    ULONG Buttons;
    struct {
      USHORT ButtonFlags;
      USHORT ButtonData;
    };
  };
  ULONG RawButtons;
  LONG LastX;
  LONG LastY;
  ULONG ExtraInformation;
} MOUSE_INPUT_DATA, *PMOUSE_INPUT_DATA;

typedef struct _MOUSE_OBJECT {
  PDEVICE_OBJECT mouse_device;
  QWORD service_callback;
  int use_mouse;
} MOUSE_OBJECT, *PMOUSE_OBJECT;

extern POBJECT_TYPE *IoDriverObjectType;

NTSYSCALLAPI
NTSTATUS
ObReferenceObjectByName(__in PUNICODE_STRING ObjectName, __in ULONG Attributes,
                        __in_opt PACCESS_STATE AccessState,
                        __in_opt ACCESS_MASK DesiredAccess,
                        __in POBJECT_TYPE ObjectType,
                        __in KPROCESSOR_MODE AccessMode,
                        __inout_opt PVOID ParseContext, __out PVOID *Object);
void RtlInitUnicodeString(PUNICODE_STRING DestinationString,
                          const wchar_t *SourceString);
int wcscmp(const wchar_t *s1, const wchar_t *s2);
void ObfDereferenceObject(PVOID Object);
void KeDelayExecutionThread(KPROCESSOR_MODE WaitMode, BOOLEAN Alertable,
                            void *Interval);
void KeStackAttachProcess(PVOID Process, PKAPC_STATE ApcState);
void KeUnstackDetachProcess(PKAPC_STATE ApcState);
BOOL mouse_open(void);
void DbgPrint(const char *format, ...);

// Interlocked functions
LONG InterlockedCompareExchange(volatile LONG *Destination, LONG Exchange, LONG Comperand);
LONG InterlockedExchange(volatile LONG *Target, LONG Value);

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
#ifdef __cplusplus
extern "C" {
#endif
unsigned __int64 __readcr8(void);
void __writecr8(unsigned __int64);
#ifdef __cplusplus
}
#endif
#endif

#ifdef __cplusplus
}
#endif

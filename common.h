#pragma once

#include <ntifs.h>
#include <ntimage.h>
#include <ntstrsafe.h>

typedef struct _view_matrix_t {
  float matrix[4][4];
} view_matrix_t;

#ifdef __cplusplus
extern "C" {
#endif
#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifndef QWORD
typedef unsigned long long QWORD;
#endif
#ifndef BOOL
typedef int BOOL;
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef struct _V3 {
  float x, y, z;
} V3;

typedef struct _V2 {
  float x, y;
} V2;

typedef struct _QA {
  float p, y, r;
} QA;

typedef struct _BONE_INFO {
  V3 Pos;
} BONE_INFO;

#define MAX_ENTITIES 64
#define MAX_BONES 128

typedef struct _PLAYER_DATA {
  BOOL valid;
  int health;
  int team;
  int life_state;
  V3 origin;
  V3 head_pos;
  BONE_INFO bones[MAX_BONES];
  float distance;
  ULONG_PTR pawn_address;
  ULONG_PTR controller_address;
  wchar_t name[32];
} PLAYER_DATA;

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
  ULONG_PTR service_callback;
  int use_mouse;
} MOUSE_OBJECT, *PMOUSE_OBJECT;

extern PEPROCESS cs2;
extern ULONG_PTR g_client_base;

NTSTATUS InitProcessAccess(void);
ULONG_PTR GetModuleBase(PEPROCESS process, const wchar_t *module_name);
BOOL ReadMemory(ULONG_PTR address, PVOID buffer, SIZE_T size);

BOOL mouse_open(void);
void mouse_move(long x, long y, unsigned short button_flags);

NTSTATUS InitializeKeyboard(PDRIVER_OBJECT DriverObject);

#define OFFSET_OF(type, member) ((ULONG_PTR)(&((type *)0)->member))

// System functions
NTSYSCALLAPI NTSTATUS ObReferenceObjectByName(
    PUNICODE_STRING ObjectName, ULONG Attributes, PACCESS_STATE AccessState,
    ACCESS_MASK DesiredAccess, POBJECT_TYPE ObjectType,
    KPROCESSOR_MODE AccessMode, PVOID ParseContext, PVOID *Object);

extern POBJECT_TYPE *IoDriverObjectType;
extern PCHAR PsGetProcessImageFileName(PEPROCESS Process);
extern PVOID PsGetProcessPeb(PEPROCESS Process);

#ifdef __cplusplus
}
#include <../km/crt/new.h>

template <typename T> class SimpleVector {
private:
  T *data;
  size_t _size;
  size_t _capacity;

  void reserve(size_t new_cap) {
    if (new_cap <= _capacity)
      return;
    T *new_data =
        (T *)ExAllocatePool2(POOL_FLAG_PAGED, new_cap * sizeof(T), 'tceV');
    if (!new_data)
      return;
    if (data) {
      RtlCopyMemory(new_data, data, _size * sizeof(T));
      ExFreePoolWithTag(data, 'tceV');
    }
    data = new_data;
    _capacity = new_cap;
  }

public:
  SimpleVector() : data(nullptr), _size(0), _capacity(0) {}
  ~SimpleVector() {
    if (data)
      ExFreePoolWithTag(data, 'tceV');
  }

  void push_back(const T &value) {
    if (_size >= _capacity)
      reserve(_capacity == 0 ? 8 : _capacity * 2);
    if (data)
      data[_size++] = value;
  }

  void clear() { _size = 0; }
  size_t size() const { return _size; }
  T &operator[](size_t index) { return data[index]; }
  const T &operator[](size_t index) const { return data[index]; }

  T *begin() { return data; }
  T *end() { return data + _size; }
  const T *begin() const { return data; }
  const T *end() const { return data + _size; }
};

template <typename T> T R(ULONG_PTR address) {
  T buffer{};
  ReadMemory(address, &buffer, sizeof(T));
  return buffer;
}
#endif

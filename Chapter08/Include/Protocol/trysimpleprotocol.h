#include <Uefi.h>

// {3bb954fc-ba72-4bbd-9626-46c931921cd6}
#define EFI_SIMPLE_PROTOCOL_GUID { 0x3bb954fc, 0xba72, 0x4bbd, {0x96, 0x26, 0x46, 0xc9, 0x31, 0x92, 0x1c, 0xd6} }

#define SIMPLE_PROTOCOL EFI_SIMPLE_PROTOCOL_GUID

extern EFI_GUID gEfiSimpleProtocolGUID;
//extern EFI_GUID gEfiSimpleProtocolGUID;

typedef struct _EFI_SIMPLE_PROTOCOL EFI_SIMPLE_PROTOCOL;
typedef EFI_SIMPLE_PROTOCOL EFI_SIMPLE;


typedef EFI_STATUS (EFIAPI *EFI_SIMPLE_FUNC)(IN EFI_SIMPLE_PROTOCOL *This, IN CHAR16 *str, OUT INT64* totalLen);

struct _EFI_SIMPLE_PROTOCOL
{
    UINT64 Revision;
    EFI_SIMPLE_FUNC SimpleFunc;
};


#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "Protocol/trysimpleprotocol.h"

#define SIMPLE_PROTOCOL_PRIVATE_DATA_SIGNATURE SIGNATURE_32('S', 'P', 'P', 'D')

typedef struct
{
    UINTN Signature;
    EFI_SIMPLE_PROTOCOL simpleProtocol;
    CHAR16 *extraStr;
} SIMPLE_PROTOCOL_PRIVATE_DATA;

#define SIMPLE_PROTOCOL_PRIVATE_DATA_FROM_THIS(a) CR(a, SIMPLE_PROTOCOL_PRIVATE_DATA, simpleProtocol, SIMPLE_PROTOCOL_PRIVATE_DATA_SIGNATURE)

static SIMPLE_PROTOCOL_PRIVATE_DATA gSimpleProtocolPrivate;

EFI_STATUS EFIAPI SimpleFunc(IN EFI_SIMPLE_PROTOCOL *This, IN CHAR16 *str, OUT INT64 *totalLen);

EFI_SIMPLE_PROTOCOL *InitSimpleProtocolPrivate()
{
    SIMPLE_PROTOCOL_PRIVATE_DATA *private = &gSimpleProtocolPrivate;
    private->Signature = SIMPLE_PROTOCOL_PRIVATE_DATA_SIGNATURE;
    private->extraStr = L"Hello World";
    private->simpleProtocol.Revision = 1;
    private->simpleProtocol.SimpleFunc = SimpleFunc;
    return &private->simpleProtocol;
}

EFI_STATUS EFIAPI InitSimpleProtocol(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_SIMPLE_PROTOCOL *simpleProtocol = InitSimpleProtocolPrivate();
    gBS->InstallProtocolInterface(&ImageHandle, &gEfiSimpleProtocolGUID, EFI_NATIVE_INTERFACE, simpleProtocol);
    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI SimpleFunc(IN EFI_SIMPLE_PROTOCOL *This, IN CHAR16 *str, OUT INT64 *totalLen)
{
    SIMPLE_PROTOCOL_PRIVATE_DATA *private = SIMPLE_PROTOCOL_PRIVATE_DATA_FROM_THIS(This);
    UINTN originLen = StrLen(str);
    UINTN extraLen = StrLen(private->extraStr);

    *totalLen = originLen + extraLen;
    return EFI_SUCCESS;
}
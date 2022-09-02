#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>

#define INIT_NAME_BUFFER_SIZE 128
#define INIT_DATA_BUFFER_SIZE 1024

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    UINTN nameBufferSize = INIT_NAME_BUFFER_SIZE;
    UINTN dataBufferSize = INIT_DATA_BUFFER_SIZE;
    CHAR16 *foundVarName = AllocateZeroPool(nameBufferSize);
    if (foundVarName == NULL)
    {
        return EFI_OUT_OF_RESOURCES;
    }
    UINT8 *dataBuffer = AllocateZeroPool(dataBufferSize);
    if (dataBuffer == NULL)
    {
        FreePool(foundVarName);
        return EFI_OUT_OF_RESOURCES;
    }

    EFI_GUID foundVarGuid;
    while (TRUE)
    {
        UINTN nameSize = nameBufferSize;
        EFI_STATUS status = gRT->GetNextVariableName(&nameSize, foundVarName, &foundVarGuid);
        if (status == EFI_BUFFER_TOO_SMALL)
        {
            CHAR16 *oldName = foundVarName;
            UINTN oldNameBufferSize = nameBufferSize;
            nameBufferSize = nameSize > nameBufferSize * 2 ? nameSize : nameBufferSize * 2;
            foundVarName = AllocateZeroPool(nameBufferSize);
            if (foundVarName == NULL)
            {
                FreePool(oldName);
                break;
            }
            CopyMem(foundVarName, oldName, oldNameBufferSize);
            FreePool(oldName);
            nameSize = nameBufferSize;
            status = gRT->GetNextVariableName(&nameSize, foundVarName, &foundVarGuid);
        }
        if (status == EFI_NOT_FOUND)
        {
            break;
        }
        Print(L"%s\n", foundVarName);
    }

    return EFI_SUCCESS;
}

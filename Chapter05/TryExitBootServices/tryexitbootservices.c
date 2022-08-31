#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Exit boot services\n");
    CHAR16 *vendor = SystemTable->FirmwareVendor;
    Print(L"%s\n", vendor);
    UINTN memoryMapSize = 0;
    EFI_MEMORY_DESCRIPTOR *memoryMap = NULL;
    UINTN mapKey = 0;
    UINTN descriptorSize = 0;
    UINT32 descriptorVersion = 0;
    gBS->GetMemoryMap(&memoryMapSize, memoryMap, &mapKey, &descriptorSize, &descriptorVersion);
    gBS->ExitBootServices(ImageHandle, mapKey);
    ASSERT(SystemTable->BootServices == NULL);

    return (EFI_STATUS)-1;
}

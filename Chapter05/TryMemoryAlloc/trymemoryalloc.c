#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS TestAllocateAnyPages()
{
    EFI_PHYSICAL_ADDRESS pages;
    EFI_STATUS status = gBS->AllocatePages(AllocateAnyPages, EfiBootServicesData, 3, &pages);
    Print(L"Try using AllocatePages to allocate any pages: %r %x\n", status, pages);
    if (status == EFI_SUCCESS)
    {
        CHAR16 *greeting = L"Hello world\n";
        CHAR16 *str = (CHAR16 *)pages;
        UINTN strLength = sizeof(CHAR16) * 13;
        Print(L"sizeof greeting: %d\n", strLength);
        gBS->CopyMem(str, greeting, strLength);
        Print(str);
        gBS->FreePages(pages, 3);
    }
    else
    {
        Print(L"Allocate failed.\n");
    }
    return status;
}

EFI_STATUS TestAllocateSpecificAddress()
{
    EFI_PHYSICAL_ADDRESS pages = 1024 * 1024 * 10;
    EFI_STATUS status = gBS->AllocatePages(AllocateAddress, EfiBootServicesData, 3, &pages);
    Print(L"Allocate specific address: %r %x\n", status, pages);
    if (status == EFI_SUCCESS)
    {
        CHAR16 *greeting = L"Hello world\n";
        CHAR16 *str = (CHAR16 *)pages;
        UINTN strLength = sizeof(CHAR16) * 13;
        Print(L"sizeof greeting: %d\n", strLength);
        gBS->CopyMem(str, greeting, strLength);
        Print(str);
        gBS->FreePages(pages, 3);
    }
    else
    {
        Print(L"Allocate failed.\n");
    }
    return status;
}

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
    TestAllocateAnyPages();
    TestAllocateSpecificAddress();
    return EFI_SUCCESS;
}

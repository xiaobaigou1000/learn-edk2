#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Protocol/SimpleFileSystem.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fileSystem;
    EFI_STATUS status = gBS->LocateProtocol(&gEfiSimpleFileSystemProtocolGuid, NULL, (void **)&fileSystem);
    if (EFI_ERROR(status))
    {
        DebugPrint(DEBUG_ERROR, "locate file system protocol failed.\n");
        return EFI_UNSUPPORTED;
    }

    EFI_FILE_PROTOCOL *root;
    status = fileSystem->OpenVolume(fileSystem, &root);
    EFI_FILE_PROTOCOL *fileHandle;
    root->Open(root, &fileHandle, (CHAR16 *)L"hello.txt", EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    const CHAR16 *text = L"hello world\nthis file is written in efi shell";
    UINTN textSize = StrLen(text) * sizeof(CHAR16);
    fileHandle->Write(fileHandle, &textSize, (VOID *)text);
    fileHandle->Close(fileHandle);

    root->Open(root, &fileHandle, (CHAR16 *)L"hello.txt", EFI_FILE_MODE_READ, 0);
    void *buffer = NULL;
    gBS->AllocatePool(EfiBootServicesData, textSize, &buffer);
    fileHandle->Read(fileHandle, &textSize, buffer);
    Print((CHAR16 *)buffer);
    Print(L"\n");
    fileHandle->Close(fileHandle);
    gBS->FreePool(buffer);

    return EFI_SUCCESS;
}
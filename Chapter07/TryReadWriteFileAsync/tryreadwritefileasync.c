#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Protocol/SimpleFileSystem.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

EFI_STATUS EFIAPI OpenAsyncFile(EFI_FILE_PROTOCOL *root, CHAR16 *fileName, UINT64 openMode, OUT EFI_FILE_PROTOCOL **file)
{
    EFI_FILE_IO_TOKEN openToken;
    EFI_FILE_PROTOCOL *asyncFile;
    EFI_STATUS status = gBS->CreateEvent(0, TPL_NOTIFY, NULL, NULL, &openToken.Event);
    status = root->OpenEx(root, &asyncFile, L"async_file.txt", EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0, &openToken);
    if (EFI_ERROR(status))
    {
        Print(L"Damn!\n");
        gBS->CloseEvent(openToken.Event);
        return status;
    }

    UINTN index = 0;
    gBS->WaitForEvent(1, &openToken.Event, &index);
    Print(L"Aync Open Status: %d\n", openToken.Status);
    gBS->CloseEvent(openToken.Event);
    *file = asyncFile;
    return status;
}

EFI_STATUS EFIAPI WriteFileWithWaitForEvent(EFI_FILE_PROTOCOL *root)
{
    return EFI_UNSUPPORTED;
}

EFI_STATUS EFIAPI ReadFileWithNotification(EFI_FILE_PROTOCOL *root)
{
    return EFI_UNSUPPORTED;
}

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

    WriteFileWithWaitForEvent(root);

    return EFI_SUCCESS;
}
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
    EFI_STATUS status = gBS->CreateEvent(0, TPL_NOTIFY, NULL, NULL, &openToken.Event);
    status = root->OpenEx(root, file, fileName, openMode, 0, &openToken);
    if (EFI_ERROR(status))
    {
        Print(L"Dispatch Open file failed!\n");
        gBS->CloseEvent(openToken.Event);
        return status;
    }

    UINTN index = 0;
    gBS->WaitForEvent(1, &openToken.Event, &index);
    Print(L"Aync Open Status: %d\n", openToken.Status);
    gBS->CloseEvent(openToken.Event);
    return status;
}

EFI_STATUS EFIAPI WriteFileWithWaitForEvent(EFI_FILE_PROTOCOL *root)
{
    EFI_FILE_PROTOCOL *file;
    OpenAsyncFile(root, L"async_file.txt", EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, &file);
    EFI_FILE_IO_TOKEN writeToken;
    gBS->CreateEvent(0, TPL_NOTIFY, NULL, NULL, &writeToken.Event);
    CHAR16 *textToWrite = L"Hello Async File IO\n";
    writeToken.Buffer = (void *)textToWrite;
    writeToken.BufferSize = StrLen(textToWrite) * 2;
    EFI_STATUS status = file->WriteEx(file, &writeToken);
    if (EFI_ERROR(status))
    {
        gBS->CloseEvent(writeToken.Event);
        Print(L"Async write dispatch failed. Status Code: %r\n", status);
        return status;
    }
    UINTN index = 0;
    gBS->WaitForEvent(1, &writeToken.Event, &index);
    Print(L"Async write status: %d\n", writeToken.Status);
    gBS->CloseEvent(writeToken.Event);
    file->Close(file);
    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI ReadFileNofiticationFunc(EFI_EVENT event, void *context)
{
    EFI_FILE_IO_TOKEN *readToken = (EFI_FILE_IO_TOKEN *)context;
    Print(L"Async Read Status: %d\nBuffer size: %d\n", readToken->Status, readToken->BufferSize);
    Print(L"Text in file:\n...File Start...\n");
    Print(readToken->Buffer);
    Print(L"\n ...File end...\n");
    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI ReadFileWithNotification(EFI_FILE_PROTOCOL *root)
{
    EFI_FILE_PROTOCOL *file;
    OpenAsyncFile(root, L"async_file.txt", EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, &file);

    EFI_FILE_IO_TOKEN readToken;
    gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_NOTIFY, (EFI_EVENT_NOTIFY)ReadFileNofiticationFunc, (void *)&readToken, &readToken.Event);
    readToken.BufferSize = 1024;
    gBS->AllocatePool(EfiBootServicesData, readToken.BufferSize, &readToken.Buffer);
    file->ReadEx(file, &readToken);
    UINTN index = 0;
    gBS->WaitForEvent(1, &readToken.Event, &index);
    gBS->FreePool(readToken.Buffer);
    gBS->CloseEvent(readToken.Event);

    return EFI_SUCCESS;
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
    ReadFileWithNotification(root);

    return EFI_SUCCESS;
}
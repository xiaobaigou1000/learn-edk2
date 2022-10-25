#include <Uefi.h>
#include <stdio.h>
#include <Protocol/SimpleFileSystem.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/FileInfo.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#define STBI_NO_STDIO
#include "stb_image.h"

int main(int argc, char **argv)
{
    printf("Hello world edk2-libc\n");

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *simpleFileSystemProtocol = NULL;
    gBS->LocateProtocol(&gEfiSimpleFileSystemProtocolGuid, NULL, (void **)&simpleFileSystemProtocol);
    EFI_FILE_PROTOCOL *root = NULL;
    simpleFileSystemProtocol->OpenVolume(simpleFileSystemProtocol, &root);

    EFI_FILE_PROTOCOL *file = NULL;

    root->Open(root, &file, L"hello.png", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

    EFI_FILE_INFO *fileInfo = NULL;
    UINTN efiFileInfoSize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 512;
    gBS->AllocatePool(EfiBootServicesData, efiFileInfoSize, (void **)fileInfo);
    file->GetInfo(file, &gEfiFileInfoGuid, efiFileInfoSize, &fileInfo);
    

    int width = 0;
    int height = 0;
    int original_channels = 0;
    int desired_channels = 4;

    return EFI_SUCCESS;
}

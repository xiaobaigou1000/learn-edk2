#include <Uefi.h>
#include <stdio.h>
#include <Protocol/SimpleFileSystem.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Guid/FileInfo.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#define STBI_NO_STDIO
#include "stb_image.h"

int main(int argc, char **argv)
{
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *simpleFileSystemProtocol = NULL;
    gBS->LocateProtocol(&gEfiSimpleFileSystemProtocolGuid, NULL, (void **)&simpleFileSystemProtocol);
    EFI_FILE_PROTOCOL *root = NULL;
    simpleFileSystemProtocol->OpenVolume(simpleFileSystemProtocol, &root);

    EFI_FILE_PROTOCOL *file = NULL;
    root->Open(root, &file, L"hello.png", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    EFI_FILE_INFO *fileInfo = NULL;
    UINTN efiFileInfoSize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 512;
    gBS->AllocatePool(EfiBootServicesData, efiFileInfoSize, (void **)&fileInfo);
    file->GetInfo(file, &gEfiFileInfoGuid, &efiFileInfoSize, fileInfo);
    Print(L"File Name: %s, File Size: %d\n", fileInfo->FileName, fileInfo->FileSize);

    void *fileBuffer = NULL;
    gBS->AllocatePool(EfiBootServicesData, fileInfo->FileSize, &fileBuffer);
    file->Read(file, &fileInfo->FileSize, fileBuffer);

    int width = 0;
    int height = 0;
    int original_channels = 0;
    int desired_channels = 4;
    stbi_uc *image = stbi_load_from_memory((stbi_uc *)fileBuffer, fileInfo->FileSize, &width, &height, &original_channels, desired_channels);
    printf("Image Read Complete Status: %d\n", image != NULL);
    gBS->FreePool(fileInfo);

    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *pixel = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)image;
    for (UINTN i = 0; i < width * height; i++)
    {
        EFI_GRAPHICS_OUTPUT_BLT_PIXEL *current = pixel + i;
        UINT8 temp = current->Blue;
        current->Blue = current->Red;
        current->Red = temp;
    }

    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
    gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void **)&gop);
    gop->SetMode(gop, 0);
    gop->Blt(gop, pixel, EfiBltBufferToVideo, 0, 0, 0, 0, width, height, 0);

    UINTN index = 0;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &index);

    stbi_image_free(image);
    gBS->FreePool(fileBuffer);

    return EFI_SUCCESS;
}

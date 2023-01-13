#include "loadImage.h"

bool JPEG::Load(const char* filename)
{
    FILE* File;
    errno_t error;
    error = fopen_s(&File, filename, "rb");
    if (error != NULL) {
        printf("�t�@�C��������܂���");
        return false;
    }
    jpeg_stdio_src(&cinfo, File);
    jpeg_read_header(&cinfo, TRUE);
    sizeX = cinfo.image_width;
    sizeY = cinfo.image_height;
    jpeg_start_decompress(&cinfo);
    // �C���[�W��ێ����郁�����̈�̊m�ۂƏ�����
    Data = (char*)malloc(sizeY * (sizeX * 3));
    img = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * cinfo.image_height);
    for (int i = 0; (unsigned)i < cinfo.image_height; i++) {
        img[i] = (JSAMPROW)calloc(sizeof(JSAMPLE), 3 * cinfo.image_width);
    }
    // �S�C���[�W�f�[�^���擾 
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, img + cinfo.output_scanline, cinfo.output_height - cinfo.output_scanline);
    }
    for (int i = 0; (unsigned)i < sizeY; i++) {
        for (int j = 0; (unsigned)j < sizeX * 3; j++) {
            Data[i * sizeX * 3 + j] = (char)img[sizeY - i - 1][j];
        }
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(File);
    // �C���[�W�f�[�^��ێ����郁�����̈���J��
    for (int i = 0; (unsigned)i < sizeY; i++)
        free(img[i]);
    free(img);
    return true;
}

void JPEG::TexSet()
{

}

JPEG::JPEG(const char* FileName)
{
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    Load(FileName);
   // TexSet();
}

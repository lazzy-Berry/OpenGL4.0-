#include "loadImage.h"

bool JPEG::Load(const char* filename)
{
    FILE* File;
    errno_t error;
    error = fopen_s(&File, filename, "rb");
    if (error != NULL) {
        printf("ファイルがありません");
        return false;
    }
    jpeg_stdio_src(&cinfo, File);
    jpeg_read_header(&cinfo, TRUE);
    sizeX = cinfo.image_width;
    sizeY = cinfo.image_height;
    jpeg_start_decompress(&cinfo);
    // イメージを保持するメモリ領域の確保と初期化
    Data = (char*)malloc(sizeY * (sizeX * 3));
    img = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * cinfo.image_height);
    for (int i = 0; (unsigned)i < cinfo.image_height; i++) {
        img[i] = (JSAMPROW)calloc(sizeof(JSAMPLE), 3 * cinfo.image_width);
    }
    // 全イメージデータを取得 
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
    // イメージデータを保持するメモリ領域を開放
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

Texture2D::Texture2D(const char* filename, const FREE_IMAGE_FORMAT format, int flags)
{
    m_image_file = FreeImage_Load(format, filename, flags);
    if (!m_image_file)
    {
        printf("Cannot load file:\n");
        system("PAUSE");
        exit(1);
    }
    m_width = FreeImage_GetWidth(m_image_file);
    m_height = FreeImage_GetHeight(m_image_file);
    m_pitch = FreeImage_GetPitch(m_image_file);

    //アルファチャンネルの有無によって領域の確保を変える
    int bpp = FreeImage_GetBPP(m_image_file);
    switch (bpp)
    {
    case 24:
        allocateAsRGB();
        break;
    case 32:
        allocateAsRGBA();
        break;
    }
    FreeImage_Unload(m_image_file);
}

Texture2D::~Texture2D()
{
    delete[] m_bits;
}

unsigned char* Texture2D::getBits()
{
    return m_bits;
}

unsigned Texture2D::getWidth() const
{
    return m_width;
}

unsigned Texture2D::getHeight() const
{
    return m_height;
}

void Texture2D::allocateAsRGBA()
{
    //FIBITMAP形式からunsigned char*形式に変換する.
    //RGBA情報を保存
    m_bits = new unsigned char[m_width * m_height * 4];

    unsigned int pitch = FreeImage_GetPitch(m_image_file);
    unsigned int id = 0;
    BYTE* bits = (BYTE*)FreeImage_GetBits(m_image_file);
    for (unsigned int y = 0; y < m_height; ++y)
    {
        BYTE* pixel = (BYTE*)bits;
        for (unsigned int x = 0; x < m_width; ++x)
        {
            m_bits[id] = pixel[FI_RGBA_RED];
            m_bits[id + 1] = pixel[FI_RGBA_GREEN];
            m_bits[id + 2] = pixel[FI_RGBA_BLUE];
            m_bits[id + 3] = pixel[FI_RGBA_ALPHA];

            id += 4;
            pixel += 4;
        }
        bits += pitch;
    }
}

void Texture2D::allocateAsRGB()
{
    //FIBITMAP形式からunsigned char*形式に変換する.
//RGB情報を保存
    m_bits = new unsigned char[m_width * m_height * 4];

    unsigned int pitch = FreeImage_GetPitch(m_image_file);
    unsigned int id = 0;
    BYTE* bits = (BYTE*)FreeImage_GetBits(m_image_file);
    for (unsigned int y = 0; y < m_height; ++y)
    {
        BYTE* pixel = (BYTE*)bits;
        for (unsigned int x = 0; x < m_width; ++x)
        {
            m_bits[id] = pixel[FI_RGBA_RED];
            m_bits[id + 1] = pixel[FI_RGBA_GREEN];
            m_bits[id + 2] = pixel[FI_RGBA_BLUE];
            m_bits[id + 3] = 255;

            id += 4;
            pixel += 3;
        }
        bits += pitch;
    }
}

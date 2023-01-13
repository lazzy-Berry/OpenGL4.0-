#pragma once
#ifndef loadImage_H
#define loadImage_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>
#include <glm/glm.hpp>

#include "jpeglib.h"
#include <stdio.h>

#define WIDTH 1024
#define HEIGHT 1024

//libjpegを用いて画像を読み込む用のクラス
//参考にさせていただきました
//https://w.atwiki.jp/opengl/pages/68.html
class JPEG
{
public:
    JSAMPARRAY img;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    unsigned long sizeX;  //横
    unsigned long sizeY; //縦
    char* Data;  //画像データ格納
    bool Load(const char* filename);
    GLuint texture;
    void TexSet();
    JPEG(const char* FileName);
};

//FreeImageを用いて画像を読み込む用のクラス
//参考にさせていただきました
//https://ameblo.jp/nossey/entry-11900199164.html
//FreeImage.dllを出力先フォルダにコピーしておくこと
class Texture2D
{
private:
    unsigned m_width, m_height, m_pitch;
    FIBITMAP* m_image_file;
    unsigned char* m_bits;

public:
    Texture2D(const char* filename, const FREE_IMAGE_FORMAT format, int flags = 0);
    ~Texture2D();

    unsigned char* getBits();
    unsigned getWidth() const;
    unsigned getHeight() const;
    GLuint texture;
private:
    void allocateAsRGBA();
    void allocateAsRGB();
};

#endif
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

//libjpeg��p���ĉ摜��ǂݍ��ޗp�̃N���X
//�Q�l�ɂ����Ă��������܂���
//https://w.atwiki.jp/opengl/pages/68.html
class JPEG
{
public:
    JSAMPARRAY img;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    unsigned long sizeX;  //��
    unsigned long sizeY; //�c
    char* Data;  //�摜�f�[�^�i�[
    bool Load(const char* filename);
    GLuint texture;
    void TexSet();
    JPEG(const char* FileName);
};

//FreeImage��p���ĉ摜��ǂݍ��ޗp�̃N���X
//�Q�l�ɂ����Ă��������܂���
//https://ameblo.jp/nossey/entry-11900199164.html
//FreeImage.dll���o�͐�t�H���_�ɃR�s�[���Ă�������
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
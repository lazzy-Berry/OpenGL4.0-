#pragma once
#ifndef loadImage_H
#define loadImage_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "jpeglib.h"
#include <stdio.h>

#define WIDTH 1024
#define HEIGHT 1024

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


#endif
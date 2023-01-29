#ifndef SeamlessNoiseTex_H
#define SeamlessNoiseTex_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace SeamlessNoiseTex
{
    int generate2DTex(bool seamless = false, float baseFreq = 4.0f, int w = 128, int h = 128);
};

#endif // SeamlessNoiseTex_H
#include "NoiseTex.h"
#include <cstdio>
#include <noise/noise.h>
#include <glm/glm.hpp>

int NoiseTex::generate2DTex(bool seamless, float baseFreq, int w, int h)
{
    int width = w;
    int height = h;

    noise::module::Perlin perlinNoise;

    //オクターブ1の基本周波数
    perlinNoise.SetFrequency(4.0);

    GLubyte* data = new GLubyte[width * height * 4];

    double xRange = 1.0;
    double yRange = 1.0;
    double xFactor = xRange / width;
    double yFactor = yRange / height;

    for (int oct = 0; oct < 4; oct++)
    {
        perlinNoise.SetOctaveCount(oct + 1);
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                double x = xFactor * i;
                double y = yFactor * j;
                double z = 0.0;

                float val = 0.0f;
                val = perlinNoise.GetValue(x, y, z);

                val = (val + 1.0f) * 0.5f;
                val = val > 1.0f ? 1.0f : val;
                val = val < 0.0f ? 0.0f : val;

                data[((j * width + i) * 4) + oct] = (GLubyte)(val * 255.0f);
            }
        }
    }

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete[] data;
    return texId;
}

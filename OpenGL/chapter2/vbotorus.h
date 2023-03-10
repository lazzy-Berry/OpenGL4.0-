#pragma once
#ifndef VBOTORUS_H
#define VBOTORUS_H

#include "drawable.h"

class VBOTorus : public Drawable
{
private:
    unsigned int vaoHandle;
    int faces, rings, sides;

    void generateVerts(float*, float*, float*, unsigned int*,
        float, float);

public:
    VBOTorus(float, float, int, int);

    void render() const;
    unsigned int getVertexArrayHandle() const;
};

#endif // VBOTORUS_H
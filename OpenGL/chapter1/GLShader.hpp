#ifndef GLSHADER_H
#define GLSHADER_H

#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

GLuint LoadShader(const char* vertex_path, const char* fragment_path);

void SendRotationUniform(GLuint* programHandle, float angle, GLuint* vaoHandle);

#endif
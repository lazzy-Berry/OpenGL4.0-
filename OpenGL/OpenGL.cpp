
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

int main()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Glewの初期化エラー: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Hello World!\n";
};



#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

int main()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        //Windowインスタンスが生成されていないため、Missing　version　Errorがでる
        std::cout << "Glewの初期化エラー: " << glewGetErrorString(err) << std::endl;
    }
    
    //拡張機能が利用可能かどうかを実行時にチェック
    if (!GLEW_ARB_vertex_program)
    {
        std::cout << "GLEW_ARB_vertex_programがありません" << std::endl;
    }

    //GLMテスト
    glm::vec4 position = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0),
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0));

    glm::mat4 mv = view * model;
    glm::vec4 transformed = mv * position;

    //GLMの型をOpenGLへの入力として使う
    //glm::mat4 proj = glm::perspective(viewAngle, aspect, nearDist, farDist);
    //glUniformMatrix4fv(location, 1, GL_FALSE, &proj[0][0]);

    //GLSLとOpenGLのversionを決定する
    const GLubyte* renderer = glGetString(GL_RENDER);
    const GLubyte* vender = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

   /* std::cout << "GL_RENDER: " << renderer << std::endl;
    std::cout << "GL_VENDOR: " << vender << std::endl;
    std::cout << "GL_VERSION: " << version << std::endl;
    std::cout << "GL_SHADING_LANGUAGE_VERSION: " << glslVersion << std::endl;
    std::cout << "GL_MAJOR_VERSION: " << major << std::endl;
    std::cout << "GL_MINOR_VERSION: " << minor << std::endl;*/

    ////長くなるので使う際は注意
    //const GLubyte* extensions = glGetString(GL_EXTENSIONS);
    ////別の方法
    //GLint nExtensions;
    //glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
    //for (int i = 0; i < nExtensions; i++)
    //{
    //    printf("%s \n", glGetStringi(GL_EXTENSIONS, i));
    //}

};



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
};


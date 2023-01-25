#include "ScenePcf.h"
#include "../../chapter2/defines.h"

using glm::vec3;

using std::rand;
using std::srand;
#include <ctime>
using std::time;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>

void ScenePcf::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    //prog.setUniform("ProjectionMatrix", projection);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ShadowMatrix", lightPV * model);
}

void ScenePcf::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter7\\Pcf\\Shader\\PcfShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter7\\Pcf\\Shader\\PcfShader.frag", GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.link())
    {
        printf("Shader program failed to link!\n%s",
            prog.log().c_str());
        exit(1);
    }

    if (!prog.validate())
    {
        printf("Program failed to validate!\n%s",
            prog.log().c_str());
        exit(1);
    }
    prog.use();
}

void ScenePcf::setupFBO()
{
    GLfloat border[] = { 1.0f, 0.0f,0.0f,0.0f };
    // The depth buffer texture
    GLuint depthTex;
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth,
        shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    // Assign the depth buffer texture to texture channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTex);

    // Create and set up the FBO
    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depthTex, 0);

    GLenum drawBuffers[] = { GL_NONE };
    glDrawBuffers(1, drawBuffers);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is complete.\n");
    }
    else {
        printf("Framebuffer is not complete.\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ScenePcf::drawScene()
{
    vec3 color = vec3(0.7f, 0.5f, 0.3f);
    prog.setUniform("Material.Ka", color * 0.05f);
    prog.setUniform("Material.Kd", color);
    prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
    prog.setUniform("Material.Shininess", 150.0f);
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f, 0.0f, 0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    teapot->render();

    prog.setUniform("Material.Ka", color * 0.05f);
    prog.setUniform("Material.Kd", color);
    prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
    prog.setUniform("Material.Shininess", 150.0f);
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f, 2.0f, 5.0f));
    model *= glm::rotate(-60.0f, vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    torus->render();

    //prog.setUniform("Material.Kd", 0.25f, 0.25f, 0.25f);
    //prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    //prog.setUniform("Material.Ka", 0.05f, 0.05f, 0.05f);
    //prog.setUniform("Material.Shininess", 1.0f);
    //model = mat4(1.0f);
    //model *= glm::translate(vec3(0.0f, 0.0f, 0.0f));
    //setMatrices();
    //plane->render();
    //model = mat4(1.0f);
    //model *= glm::translate(vec3(-5.0f, 5.0f, 0.0f));
    //model *= glm::rotate(-90.0f, vec3(0.0f, 0.0f, 1.0f));
    //setMatrices();
    //plane->render();
    //model = mat4(1.0f);
    //model *= glm::translate(vec3(0.0f, 5.0f, -5.0f));
    //model *= glm::rotate(90.0f, vec3(1.0f, 0.0f, 0.0f));
    //setMatrices();
    //plane->render();
    //model = mat4(1.0f);
}

void ScenePcf::drawBuildingScene()
{
    vec3 color = vec3(1.0f, 0.85f, 0.55f);
    prog.setUniform("Material.Ka", color * 0.1f);
    prog.setUniform("Material.Kd", color);
    prog.setUniform("Material.Ks", vec3(0.0f));
    prog.setUniform("Material.Shininess", 1.0f);
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f, 0.0f, 0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    mesh->render();

    //prog.setUniform("Material.Kd", 0.25f, 0.25f, 0.25f);
    //prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    //prog.setUniform("Material.Ka", 0.05f, 0.05f, 0.05f);
    //prog.setUniform("Material.Shininess", 1.0f);
    //model = mat4(1.0f);
    //model *= glm::translate(vec3(0.0f, 0.0f, 0.0f));
    //setMatrices();
    //plane->render();
}

ScenePcf::ScenePcf()
{
    width = 800;
    height = 600;
    shadowMapWidth = 512;
    shadowMapHeight = 512;
}

void ScenePcf::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    
    angle = TWOPI * 1.85f;
    
    teapot = new TeaPot(14, mat4(1.0f));
    plane = new VBOPlane(40.f, 40.f, 2, 2);
    float scale = 2.0f;
    torus = new VBOTorus(0.7f * scale, 0.3f * scale, 50, 50);
    mesh = new VBOMesh("chapter7\\Mesh\\building.obj");

    setupFBO();

    GLuint programHandle = prog.getHandle();
    pass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "recordDepth");
    pass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "shadeWithShadow");

    shadowBias = mat4(vec4(0.5f, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, 0.5f, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 0.5f, 0.0f),
        vec4(0.5f, 0.5f, 0.5f, 1.0f)
    );

    lightFrustum = new Frustum(Projection::PERSPECTIVE);
    lightPos = vec3(-2.5f, 2.0f, -2.5f);  // World coords
    lightFrustum->orient(lightPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    lightFrustum->setPerspective(40.0f, 1.0f, 0.1f, 100.0f);

    lightPV = shadowBias * lightFrustum->getProjectionMatrix() * lightFrustum->getViewMatrix();

    prog.setUniform("Light.Intensity", vec3(0.85f));

    prog.setUniform("ShadowMap", 0);
}

void ScenePcf::render()
{
    //pass1
    view = lightFrustum->getViewMatrix();
    projection = lightFrustum->getProjectionMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, shadowMapWidth, shadowMapHeight);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.5f, 10.f);
    glCullFace(GL_BACK);
    glDisable(GL_POLYGON_OFFSET_FILL);

    //pass2
    float c = 1.0f;
    vec3 cameraPos(c * 1.5f * cos(angle), c * 7.0f, c * 1.5f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    prog.setUniform("Light.Position", view * vec4(lightFrustum->getOrigin(), 1.0f));
    projection = glm::perspective(90.0f, (float)width / height, 0.1f, 100.0f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);
    glDisable(GL_CULL_FACE);
   //drawBuildingScene();
    drawScene();
    glFinish();

}

void ScenePcf::update(float t)
{
    angle += 0.001f;
    if (angle > TWOPI) angle -= TWOPI;
}

void ScenePcf::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}

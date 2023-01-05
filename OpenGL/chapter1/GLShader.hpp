#ifndef GLSHADER_H
#define GLSHADER_H

#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

//Shaderファイルを読み込む
GLuint LoadShader(const char* vertex_path, const char* fragment_path);

//シェーダをバインドする
void BindShaders(GLuint* programHandle, GLuint* vaoHandle, GLuint* vboHandle);

//ユニフォーム変数を使ってシェーダーにデータを送る
//三角形を回転
void SendRotationUniform(GLuint* programHandle, float angle, GLuint* vaoHandle);

//個々のアクティブな属性についての情報を取得してプリントする
void PrintActibAttribs(GLuint* programHandle);

//アクティブなユニフォーム変数のリストを得る
void PrintActiveUniformAttribs(GLuint* programHandle);

//シェーダをバインドする
//ユニフォーム用
void BindUniformShaders(GLuint* programHandle, GLuint* vaoHandle, GLuint* vboHandle);

//ユニフォームブロックとユニフォームバッファオブジェクトを使う
void UseUniformBlockAndBuffer(GLuint* programHandle);
void renderUseUniformBlockAndBuffer(GLuint* vaoHandle);


#endif
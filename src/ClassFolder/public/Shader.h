#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // program ID
    unsigned int ID;

public:
    // 생성자는 shader를 읽고 생성합니다.
    Shader(const char *vertexPath, const char *fragmentPath);

    // shader를 활성화하고 사용합니다.
    void use();
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;

    //Shader Clear용 함수
    void clear();

    //Shader를 초기화할때 사용합니다.
    GLuint CompileShader(GLenum shaderType, const std::string& shaderCode);
    void CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};


#endif //end SHADER_H
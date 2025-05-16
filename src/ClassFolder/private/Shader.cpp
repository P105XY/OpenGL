#include "public/Shader.h"


Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    CreateShader(vertexPath, fragmentPath);
}

Shader::Shader() {}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, const bool& value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, const int& value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, const float& value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::clear()
{
    glDeleteProgram(ID);
}

GLuint Shader::CompileShader(GLenum shaderType, const std::string &shaderCode)
{
    GLuint shaderID = glCreateShader(shaderType);
    const GLchar *shadersource = shaderCode.c_str();
    glShaderSource(shaderID, 1, &shadersource, nullptr);
    glCompileShader(shaderID);

    GLint result;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char *errorMessage = new char(length * sizeof(char));

        glGetShaderInfoLog(shaderID, length, &length, errorMessage);
        std::cout << "ERROR : " << ((shaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment") << "SHADER" << std::endl;
        std::cout << "ERROR MESSAGE : " << errorMessage << std::endl;

        glDeleteShader(shaderID);
        delete (errorMessage);
        return -1;
    }

    return shaderID;
}

void Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    GLuint programID = glCreateProgram();

    GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Shader생성 후 Shader사용할 Program에 Shader를 linking.
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);

    // 이후 program이 valid인지 체크.
    glValidateProgram(programID);

    // Shader생성 이후 링크가 끝났으므로, Shader를 제거
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    this->ID = programID;
}

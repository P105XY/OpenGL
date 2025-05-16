#include "public/Light.h"
#include "public/Shader.h"
#include "Util/FileSupport.h"

Light::Light(){}

Light::Light(const char *vertexPath, const char *fragmentPath, const glm::vec3 &initLightColor)
{
    this->lightColor = initLightColor;
    std::string LightVSCodeStr;
    FileFromString("src/ClassFolder/Shader/LightShader.vs", LightVSCodeStr);

    std::string LightFSCodeStr;
    FileFromString("src/ClassFolder/Shader/LightShader.fs", LightFSCodeStr);
    CreateShader(LightVSCodeStr.c_str(), LightFSCodeStr.c_str());

    lightColor = initLightColor;
    CreateLight();
    this->lightID = ID;
}

void Light::SetLightColor(const glm::vec3 &inLightColor)
{
    this->lightColor = inLightColor;
}

void Light::SetLightObjectColor(const glm::vec3 &inLightObjectColor)
{
    this->lightObjectColor = inLightObjectColor;
}

void Light::SetLightSpecular(const glm::vec3 &inLightSpecular)
{
    this->lightSpecular = inLightSpecular;
}

void Light::SetLightAmbient(const glm::vec3 &inLightAmbient)
{
    this->lightAmbient = inLightAmbient;
}

void Light::CreateLight() 
{
    float lightVertices[] = {
    // back face (Z = -0.5)
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    // front face (Z = +0.5)
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    // left face (X = -0.5)
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    // right face (X = +0.5)
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    // bottom face (Y = -0.5)
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    // top face (Y = +0.5)
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

    glGenVertexArrays(1, &lightObjVAO);
    glGenBuffers(1, &lightobjVBO);

    glBindBuffer(GL_ARRAY_BUFFER, lightobjVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

    glBindVertexArray(lightObjVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Light::DrawLightObject(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
{
    use();
    setVec3("objectColor", lightObjectColor);
    setVec3("lightColor", lightColor);

    setMat4("projection", projection);
    setMat4("view", view);
    setMat4("model", model);

    glBindVertexArray(lightObjVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

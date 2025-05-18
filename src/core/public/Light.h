#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw/3.4/include/GLFW/glfw3.h>
#include <iostream>

#include "public/Shader.h"

const glm::vec3 RedColor(glm::vec3(1.0f,0.0f,0.0f));

class Light : public Shader
{
public:
    unsigned int lightID;

    unsigned int lightObjVAO;
    unsigned int lightobjVBO;

    glm::vec3 lightColor;
    glm::vec3 lightObjectColor;
    glm::vec3 lightPosition;

    glm::vec3 lightSpecular;
    glm::vec3 lightAmbient;

public:
    Light();
    Light(const char *vertexPath, const char *fragmentPath, const glm::vec3& initLightColor);

    void SetLightColor(const glm::vec3& inLightColor);
    void SetLightObjectColor(const glm::vec3& inLightObjectColor);
    void SetLightSpecular(const glm::vec3& inLightSpecular);
    void SetLightAmbient(const glm::vec3& inLightAmbient);
    void CreateLight();
    void DrawLightObject(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
};  
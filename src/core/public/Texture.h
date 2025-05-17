#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Texture
{
public:
    std::vector<GLuint> textureIDList;

public:
    Texture();
    void CreateTexture(const std::string textureFilePath);
    void SetTexture(const unsigned int &VAO, const unsigned int &programID);
};
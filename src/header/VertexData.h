#include <iostream>
#include <math.h>
#include <string>
#include <glm.hpp>

//2차원에서 정점을 나타낼 때 사용할 위치 구조체
struct Vector2DStruct
{
    float xPosition;
    float yPosition;
};

//3차원용 정점 데이터 구조체
struct Vertex3DStruct
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};

//텍스쳐용 구조체
struct TextureStruct
{
    unsigned int textureID;
    std::string type;
};

//간단한 색상을 나타낼 구조체
struct ColorStruct
{
    float r;
    float g;
    float b;
};

//정점 하나를 구성하며, 내부 데이터를 채워넣을 구조체
struct Vertex2DObject
{
    Vector2DStruct position;
    ColorStruct Color;
};

struct Vertex3DObject
{

};

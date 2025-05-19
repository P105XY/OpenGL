#ifndef MODELDATA_H
#define MODELDATA_H

#include <iostream>
#include <math.h>
#include <string>
#include <glm.hpp>
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <Shader.h>

#include "public/Texture.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
    public:
        /*  Mesh 데이터  */
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        /*  함수         */
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void Draw(Shader shader);
    private:
        /*  렌더 데이터  */
        unsigned int VAO, VBO, EBO;
        /*  함수         */
        void setupMesh();
};  

class ModelClass
{
public:
    ModelClass(const char *path)
    {
    }

private:
};

#endif //End MODELDATA_H
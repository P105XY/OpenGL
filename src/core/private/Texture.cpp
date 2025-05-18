#include "public/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "public/std_image.h"

Texture::Texture()
{
}

void Texture::CreateTexture(const std::string textureFilePath)
{
    // 텍스쳐 생성 후, 바인딩 진행.
    // 이후 해당 텍스쳐ID를 사용해서 텍스쳐에 접근한다.
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 텍스쳐의 warpping, filtering옵션을 설정한다.
    // 대상은 현재 바인딩된 텍스쳐에 대해서 설정한다.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 텍스쳐에 대한 옵션, 바인딩이 완료되면 텍스쳐를 로드, 생성한다.
    int textureWidth, textureHeight, textureChannel;
    unsigned char *textureData = stbi_load(
        textureFilePath.c_str(),
        &textureWidth,
        &textureHeight,
        &textureChannel,
        0);

    if (textureData)
    {
        // 텍스쳐 로드 생성이 완료되면, 바인딩된 텍스쳐에 대해서 이미지를 입력해 준다.
        // 이미지 입력이 종료되면 밉맵을 생성한다.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     textureWidth, textureHeight, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed load texture" << std::endl;
    }

    // 텍스쳐의 로드, 생성, 옵션 설정 등 모든 설정이 종료 시 호출.
    // 이미지 데이터에 대한 메모리를 반환한 뒤, 텍스쳐 ID를 설정해 준다.
    stbi_image_free(textureData);
    this->textureIDList.push_back(textureID);
}

void Texture::SetTexture(const unsigned int &VAO, const unsigned int &programID)
{
    //setXXX호출 이전에 shader 를 use해 준다.
    glUseProgram(programID);

    //현재 텍스쳐ID가 저장되어 있는 리스트의 크기를 사용해서 배열을 생성한다.
    const GLuint textureListSize = static_cast<GLuint>(textureIDList.size());
    std::vector<GLint> unitIndices(textureListSize);

    //위에서 생성한 배열을 통해서 n번째 텍스쳐를 사용함. n번째 textureID에 바인딩한다. 등을 설정.
    //마지막으로 n번째 텍스쳐 유닛을 전달해서, 사용할 수 있도록 한다.
    for (GLuint i = 0; i < textureIDList.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textureIDList[i]);

        unitIndices[i] = static_cast<GLint>(i);
    }

    //shader code에 있는 각각의 변수를 가져와서 데이터를 입력한다.
    GLuint locTextures = glGetUniformLocation(programID, "textures");
    GLuint locCount = glGetUniformLocation(programID, "bindedtexturesize");

    //카운트는 사용할 텍스쳐의 갯수, textures는 실제로 사용할 텍스쳐의 객체 ID를 의미한다.
    glUniform1i(locCount, static_cast<GLint>(textureListSize));
    glUniform1iv(locTextures, static_cast<GLsizei>(textureListSize), unitIndices.data());
}
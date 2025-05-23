#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 texCoord;
out vec4 outColor;
out vec3 Normal;
out vec3 FragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 ourColor;

void main()
{
    outColor = ourColor;
    texCoord = aTexCoord;

    FragPosition = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
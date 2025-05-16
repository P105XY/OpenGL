#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;
out vec4 outColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 ourColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    outColor = ourColor;
    texCoord = aTexCoord;
}
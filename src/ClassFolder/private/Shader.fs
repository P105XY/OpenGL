#version 330 core

out vec4 fracColor;
in vec3 ourColor;

void main()
{
    fracColor = vec4(ourColor, 1.0f);
}
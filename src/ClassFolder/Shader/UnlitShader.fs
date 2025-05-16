#version 330 core

out vec4 fracColor;

in vec4 outColor; 
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    fracColor = texture(ourTexture, texCoord) * vec4(lightColor * objectColor, 1.0);
}
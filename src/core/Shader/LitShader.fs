#version 330 core

//메시의 색상을 정해주는 Struct
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
};

//액터가 받는 빛의 색을 정해주는 Struct
struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 fracColor;

in vec4 outColor; 
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPosition;

const int MAX_TEXTURE = 16;

uniform sampler2D textures[MAX_TEXTURE];
uniform int bindedtexturesize;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform Light light;
uniform Material material;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPosition);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 viewDirection = normalize(viewPosition - FragPosition);
    vec3 reflectDirection = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 256);

    vec3 ambient  = light.ambient * material.ambient;
    vec3 diffuse  = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);  

    vec4 tex1 = texture(textures[0], texCoord);
    vec4 tex2 = texture(textures[1], texCoord);

    vec3 emissiveColor = texture(textures[1], texCoord).rgb;
    emissiveColor *= 5.0;

    vec3 result = (ambient + diffuse + specular + emissiveColor) * objectColor;
    fracColor = mix(tex1, tex2, 0.5) * vec4(result, 1.0);
}
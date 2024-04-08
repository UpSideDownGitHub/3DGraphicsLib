#pragma once
#include "Includes.h"

class Shader
{
public:
    // constructor
    Shader(GLuint*);
    
    // methods
    void initShaders();

private:
    // program
    GLuint *program;

    // Shader Sources
    const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;        
uniform mat4 modelMatrix;

out vec3 FragColor;
out vec3 Normal;
out vec3 FragPos; 
out vec2 TexCord; 

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
    FragColor = color;
    Normal = vec3(modelMatrix * vec4(normal, 1.0));
    FragPos = vec3(modelMatrix * vec4(position, 1.0));
    TexCord = texCord;
}
)";

    const char* fragmentShaderSource = R"(
#version 330 core

in vec3 FragColor;
in vec3 Normal; 
in vec3 FragPos; 
in vec2 TexCord;

uniform mat4 modelMatrixfrag;
uniform vec3 lightPosition; 
uniform vec3 lightColor;    
uniform sampler2D textureSampler;
uniform vec3 cameraPosition;

out vec4 FragColorOut; 

void main() {

    vec4 texColor = texture(textureSampler, TexCord).rgba;

    // Calculate ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * texColor.rgb;

    vec3 norm = Normal;

    // Calculate diffuse lighting
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * texColor.rgb;

    // Calculate specular lighting
    float specularStrength = 0.1;
    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = specularStrength * spec * lightColor;

    // Final color calculation
    vec3 result = ambient + diffuse + specular;
    FragColorOut = vec4(result, texColor.a);
}
)";

};


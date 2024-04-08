#include "Shader.h"

Shader::Shader(GLuint *program){ 
    this->program = program;
}

void Shader::initShaders() {

    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cerr << "Error: Vertex shader compilation failed";
        exit(1);
    }

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for fragment shader compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cerr << "Error: Fragment shader compilation failed";
        exit(1);
    }

    // Link shaders into a program
    *program = glCreateProgram();
    glAttachShader(*program, vertexShader);
    glAttachShader(*program, fragmentShader);
    glLinkProgram(*program);

    // Check for program linking errors
    glGetProgramiv(*program, GL_LINK_STATUS, &success);
    if (!success) {
        std::cerr << "Error: Program linking failed";
        exit(1);
    }

    // Delete shaders as they are now linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
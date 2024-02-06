/*
#include "Main.h"

// Shader source code
const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;  // Added for normals

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;        // Added for eye space position
uniform mat4 modelMatrix;

out vec3 Normal;
out vec3 FragPos;

void main() {
  mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
  Normal = normalMatrix * normal;
  FragPos = vec3(modelMatrix * vec4(position, 1.0));
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core

in vec3 Normal;
in vec3 FragPos;

uniform
 
vec3 lightPosition;
uniform
 
vec3 ambientColor;
uniform
 
vec3 diffuseColor;
uniform
 
vec3 specularColor;
uniform
 
float shininess;

out vec4 fragmentColor;

void main() {
  // Calculate vectors
  vec3 lightDirection = normalize(lightPosition - FragPos);
  float diffuseFactor = max(dot(Normal, lightDirection), 0.0f);
  vec3 halfVector = normalize(lightDirection + normalize(-FragPos));  // Simplified for directional light
  float specularFactor = pow(max(dot(Normal, halfVector), 0.0f), shininess);

  // Apply lighting equation
  vec3 finalColor = ambientColor + diffuseColor * diffuseFactor + specularColor * specularFactor;

  fragmentColor = vec4(finalColor, 1.0);
}
)";

GLuint program;
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
glm::mat4 modelMatrix;
glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.0f); // Orange color
glm::vec3 lightPosition = glm::vec3(1.0f, 0.5f, 0.0f);
glm::vec3 ambientColor = glm::vec3(1.0f, 0.5f, 0.0f); 
glm::vec3 diffuseColor = glm::vec3(1.0f, 0.5f, 0.0f); 
glm::vec3 specularColor = glm::vec3(1.0f, 0.5f, 0.0f); 
float shininess = 0.5f;

const int numVertices = 100;
const int numIndices = numVertices * 6;
std::vector<GLfloat> vertices;
std::vector<GLuint> indices;

GLuint vao;
GLuint vbo;
GLuint ebo;

void init_torus() {
    for (int i = 0; i < numVertices; i++) {
        float angle1 = 2 * M_PI * i / numVertices;
        float angle2 = 2 * M_PI * i / (numVertices / 2);
        float x = (0.5 * cos(angle1) + 2 * cos(angle2)) * cos(angle1);
        float y = (0.5 * sin(angle1) + 2 * cos(angle2)) * sin(angle1);
        float z = 2 * sin(angle2);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    for (int i = 0; i < numVertices; i++) {
        indices.push_back(i);
        indices.push_back((i + 1) % numVertices);
        indices.push_back((i + numVertices / 2) % numVertices);
        indices.push_back((i + 1) % numVertices);
        indices.push_back((i + 1 + numVertices / 2) % numVertices);
        indices.push_back((i + numVertices / 2) % numVertices);
    }
}

void init_vertex_array() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

// Rendering functions
void display() {
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader program
    glUseProgram(program);

    // Set uniform variables
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(program, "objectColor"), 1, glm::value_ptr(objectColor));

    glUniform3fv(glGetUniformLocation(program, "lightPosition"), 1, glm::value_ptr(lightPosition));
    glUniform3fv(glGetUniformLocation(program, "ambientColor"), 1, glm::value_ptr(ambientColor));
    glUniform3fv(glGetUniformLocation(program, "diffuseColor"), 1, glm::value_ptr(diffuseColor));
    glUniform3fv(glGetUniformLocation(program, "specularColor"), 1, glm::value_ptr(specularColor));
    glUniform1f(glGetUniformLocation(program, "shininess"), shininess);

    // Draw torus
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

    // Swap buffers to display the rendered frame
    glutSwapBuffers();
}
void update() {

}



int main(int argc, char* argv[]) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Basic OpenGL Shaders");
    
    glewExperimental = GL_TRUE;  // Needed for some systems
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(1);
    }

    // Initialize scene and OpenGL state
    init_window(800, 600);

    // Initialize shaders
    init_shaders();

    // Initialize torus
    init_torus();
    init_vertex_array();

    // Register callbacks
    glutDisplayFunc(display);
    glutIdleFunc(update);

    // Enter main loop
    glutMainLoop();
}


void init_window(int width, int height) {
    // Set viewport
    glViewport(0, 0, width, height);

    // Set background color
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Setup projection matrix
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);

    // Setup view matrix
    viewMatrix = glm::lookAt(glm::vec3(0.0f, -400.0f, 3.0f), glm::vec3(0.0f, 0.0f, -0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void init_shaders() {
    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Error: Vertex shader compilation failed: " << infoLog << std::endl;
        exit(1);
    }

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for fragment shader compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Error: Fragment shader compilation failed: " << infoLog << std::endl;
        exit(1);
    }

    // Link shaders into a program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for program linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Error: Program linking failed: " << infoLog << std::endl;
        exit(1);
    }

    // Delete shaders as they are now linked into the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
*/
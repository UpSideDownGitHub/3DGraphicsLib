#include "Includes.h"
#include "Shader.h"
#include "TextureLoader.h"
# define M_PI 3.14159265358979323846 /* pi */

GLuint vao, vbo, ibo;
GLuint normalVBO, colorVBO, texCordVBO;
int numVertices = 50;
int numTorusSlices = 50;
float torusRadius = 1.0f;
float tubeRadius = 0.3f;

float scaleFactor = 1.0f;
float scaleSpeed = 0.01f;
float rotationFactor = 1.0f;
float rotationSpeed = 1.0f;

GLuint program;
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
glm::mat4 modelMatrix(1.0f);

glm::vec3 lightPosition(0.0f, 200.0f, 5.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 cameraPosition(0.0f, 200.0f, 5.0f);

// shaders
Shader shaders = Shader(&program);

// textures
TextureLoader texture1 = TextureLoader();

void createTorus() {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> colors;
    std::vector<GLfloat> texCords;
    std::vector<GLushort> indices;


    for (int i = 0; i < numTorusSlices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            float theta = 2.0f * M_PI * j / numVertices;
            float phi = 2.0f * M_PI * i / numTorusSlices;

            float x = (torusRadius + tubeRadius * std::cos(theta)) * std::cos(phi);
            float y = (torusRadius + tubeRadius * std::cos(theta)) * std::sin(phi);
            float z = tubeRadius * std::sin(theta);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Calculate partial derivatives for normal calculation
            float dx_dtheta = -tubeRadius * std::sin(theta) * std::cos(phi);
            float dy_dtheta = -tubeRadius * std::sin(theta) * std::sin(phi);
            float dz_dtheta = tubeRadius * std::cos(theta);

            float dx_dphi = -(torusRadius + tubeRadius * std::cos(theta)) * std::sin(phi);
            float dy_dphi = (torusRadius + tubeRadius * std::cos(theta)) * std::cos(phi);
            float dz_dphi = 0.0f; // No change in z with respect to phi

            // Calculate the normal vector using cross product
            float nx = dy_dphi * dz_dtheta - dz_dphi * dy_dtheta;
            float ny = dz_dphi * dx_dtheta - dx_dphi * dz_dtheta;
            float nz = dx_dphi * dy_dtheta - dy_dphi * dx_dtheta;

            // Normalize the normal vector
            float normalLength = std::sqrt(nx * nx + ny * ny + nz * nz);
            nx /= normalLength;
            ny /= normalLength;
            nz /= normalLength;

            // Add the normal components to the output vector
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // Assign colors (assuming per-vertex colors)
            colors.push_back(1.0f); // Replace with actual color
            colors.push_back(0.5f); // Replace with actual color
            colors.push_back(0.0f); // Replace with actual color

            // UV's
            texCords.push_back(static_cast<float>(i) / static_cast<float>(numTorusSlices));
            texCords.push_back(static_cast<float>(j) / static_cast<float>(numVertices));
        }
    }

    // Generate indices for rendering
    for (int i = 0; i < numTorusSlices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            int nextJ = (j + 1) % numVertices;
            int nextI = (i + 1) % numTorusSlices;

            int currentIndex = i * numVertices + j;
            int nextIndex = i * numVertices + nextJ;
            int nextIIndex = nextI * numVertices + j;
            int nextINextJIndex = nextI * numVertices + nextJ;

            indices.push_back(currentIndex);
            indices.push_back(nextIndex);
            indices.push_back(nextIIndex);

            indices.push_back(nextIndex);
            indices.push_back(nextINextJIndex);
            indices.push_back(nextIIndex);
        }
    }


    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Specify attribute pointers for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Generate and bind VBO for normals
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);

    // Specify attribute pointers for normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // Generate and bind VBO for colors
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

    // Specify attribute pointers for colors
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    // Generate and bind VBO for texture coordinates
    glGenBuffers(1, &texCordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCordVBO);
    glBufferData(GL_ARRAY_BUFFER, texCords.size() * sizeof(GLfloat), texCords.data(), GL_STATIC_DRAW);

    // Specify attribute pointers for texture coordinates
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);

    // Generate and bind index buffer
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, texture1.textureID);
    glUseProgram(program);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix *= glm::rotate(modelMatrix, rotationFactor, glm::vec3(1.0f, 0.0f, 0.0f));;
    modelMatrix *= glm::scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));


    // Set uniform variables
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrixfrag"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(program, "lightPosition"), 1, glm::value_ptr(lightPosition));
    glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, glm::value_ptr(cameraPosition));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numTorusSlices * numVertices * 6, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glutSwapBuffers();
}

void update(int value) {
    glutPostRedisplay(); 

    rotationFactor += rotationSpeed;
    if (rotationFactor >= 360)
        rotationFactor = 0;

    scaleFactor += scaleSpeed;
    if ((scaleFactor > 1.1f && scaleSpeed > 0) || (scaleFactor < 0.3f && scaleSpeed < 0))
        scaleSpeed = -scaleSpeed;

    glutTimerFunc(16, update, 0); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Torus");

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(1);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    createTorus();
    shaders.initShaders();
    texture1.initTextures("two.bmp");
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 1.0f, 1000.0f);
    viewMatrix = glm::lookAt(glm::vec3(0.0f, 200.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glutDisplayFunc(render);
    glutTimerFunc(16, update, 0); 

    glutMainLoop();

    return 0;
}

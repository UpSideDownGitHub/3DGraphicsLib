#include "Includes.h"
#include "Shader.h"
#include "TextureLoader.h"
#include "Shape.h"

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

// Torus
Torus torus = Torus();

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

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

    torus.render();

    glUseProgram(0);
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

    torus.initShape();
    torus.setTexture("two.bmp");
    shaders.initShaders();
    
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 1.0f, 1000.0f);
    viewMatrix = glm::lookAt(glm::vec3(0.0f, 200.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glutDisplayFunc(render);
    glutTimerFunc(16, update, 0); 

    glutMainLoop();

    return 0;
}

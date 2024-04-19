#include "Includes.h"
#include "Shader.h"
#include "TextureLoader.h"
#include "Shape.h"

// scale and rotation values
float scaleFactor = 1.0f;
float scaleSpeed = 0.01f;
float rotationFactor = 1.0f;
float rotationSpeed = 1.0f;

// GLM Matrices
GLuint program;
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
glm::mat4 modelMatrix(1.0f);

// Light & Camera
glm::vec3 lightPosition(0.0f, 200.0f, 5.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 cameraPosition(0.0f, 200.0f, 5.0f);

// shaders
Shader shaders = Shader(&program);

// Torus
Torus torus = Torus(10, 10, 1.0f, 0.5f);
Torus torus2 = Torus(26, 26, 1.0f, 0.3f);
Torus torus3 = Torus(6, 6, 1.0f, 0.3f);

/*
Main render function that will handle the rendering of all of the components
*/
void render() {
    // prepare the screen to be rendered to
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // set the current shader program to our program with custom vertex and fragement shaders
    glUseProgram(program);

    // apply transformations -> in order (RESET, ROTATE, SCALE)
    modelMatrix = glm::mat4(1.0f);
    modelMatrix *= glm::rotate(modelMatrix, rotationFactor, glm::vec3(1.0f, 0.0f, 0.0f));
    float scale = scaleFactor / 1.75f;
    modelMatrix *= glm::scale(modelMatrix, glm::vec3(scale, scale, scale));

    // Set uniform variables
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrixfrag"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(program, "lightPosition"), 1, glm::value_ptr(lightPosition));
    glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, glm::value_ptr(cameraPosition));

    // render the torus
    torus.render();

    // apply transfotmations -> in order (RESET, ROTATE, SCALE)
    modelMatrix = glm::mat4(1.0f);
    modelMatrix *= glm::rotate(modelMatrix, rotationFactor / 2, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix *= glm::rotate(modelMatrix, rotationFactor / 2, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix *= glm::scale(modelMatrix, glm::vec3(scale * 2, scale * 2, scale * 2));

    // Set uniform variables
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrixfrag"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(program, "lightPosition"), 1, glm::value_ptr(lightPosition));
    glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, glm::value_ptr(cameraPosition));

    // render the torus
    torus2.render();

    // apply transfotmations -> in order (RESET, ROTATE, SCALE)
    modelMatrix = glm::mat4(1.0f);
    modelMatrix *= glm::rotate(modelMatrix, rotationFactor , glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix *= glm::rotate(modelMatrix, rotationFactor , glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix *= glm::scale(modelMatrix, glm::vec3(scale / 2, scale / 2, scale / 2));

    // Set uniform variables
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrixfrag"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(program, "lightPosition"), 1, glm::value_ptr(lightPosition));
    glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, glm::value_ptr(cameraPosition));

    // render the torus
    torus3.render();

    // reset the shader program and render to screen
    glUseProgram(0);
    glutSwapBuffers();
}

/*
    Handles the updating of the rotation and scale of the tori, this will be called 
    every 16 miliseconds (60FPS)

    This is started in the "int Main" method
*/
void update(int value) {
    glutPostRedisplay(); 

    //  update rotation (looping at 360)
    rotationFactor += rotationSpeed;
    if (rotationFactor >= 360)
        rotationFactor = 0;

    // update scale (going from 0.3 <-> 1.1)
    scaleFactor += scaleSpeed;
    if ((scaleFactor > 1.1f && scaleSpeed > 0) || (scaleFactor < 0.3f && scaleSpeed < 0))
        scaleSpeed = -scaleSpeed;
    
    // call this method again in 16 miliseconds (for 60FPS)
    glutTimerFunc(16, update, 0); 
}

/*
    Main function of the program, as such sets up the GLUT window, as well as 
    initilising the main parts of the program, such as shapes, textures, and shaders
*/
int main(int argc, char** argv) {
    // initilise GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Torus");

    // iniitlise GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(1);
    }

    // set the background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // initilise the 3 tori as well as the textures
    torus.initShape();
    torus.setTexture("two.bmp");
    torus2.initShape();
    torus2.setTexture("one.bmp");
    torus3.initShape();
    torus3.setTexture("three.bmp");

    // initilise the shaders
    shaders.initShaders();
    
    // set the current projection and view
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 1.0f, 1000.0f);
    viewMatrix = glm::lookAt(glm::vec3(0.0f, 200.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // set the defualt GLUT functions
    glutDisplayFunc(render);
    glutTimerFunc(16, update, 0); 

    // start the main loop (run the program)
    glutMainLoop();

    // DIE
    return 0;
}
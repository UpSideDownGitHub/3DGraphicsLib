#include "Shape.h"

void Shape::setTexture(const char* fileName) {
    texture.initTextures(fileName);
}

void Torus::render() {
    glBindTexture(GL_TEXTURE_2D, texture.textureID);

    glBindVertexArray(vao);
    //glDrawElements(GL_LINE_STRIP, numTorusSlices * numVertices * 6, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_TRIANGLES, numTorusSlices * numVertices * 6, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Torus::initShape(){
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> colors;
    std::vector<GLfloat> texCords;
    std::vector<GLushort> indices;


    for (int i = 0; i < numTorusSlices + 1; i++) {
        for (int j = 0; j < numVertices; j++) {
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
            float dz_dphi = 0.0f;

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
            colors.push_back(1.0f); 
            colors.push_back(0.5f); 
            colors.push_back(0.0f); 

            // to fix issue at the end rotate revers it so it goes 0->5->0
            // rather than 0->9 which causes last section to have entire texture
            float u = i > numTorusSlices / 2 ? ((float)numTorusSlices - (float)i) / (float)numTorusSlices : (float)i / numTorusSlices;
            float v = j > numVertices / 2 ? ((float)numVertices - (float)j) / (float)numVertices : (float)j / numVertices;
            texCords.push_back(u);
            texCords.push_back(v);
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
    glBufferData(GL_ARRAY_BUFFER, texCords.size() * sizeof(float), texCords.data(), GL_STATIC_DRAW);

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

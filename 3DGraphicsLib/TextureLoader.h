#pragma once
#include "Includes.h"

/*
    struct to hold the data concerning an image
        - width
        - height
        - data
*/
struct Image {
    int width;
    int height;
    std::vector<unsigned char> data;
};

class TextureLoader
{
public:
    // variables
    GLuint textureID;
    const char* image;

    // constructor
    TextureLoader();
    
    // methods
    void initTextures(const char*);
    Image loadImageBMP(const char*);
    void loadTextures();
private:
};


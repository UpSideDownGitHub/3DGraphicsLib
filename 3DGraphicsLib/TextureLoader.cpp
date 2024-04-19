#include "TextureLoader.h"

// Default Constructor
TextureLoader::TextureLoader(){ }

/*
    this will inisilise the texture with the given name
*/
void TextureLoader::initTextures(const char* file) {
    image = file;
    loadTextures();
}

/*
    this will load the texture and then Create the texture in OpenGL
    this involves:
        - generating the texture
        - binding it
        - setting texture parameters
        - genreating the image
        - genreating mipmaps
        - unbinding at end
*/
void TextureLoader::loadTextures(){
    // load texture
    Image loadedImage = loadImageBMP(image);
    //  create texture 
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, loadedImage.width, loadedImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, loadedImage.data.data());
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

/*
    this function will load the BMP called "filename" and turn it into 
        - width
        - height
        - data
*/
Image TextureLoader::loadImageBMP(const char* filename) {
    // load the image
    std::ifstream file(filename, std::ios::binary);
    // if the opening failed
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filename;
        exit(1);
    }

    // go to width and height
    file.seekg(18); 
    int width, height;
    // read the width and height from the image
    file.read(reinterpret_cast<char*>(&width), sizeof(int));
    file.read(reinterpret_cast<char*>(&height), sizeof(int));

    // go to main data
    file.seekg(54); 
    int dataSize = width * height * 3;
    // load the data from the main image
    std::vector<unsigned char> imageData(dataSize);
    file.read(reinterpret_cast<char*>(imageData.data()), dataSize);
    file.close();
    
    // create an instance of the Image struct
    // assign the image data and return it.
    Image l_image;
    l_image.width = width;
    l_image.height = height;
    l_image.data = std::move(imageData);
    return l_image;
}
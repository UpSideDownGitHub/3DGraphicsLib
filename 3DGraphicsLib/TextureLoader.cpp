#include "TextureLoader.h"

TextureLoader::TextureLoader(){ }

void TextureLoader::initTextures(const char* file) {
    image = file;
    loadTextures();
}

void TextureLoader::loadTextures(){
    Image loadedImage = loadImageBMP(image);

    //std::cout << "Width: " << loadedImage.width;
    //std::cout << "Height: " << loadedImage.height;
    //std::cout << "Data: " << loadedImage.data.data() << "\n";

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, loadedImage.width, loadedImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, loadedImage.data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Image TextureLoader::loadImageBMP(const char* filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filename;
        exit(1);
    }

    file.seekg(18); // go to width and height
    int width, height;
    file.read(reinterpret_cast<char*>(&width), sizeof(int));
    file.read(reinterpret_cast<char*>(&height), sizeof(int));

    file.seekg(54); // go to main data
    int dataSize = width * height * 3;
    std::vector<unsigned char> imageData(dataSize);
    file.read(reinterpret_cast<char*>(imageData.data()), dataSize);
    file.close();

    Image l_image;
    l_image.width = width;
    l_image.height = height;
    l_image.data = std::move(imageData);

    return l_image;
}
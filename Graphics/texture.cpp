#define STB_IMAGE_IMPLEMENTATION
#include "../Include/stb_image.h"
#include "../Include/texture.h"
#include <stdio.h>

GLuint loadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) { 
        printf("Gagal load image: %s\n", filename); 
        return 0; 
    }
    GLuint texture; 
    glGenTextures(1, &texture); 
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data); 
    return texture;
}

#define STB_IMAGE_IMPLEMENTATION
#include "../Include/stb_image.h"
#include "../Include/texture.h"
#include <stdio.h>

// ================= TEXTURE GLOBAL =================
// TARUH DI SINI
GLuint textureBuilding1 = 0;
GLuint textureBuilding2 = 0;
GLuint textureBuilding3 = 0;

GLuint loadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);

    if (!data) {
        printf("Failed to load texture: %s\n", filename);
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // make texture repeat instead of clamp/stretch
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    stbi_image_free(data);
    return texture;
}

// ================= LOAD BUILDING TEXTURES =================
// TARUH DI BAWAH loadTexture()
void loadBuildingTextures() {
    textureBuilding1 = loadTexture("Assets/Textures/textureBuilding1.jpg");
    textureBuilding2 = loadTexture("Assets/Textures/textureBuilding2.jpg");
    textureBuilding3 = loadTexture("Assets/Textures/textureBuilding3.jpg");
}

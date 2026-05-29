#define STB_IMAGE_IMPLEMENTATION
#include "../Include/stb_image.h"
#include "../Include/texture.h"
#include <stdio.h>

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

    // Important for JPG/RGB images.
    // Without this, some widths can become striped or corrupted.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    // GLU builds mipmaps and also helps older OpenGL handle non-power-of-two images.
    gluBuild2DMipmaps(
        GL_TEXTURE_2D,
        format,
        width,
        height,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // This is what makes texCoord values above 1.0 repeat instead of stretch.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
    return texture;
}

void loadBuildingTextures() {
    textureBuilding1 = loadTexture("Assets/Textures/textureBuilding1.jpg");
    textureBuilding2 = loadTexture("Assets/Textures/textureBuilding2.jpg");
    textureBuilding3 = loadTexture("Assets/Textures/textureBuilding3.jpg");
}

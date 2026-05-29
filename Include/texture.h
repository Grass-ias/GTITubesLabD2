#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>

extern GLuint textureBuilding1;
extern GLuint textureBuilding2;
extern GLuint textureBuilding3;

GLuint loadTexture(const char* filename);
void loadBuildingTextures();

#endif

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>

GLuint loadTexture(const char* filename);
extern GLuint textureBuilding1;
extern GLuint textureBuilding2;
extern GLuint textureBuilding3;

void loadBuildingTextures();
#endif

#ifndef DISPLAY_H
#define DISPLAY_H

#include "globals.h"

void drawTexturedBuilding(float sizeX, float sizeY, float sizeZ, GLuint texSide, GLuint texTop);
void setupBuildingMaterial(float brightness);
void resetBuildingMaterial();
void drawGame3D();

#endif

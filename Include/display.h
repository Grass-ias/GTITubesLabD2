#ifndef DISPLAY_H
#define DISPLAY_H

#include "globals.h"

void drawImage(GLuint tex, float x, float y, float w, float h);
void drawText(float x, float y, const char* text);
void begin2D();
void end2D();
void drawMenu();
void drawHelp();
void drawHUD();
void drawTexturedBuilding(float sizeX, float sizeY, float sizeZ);
void drawGame3D();

#endif

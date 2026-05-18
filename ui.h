#ifndef UI_H
#define UI_H

#include "globals.h"

GLuint loadTexture(const char* filename);
void drawImage(GLuint tex, float x, float y, float w, float h);
void drawText(float x, float y, const char* text);
void begin2D();
void end2D();
void drawMenu();
void drawHelp();
void drawHUD();
void setupLighting();
void drawGame3D();

#endif
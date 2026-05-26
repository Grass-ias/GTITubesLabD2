#ifndef UI_H
#define UI_H

#include "globals.h"

void drawImage(GLuint tex, float x, float y, float w, float h);
void drawText(float x, float y, const char* text);
void drawShadowText(float x, float y, const char* text, float r, float g, float b);
void begin2D();
void end2D();
void drawMenu();
void drawHelp();
void drawHUD();

#endif

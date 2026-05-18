#ifndef PHYSICS_H
#define PHYSICS_H

#include "globals.h"

LevelChunk generateLevelChunk(float startX, float startY, float startZ);
bool checkWallCollision(float testX, float testY, float testZ, float rX, float rZ, float h);
float getGroundY(float testX, float testZ, float oldY, float rX, float rZ);
void shiftChunks();

#endif
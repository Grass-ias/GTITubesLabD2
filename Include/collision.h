#ifndef COLLISION_H
#define COLLISION_H

bool checkWallCollision(float testX, float testY, float testZ, float rX, float rZ, float h);
float getGroundY(float testX, float testZ, float oldY, float rX, float rZ);
bool checkClimbableWall(float testX, float testY, float testZ, float rX, float rZ, float h);

#endif

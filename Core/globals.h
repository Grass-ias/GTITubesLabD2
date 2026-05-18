#ifndef GLOBALS_H
#define GLOBALS_H

#include <GL/glut.h>

#define PI 3.14159265359
#define JUMLAH_PLATFORM 19 

struct LevelChunk {
    float x[JUMLAH_PLATFORM], y[JUMLAH_PLATFORM], z[JUMLAH_PLATFORM];
    float sx[JUMLAH_PLATFORM], sz[JUMLAH_PLATFORM];
    int type[JUMLAH_PLATFORM];
    bool visited[JUMLAH_PLATFORM];
    bool active; 
};

extern GLuint titleTexture;
extern int gameState; 
extern int windowWidth;
extern int windowHeight;

extern LevelChunk prevChunk, currChunk, nextChunk;

extern float ballX;
extern float ballY; 
extern float ballZ;

extern float velX; 
extern float velZ;
extern float speedY;
extern const float gravity; 
extern bool isGrounded; 

extern bool isSprinting;
extern bool isCrouching;
extern bool isSliding;
extern bool isTestMap; 

extern float entityZ;
extern float entitySpeed;

extern bool isHanging;
extern bool isClimbing;
extern int climbTimer;
extern const int MAX_CLIMB; 

extern float respawnX;
extern float respawnY;
extern float respawnZ;

extern float currentRadiusX; 
extern float currentRadiusZ; 
extern float currentHeight; 

extern float yaw;   
extern float pitch; 
extern bool keys[256];

#endif

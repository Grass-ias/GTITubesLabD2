#ifndef GLOBALS_H
#define GLOBALS_H

#include <GL/glut.h>

#define PI 3.14159265359
#define JUMLAH_PLATFORM 19 

struct LevelChunk {
    float x[JUMLAH_PLATFORM], y[JUMLAH_PLATFORM], z[JUMLAH_PLATFORM];
    float sx[JUMLAH_PLATFORM], sy[JUMLAH_PLATFORM], sz[JUMLAH_PLATFORM];
    int type[JUMLAH_PLATFORM];
    bool visited[JUMLAH_PLATFORM];
    bool active; 
    float color[JUMLAH_PLATFORM];

    bool hasPipe[JUMLAH_PLATFORM];
    float pipeY[JUMLAH_PLATFORM];
    float pipeZ[JUMLAH_PLATFORM];
    float pipeRadius[JUMLAH_PLATFORM];
    float pipeLength[JUMLAH_PLATFORM];
    float bgX[40];
    float bgY[40];
    float bgZ[40];
    float bgWidth[40];
    float bgHeight[40];
    int platTexIdx[JUMLAH_PLATFORM];
    int bgTexIdx[40];
};

extern GLuint titleTexture;
extern GLuint enemyTexture;
extern GLuint sideTex[3];
extern GLuint topTex[3];
extern int gameState; 
extern int windowWidth;
extern int windowHeight;

extern LevelChunk prevChunk, currChunk, nextChunk;

extern float playerX;
extern float playerY; 
extern float playerZ;

extern float velX; 
extern float velZ;
extern float speedY;
extern const float gravity; 
extern bool isGrounded; 

extern bool isSprinting;
extern bool isCrouching;
extern bool isSliding;
extern bool isTestMap; 

extern float enemyY;
extern float enemyZ;
extern float backEnemySpeed;
extern float enemyYTrackingTimer;

extern GLUquadricObj* quadricEnemy;

extern bool chaseStarted;
extern int introTimer;

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

extern int checkpointsPassed;
extern float currentFogStart;
extern float currentFogEnd;

extern bool frontEnemyActive;
extern float frontEnemyZ;

extern bool gameWon;
extern bool gameLost;
extern int deathType;

extern float yaw;   
extern float pitch; 
extern bool keys[256];

#endif

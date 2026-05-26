#include "../Include/globals.h"

GLuint titleTexture;
GLuint enemyTexture; 
GLuint sideTex[3];
GLuint topTex[3];
int gameState = 0; 
int windowWidth = 900;
int windowHeight = 600;

LevelChunk prevChunk, currChunk, nextChunk;

float playerX = 0.0f;
float playerY = 0.0f; 
float playerZ = -5.0f;

float velX = 0.0f; 
float velZ = 0.0f;
float speedY = 0.0f;
const float gravity = -0.015f; 
bool isGrounded = false; 

bool isSprinting = false;
bool isCrouching = false;
bool isSliding = false;
bool isTestMap = false; 

float enemyY = 5.0f;
float enemyZ = 15.0f;
float backEnemySpeed = 0.1f;
float enemyYTrackingTimer = 0.0f;

GLUquadricObj* quadricEnemy = NULL;

bool chaseStarted = false;
int introTimer = 0;

int checkpointsPassed = 0;
float currentFogStart = 15.0f;
float currentFogEnd = 70.0f;
bool frontEnemyActive = false;
float frontEnemyZ = 0.0f;

bool gameWon = false;
bool gameLost = false;
int deathType = 0;

bool isHanging = false;
bool isClimbing = false;
int climbTimer = 0;
const int MAX_CLIMB = 30; 

float respawnX = 0.0f;
float respawnY = 0.0f;
float respawnZ = -5.0f;

float currentRadiusX = 0.3f; 
float currentRadiusZ = 0.3f; 
float currentHeight = 1.2f; 

float yaw = 0.0f;   
float pitch = 0.0f; 
bool keys[256];

#include "globals.h"

GLuint titleTexture; 
int gameState = 0; 
int windowWidth = 900;
int windowHeight = 600;

LevelChunk prevChunk, currChunk, nextChunk;

float ballX = 0.0f;
float ballY = 0.0f; 
float ballZ = -5.0f;

float velX = 0.0f; 
float velZ = 0.0f;
float speedY = 0.0f;
const float gravity = -0.015f; 
bool isGrounded = false; 

bool isSprinting = false;
bool isCrouching = false;
bool isSliding = false;
bool isTestMap = false; 

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
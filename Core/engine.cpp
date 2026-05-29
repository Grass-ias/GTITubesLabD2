#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#include "../Include/globals.h"
#include "../Include/engine.h"
#include "../Include/input.h"
#include "../Include/player.h"
#include "../Include/enemy.h"
#include "../Include/map.h"
#include "../Include/ui.h"
#include "../Include/display.h"
#include "../Include/texture.h"
#include "../Include/sound.h"
#include "../Include/lighting.h"
#include "../Include/dice.h"

static int diceSpawnTimer = 0;

void timer(int value) {
    if (gameState == 1) {
        updatePlayer(); 
        
        if (introTimer > 0) {
            introTimer--;
            if (introTimer == 0) {
                chaseStarted = true;
                enemyZ = playerZ + 60.0f;
                currentFogStart = 12.0f;
                currentFogEnd = 62.0f;
                diceSpawnTimer = 40;
            }
        }
        
        if (chaseStarted) {
            updateEnemy();

            if (dice.active) {
                updateDice();
            }
            else {
                if (diceSpawnTimer > 0) {
                    diceSpawnTimer--;
                }
                else {
                    spawnDiceFromSide();
                    diceSpawnTimer = 170 + (rand() % 90);
                }
            }
        }
        if (!isTestMap) {
            float triggerZ = currChunk.z[18] + (currChunk.sz[18] / 2.0f); 
            
            if (playerZ <= triggerZ) {
                shiftChunks(); 

                checkpointsPassed++;
                
                if (!chaseStarted && introTimer == 0) {
                    introTimer = 150;
                } 
                else if (chaseStarted) {
                    backEnemySpeed += 0.001f;
                    
                    if (currentFogEnd > 40.0f) currentFogEnd -= 10.0f;
                    if (currentFogStart > 10.0f) currentFogStart -= 4.0f;
                }
            }
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); 
}

void display() {    
    if (!chaseStarted && introTimer == 0) {
        glClearColor(0.1f, 0.15f, 0.18f, 1.0f); 
        glEnable(GL_FOG);
        GLfloat fogColor[4];
        fogColor[0] = 0.1f;
        fogColor[1] = 0.15f;
        fogColor[2] = 0.18f;
        fogColor[3] = 1.0f;
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_START, 15.0f);
        glFogf(GL_FOG_END, 70.0f);
    } 
    else {
        glClearColor(0.015f, 0.020f, 0.040f, 1.0f); 
        glEnable(GL_FOG);
        GLfloat fogColor[4];
        fogColor[0] = 0.015f;
        fogColor[1] = 0.020f;
        fogColor[2] = 0.040f;
        fogColor[3] = 1.0f;
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_START, currentFogStart);
        glFogf(GL_FOG_END, currentFogEnd);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity();
    
    if (gameState == 0) {
        drawMenu();
    } 
    else if (gameState == 1) { 
        drawGame3D(); 
        drawHUD(); 
    } 
    else if (gameState == 2) {
        drawHelp();
    }
    
    glutSwapBuffers();
}

void Reshape(int w, int h) {
    if (h == 0 || w == 0) return;
    
    windowWidth = w; 
    windowHeight = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
    
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluPerspective(75.0, (GLfloat)w / (GLfloat)h, 0.1, 300.0); 
    
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 
}

void initEngine() {
    glEnable(GL_DEPTH_TEST); 
    setupLighting(); 
    loadBuildingTextures();
    
    srand((unsigned)time(NULL)); 
    
    for(int i = 0; i < 256; i++) {
        keys[i] = false;
    }
    
    prevChunk.active = false; 
    currChunk = generateLevelChunk(0.0f, -1.0f, -5.0f); 
    
    float nextStartX = currChunk.x[18]; 
    float nextStartY = currChunk.y[18]; 
    float nextStartZ = currChunk.z[18];
    nextChunk = generateLevelChunk(nextStartX, nextStartY, nextStartZ);
    
    initPlayer();
    initEnemy();
    initDice();
    initAudio();
    
    titleTexture = loadTexture("Graphics/sementara.png");
    sideTex[0] = loadTexture("Graphics/building1.png");
    topTex[0] = loadTexture("Graphics/top1.png");
    sideTex[1] = loadTexture("Graphics/building2.png");
    topTex[1] = loadTexture("Graphics/top2.png");
    sideTex[2] = loadTexture("Graphics/building3.png");
    topTex[2] = loadTexture("Graphics/top3.png");
}

void setupCallbacks() {
    glutDisplayFunc(display); 
    glutReshapeFunc(Reshape); 
    
    glutKeyboardFunc(keyboardDown); 
    glutKeyboardUpFunc(keyboardUp); 
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mouseMotion); 
    glutMotionFunc(mouseMotion); 
    
    glutTimerFunc(16, timer, 0); 
}

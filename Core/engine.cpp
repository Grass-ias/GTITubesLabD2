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
<<<<<<< HEAD
=======
#include "../Include/dice.h"

static int diceSpawnTimer = 0;
>>>>>>> 86274678d26223726222a12be05ee6ddd8e51b58

void timer(int value) {
    if (gameState == 1) {
        updatePlayer(); 
        
        if (introTimer > 0) {
            introTimer--;
            if (introTimer == 0) {
                chaseStarted = true;
                enemyZ = playerZ + 60.0f;
                currentFogStart = 15.0f;
                currentFogEnd = 70.0f;
<<<<<<< HEAD
=======

                spawnDiceFromSide();
                diceSpawnTimer = 120;
>>>>>>> 86274678d26223726222a12be05ee6ddd8e51b58
            }
        }
        
        if (chaseStarted) {
<<<<<<< HEAD
            updateEnemy(); 
=======
            updateEnemy();

            if (!dice.active) {
                if (diceSpawnTimer > 0) {
                    diceSpawnTimer--;
                } else {
                    spawnDiceFromSide();
                    diceSpawnTimer = 120 + (rand() % 80);
                }
            }

            updateDice();
>>>>>>> 86274678d26223726222a12be05ee6ddd8e51b58
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
        glClearColor(0.02f, 0.02f, 0.02f, 1.0f); 
        glEnable(GL_FOG);
        GLfloat fogColor[4];
        fogColor[0] = 0.02f;
        fogColor[1] = 0.02f;
        fogColor[2] = 0.02f;
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
<<<<<<< HEAD
    
=======
    loadBuildingTextures();
>>>>>>> 86274678d26223726222a12be05ee6ddd8e51b58
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
<<<<<<< HEAD
=======
    initDice();
>>>>>>> 86274678d26223726222a12be05ee6ddd8e51b58
    initAudio();
    
    titleTexture = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\sementara.png");
    sideTex[0] = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\building1.png");
    topTex[0] = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\top1.png");
    sideTex[1] = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\building2.png");
    topTex[1] = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\top2.png");
    sideTex[2] = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\building3.png");
    topTex[2] = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\top3.png");
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

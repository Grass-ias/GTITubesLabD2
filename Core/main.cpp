#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h> 

#include "globals.h"
#include "input.h"
#include "../Entity/player.h"
#include "../Entity/enemy.h"
#include "../Map/map.h"
#include "../Graphics/ui.h"

void timer(int value) {
    if (gameState == 1) {
        updatePlayer(); 
        
        if (introTimer > 0) {
            introTimer--;
            if (introTimer == 0) {
                chaseStarted = true;
                backEnemyZ = ballZ + 60.0f;
            }
        }
        
        if (chaseStarted) {
            updateEnemy(); 
        }
        if (!isTestMap) {
            float triggerZ = currChunk.z[18] + (currChunk.sz[18] / 2.0f); 
            
            if (ballZ <= triggerZ) {
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
        glClearColor(0.2f, 0.6f, 0.8f, 1.0f); 
        glDisable(GL_FOG);
    } 
    else {
        glClearColor(0.02f, 0.02f, 0.02f, 1.0f); 
        glEnable(GL_FOG);
        GLfloat fogColor[] = {0.02f, 0.02f, 0.02f, 1.0f};
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

void init() {
    glEnable(GL_DEPTH_TEST); 
    setupLighting(); 
    
    quadricEnemy = gluNewQuadric();
    gluQuadricDrawStyle(quadricEnemy, GLU_FILL);
    gluQuadricNormals(quadricEnemy, GLU_SMOOTH);
    gluQuadricTexture(quadricEnemy, GL_TRUE); 

    titleTexture = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\sementara.png");
    enemyTexture = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\monster.png");
}

int main(int argc, char **argv) {
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); /*Pacakge*/
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("YOU CAN'T RUN FROM IT"); 
    
    init(); 
    srand(time(NULL)); 
    
    for(int i = 0; i < 256; i++) {
        keys[i] = false;
    }
    
    prevChunk.active = false; 
    currChunk = generateLevelChunk(0.0f, -1.0f, -5.0f); 
    
    float nextStartX = currChunk.x[18]; 
    float nextStartY = currChunk.y[18]; 
    float nextStartZ = currChunk.z[18];
    nextChunk = generateLevelChunk(nextStartX, nextStartY, nextStartZ);
    
    ballX = currChunk.x[0]; 
    ballY = currChunk.y[0] + 0.25f; 
    ballZ = currChunk.z[0];
    respawnX = ballX; 
    respawnY = ballY; 
    respawnZ = ballZ;
    
    glutDisplayFunc(display); 
    glutReshapeFunc(Reshape); 
    
    glutKeyboardFunc(keyboardDown); 
    glutKeyboardUpFunc(keyboardUp); 
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mouseMotion); 
    glutMotionFunc(mouseMotion); 
    
    glutTimerFunc(16, timer, 0); 
    
    glutMainLoop(); 
    return 0;
}

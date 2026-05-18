#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h> 

#include "input.h"
#include "globals.h"
#include "../Entity/enemy.h"
#include "../Entity/player.h"
#include "../Map/map.h"
#include "../Graphics/ui.h"

void timer(int value) {
    if (gameState == 1) {
        updatePlayer(); 
        updateEntity(); 

        if (!isTestMap) {
            float cMinX = currChunk.x[18] - (currChunk.sx[18]/2);
            float cMaxX = currChunk.x[18] + (currChunk.sx[18]/2);
            float cMinZ = currChunk.z[18] - (currChunk.sz[18]/2);
            float cMaxZ = currChunk.z[18] + (currChunk.sz[18]/2);
            if (ballX >= cMinX && ballX <= cMaxX && ballZ >= cMinZ && ballZ <= cMaxZ) {
                if (isGrounded && ballY <= currChunk.y[18] + 0.5f) shiftChunks(); 
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); 
}

void display() {	
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
    glMatrixMode(GL_MODELVIEW); glLoadIdentity(); 
}

void init() {
    glClearColor(0.02f, 0.02f, 0.02f, 1.0f); 
    glEnable(GL_DEPTH_TEST); 
    
    glEnable(GL_FOG);
    GLfloat fogColor[] = {0.02f, 0.02f, 0.02f, 1.0f}; 
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR); 
    glFogf(GL_FOG_START, 15.0f);    
    glFogf(GL_FOG_END, 40.0f);      

    setupLighting(); 
    
    quadricEntity = gluNewQuadric();
    gluQuadricDrawStyle(quadricEntity, GLU_FILL);
    gluQuadricNormals(quadricEntity, GLU_SMOOTH); 

    titleTexture = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\sementara.png");
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("You Can't Run"); 
    
    init();
    srand(time(NULL));
    for(int i=0; i<256; i++) {
        keys[i] = false; 
    }
    
    prevChunk.active = false; currChunk = generateLevelChunk(0.0f, -1.0f, -5.0f); 
    float nextStartX = currChunk.x[18];
    float nextStartY = currChunk.y[18];
    float nextStartZ = currChunk.z[18];
    nextChunk = generateLevelChunk(nextStartX, nextStartY, nextStartZ);
    
    ballX = currChunk.x[0];
    ballY = currChunk.y[0] + 0.25f; ballZ = currChunk.z[0];
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
    
    glutMainLoop(); return 0;
}
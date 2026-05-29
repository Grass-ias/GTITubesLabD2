#include "../Include/input.h"
#include "../Include/globals.h"
#include <GL/glut.h>
#include <stdlib.h>

void mouseMotion(int x, int y) {
    if (gameState != 1) {
        return;
    }

    int centerX = windowWidth / 2; 
    int centerY = windowHeight / 2;
    if (x == centerX && y == centerY) {
        return;
    }

    float sensitivity = 0.003f;
    yaw += (x - centerX) * sensitivity; 
    pitch -= (y - centerY) * sensitivity;
    if (pitch > 1.5f) { 
        pitch = 1.5f;
    }
    if (pitch < -1.5f) {
        pitch = -1.5f;
    }
    glutWarpPointer(centerX, centerY);
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && gameState == 0) {
        y = windowHeight - y; 
        if (x >= 350 && x <= 550 && y >= 230 && y <= 270) {
            gameState = 1; glutSetCursor(GLUT_CURSOR_NONE); 
            glutWarpPointer(windowWidth/2, windowHeight/2);
        } 
        else if (x >= 350 && x <= 550 && y >= 180 && y <= 220) {
            gameState = 2;
        } 
        else if (x >= 350 && x <= 550 && y >= 130 && y <= 170) {
            exit(0);
        }
    }
}

void keyboardDown(unsigned char key, int x, int y) { 
    if (key >= 'A' && key <= 'Z') {
        key += 32; 
    }

    if (gameState == 0) {
        if (key == 13) { 
            gameState = 1; 
            glutSetCursor(GLUT_CURSOR_NONE); 
            glutWarpPointer(windowWidth/2, windowHeight/2); } 
        else if (key == 'h') 
            gameState = 2; 
        else if (key == 27) 
            exit(0);
    } else if (gameState == 1) {
        keys[key] = true; 
        if (key == '=') {
            isTestMap = !isTestMap; 
            if (isTestMap) { 
                playerX = 0.0f; 
                playerY = 5.0f; 
                playerZ = 0.0f; 
            }
            else { 
                playerX = respawnX; 
                playerY = respawnY + 2.0f; 
                playerZ = respawnZ; 
            }
            velX = 0; velZ = 0;
        }
        else if (key == 'b') { 
            gameState = 0; 
            glutSetCursor(GLUT_CURSOR_INHERIT); 
        }
        else if (key == 27) {
            exit(0);
        }
    } else if (gameState == 2) {
        if (key == 'b' || key == 27) {
            gameState = 0;
        }
    }
}

void keyboardUp(unsigned char key, int x, int y) { 
    if (key >= 'A' && key <= 'Z') {
        key += 32;
    }
    keys[key] = false;
}

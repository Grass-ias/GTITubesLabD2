#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h> 

#include "input.h"
#include "globals.h"
#include "../Map/map.h"
#include "../Graphics/ui.h"

void timer(int value) {
    if (gameState == 1) {
        float dirX = sin(yaw); 
        float dirZ = -cos(yaw);

        float rightX = cos(yaw); 
        float rightZ = sin(yaw);

        float inputX = 0.0f; 
        float inputZ = 0.0f;
        bool hasInput = false;

        if (keys['w']) { 
            inputX += dirX; inputZ += dirZ; hasInput = true; 
        }
        if (keys['s']) { 
            inputX -= dirX; inputZ -= dirZ; hasInput = true; 
        }
        if (keys['a']) { 
            inputX -= rightX; inputZ -= rightZ; hasInput = true; 
        }
        if (keys['d']) { 
            inputX += rightX; inputZ += rightZ; hasInput = true; 
        }

        float length = sqrt(inputX*inputX + inputZ*inputZ);
        if (length > 0.0f) { 
            inputX /= length; inputZ /= length; 
        }

        bool isBraking = false;
        if (hasInput) {
            float dotProduct = (inputX * velX) + (inputZ * velZ);
            if (dotProduct < -0.01f) isBraking = true;
        }

        isSprinting = keys['q'] && keys['w']; 
        bool tryingToCrouch = keys['c'];
        float currentSpeed = sqrt(velX*velX + velZ*velZ);

        float targetMaxSpeed = 0.15f; 
        if (isSprinting) {
            targetMaxSpeed = 0.25f;
        }

        bool canStand = !checkWallCollision(ballX, ballY, ballZ, 0.05f, 0.05f, 1.2f);

        if (tryingToCrouch) {
            if (isGrounded && !isSliding && currentSpeed > 0.10f && !isBraking) {
                isSliding = true; 
                isCrouching = true; 
                currentHeight = 0.6f; 
            }   
        } 
        else {
            if (canStand) {
                isCrouching = false; 
                isSliding = false; 
                currentHeight = 1.2f; 
            } 
            else {
                isCrouching = true; 
                currentHeight = 0.6f;
                if (isGrounded && currentSpeed > 0.10f && !isBraking) {
                    isSliding = true;
                }
                else {
                    isSliding = false;
                }
            }
        }

        if (isCrouching && !isSliding) {
            targetMaxSpeed = 0.06f; 
        }
        
        if (isSliding) {
            if (currentSpeed < 0.02f) { isSliding = false; velX = 0.0f; velZ = 0.0f; }
            else if (isBraking) { 
                isSliding = false; 
                velX = 0.0f;
                velZ = 0.0f; 
            } 
        }

        float wallCheckX = ballX + dirX * 0.3f;
        float wallCheckZ = ballZ + dirZ * 0.3f;
        bool hittingWallForward = checkWallCollision(wallCheckX, ballY, wallCheckZ, currentRadiusX, currentRadiusZ, currentHeight);
        bool headClear = !checkWallCollision(ballX, ballY, ballZ, 0.05f, 0.05f, 1.2f);

        if (!isGrounded && hittingWallForward && headClear) {
            bool isLedge = !checkWallCollision(wallCheckX, ballY + 1.0f, wallCheckZ, currentRadiusX, currentRadiusZ, currentHeight);

            if (isLedge) {
                if (isHanging) {
                    if (keys['w']) {
                        speedY = 0.30f; 
                        velX = dirX * 0.15f; 
                        velZ = dirZ * 0.15f;
                        isHanging = false; 
                        isClimbing = false;
                    }
                } 
                else {
                    if (keys['w']) {
                        speedY = 0.30f; 
                        velX = dirX * 0.15f; 
                        velZ = dirZ * 0.15f;
                        isClimbing = false;
                    } 
                    else if (isClimbing) { 
                        isHanging = true; 
                        isClimbing = false;
                    }
                }
            } 
            else {
                if (keys['w'] && climbTimer < MAX_CLIMB) {
                    isClimbing = true; 
                    isHanging = false; 
                    climbTimer++;
                } 
                else {
                    isClimbing = false; 
                    isHanging = false;
                }
            }
        } 
        else {
            isHanging = false; 
            isClimbing = false;
            if (isGrounded) {
                climbTimer = 0; 
            }
        }

        if (isHanging && (keys['s'] || keys['a'] || keys['d'] || keys['c'])) {
            isHanging = false;
        }

        float friction = 1.0f; 
        float accel = 0.0f; 
        float currentGravity = gravity;

        if (isHanging) {
            currentGravity = 0.0f; speedY = 0.0f; velX = 0.0f; velZ = 0.0f; 
        } 
        else if (isClimbing) {
            currentGravity = 0.0f; 
            speedY = 0.08f; 
            velX = 0.0f; 
            velZ = 0.0f; 
        } 
        else if (isSliding) {
            friction = 0.985f; 
            accel = 0.0f; 
            targetMaxSpeed = 1.0f; 
        } 
        else if (isGrounded) {
            if (hasInput) { 
                friction = 0.85f; 
                accel = targetMaxSpeed * (1.0f - friction); } 
            else { 
                friction = 0.40f; 
                accel = 0.0f; }
        } 
        else {
            friction = 0.99f;
            accel = hasInput ? 0.005f : 0.0f; 
        }

        float targetRadX = 0.3f; 
        float targetRadZ = 0.3f;
        if (isSliding && currentSpeed > 0.05f) {
            targetRadX = 0.3f + abs(velX/currentSpeed) * 0.6f; 
            targetRadZ = 0.3f + abs(velZ/currentSpeed) * 0.6f;
        }

        if (!checkWallCollision(ballX, ballY, ballZ, targetRadX, targetRadZ, currentHeight)) {
            currentRadiusX = targetRadX; currentRadiusZ = targetRadZ;
        } 
        else { 
            currentRadiusX = 0.3f; 
            currentRadiusZ = 0.3f; }

        velX += inputX * accel;
        velZ += inputZ * accel;
        velX *= friction;
        velZ *= friction;

        currentSpeed = sqrt(velX*velX + velZ*velZ);
        if (currentSpeed > targetMaxSpeed && !isSliding) {
            velX = (velX / currentSpeed) * targetMaxSpeed;
            velZ = (velZ / currentSpeed) * targetMaxSpeed;
        }

        if (!checkWallCollision(ballX + velX, ballY, ballZ, currentRadiusX, currentRadiusZ, currentHeight)) {
            ballX += velX;
        }
        else {
            velX = 0;
        }
        if (!checkWallCollision(ballX, ballY, ballZ + velZ, currentRadiusX, currentRadiusZ, currentHeight)) {
            ballZ += velZ;
        }
        else {
            velZ = 0;
        }

        if (keys[' '] && isGrounded && !isHanging && !isClimbing) { 
            speedY = isSliding ? 0.28f : 0.38f; 
            isGrounded = false; keys[' '] = false; 
        }

        float oldY = ballY;         
        speedY += currentGravity;          
        float nextY = ballY + speedY; 

        if (speedY > 0.0f && !isHanging && !isClimbing) {
            if (checkWallCollision(ballX, ballY + speedY, ballZ, 0.05f, 0.05f, currentHeight)) {
                speedY = 0.0f;
            }
        }

        float groundY = getGroundY(ballX, ballZ, oldY, currentRadiusX, currentRadiusZ);
        
        if (nextY <= groundY) {
            ballY = groundY; speedY = 0; isGrounded = true; 
            
            if (!isTestMap) {
                LevelChunk* chunksToCheck[2] = {&currChunk, &nextChunk};
                for (int c = 0; c < 2; c++) {
                    if (!chunksToCheck[c]->active) continue;
                    for (int i = 0; i < JUMLAH_PLATFORM; i++) {
                        if (chunksToCheck[c]->type[i] == 0) {
                            float minX = chunksToCheck[c]->x[i] - (chunksToCheck[c]->sx[i] / 2.0f); float maxX = chunksToCheck[c]->x[i] + (chunksToCheck[c]->sx[i] / 2.0f);
                            float minZ = chunksToCheck[c]->z[i] - (chunksToCheck[c]->sz[i] / 2.0f); float maxZ = chunksToCheck[c]->z[i] + (chunksToCheck[c]->sz[i] / 2.0f);
                            if (ballX >= minX && ballX <= maxX && ballZ >= minZ && ballZ <= maxZ) {
                                respawnX = chunksToCheck[c]->x[i]; respawnY = chunksToCheck[c]->y[i] + 0.25f; respawnZ = chunksToCheck[c]->z[i];
                            }
                        }
                    }
                }
            }
        } 
        else {
            ballY = nextY; isGrounded = false; 
        }
        
        if (ballY < (isTestMap ? -15.0f : -30.0f)) { 
            if (isTestMap) { ballX = 0; ballY = 5.0f; ballZ = 0; }
            else { ballX = respawnX; ballY = respawnY + 2.0f; ballZ = respawnZ; }
            velX = 0; velZ = 0; speedY = 0.0f; 
        }
        
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glLoadIdentity();
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
    glClearColor(0.2, 0.6, 0.8, 1.0); 
    glEnable(GL_DEPTH_TEST); 
    setupLighting(); 
    titleTexture = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\sementara.png");
}

int main(int argc, char **argv) {
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Apex Movement - Bug Fixed"); 
    
    init(); 
    srand(time(NULL)); 
    for(int i=0; i<256; i++) {
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


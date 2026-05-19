#include "player.h"
#include "../Core/globals.h"
#include "../Map/map.h" 
#include <math.h>

void updatePlayer() {
    float dirX = sin(yaw);
    float dirZ = -cos(yaw);
    
    float rightX = cos(yaw);
    float rightZ = sin(yaw);
    
    float inputX = 0.0f;
    float inputZ = 0.0f;
    
    bool hasInput = false;

    if (keys['w']) {
        inputX += dirX;
        inputZ += dirZ;
        hasInput = true;
    }
    if (keys['s']) {
        inputX -= dirX;
        inputZ -= dirZ;
        hasInput = true;
    }
    if (keys['a']) {
        inputX -= rightX;
        inputZ -= rightZ;
        hasInput = true;
    }
    if (keys['d']) {
        inputX += rightX;
        inputZ += rightZ;
        hasInput = true;
    }

    float length = sqrt(inputX*inputX + inputZ*inputZ);
    if (length > 0.0f) {
        inputX /= length;
        inputZ /= length;
    }

    bool isBraking = false;
    if (hasInput) {
        float dotProduct = (inputX * velX) + (inputZ * velZ);
        if (dotProduct < -0.01f) {
            isBraking = true;
        }
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
        if (currentSpeed < 0.02f) {
            isSliding = false;
            velX = 0.0f;
            velZ = 0.0f;
        }
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
                    isHanging = false; isClimbing = false;
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
                    isHanging = true; isClimbing = false;
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
        currentGravity = 0.0f;
        speedY = 0.0f;
        velX = 0.0f;
        velZ = 0.0f; 
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
            accel = targetMaxSpeed * (1.0f - friction);
        } 
        else {
            friction = 0.40f;
            accel = 0.0f;
        }
    } 
    else {
        friction = 0.99f;
        accel = hasInput ? 0.005f : 0.0f; 
    }

    float targetRadX = 0.3f;
    float targetRadZ = 0.3f;
    if (isSliding && currentSpeed > 0.05f) {
        targetRadX = 0.3f + fabs(velX/currentSpeed) * 0.6f;
        targetRadZ = 0.3f + fabs(velZ/currentSpeed) * 0.6f;
    }

    if (!checkWallCollision(ballX, ballY, ballZ, targetRadX, targetRadZ, currentHeight)) {
        currentRadiusX = targetRadX;
        currentRadiusZ = targetRadZ;
    } 
    else { 
        currentRadiusX = 0.3f;
        currentRadiusZ = 0.3f;
    }

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
        isGrounded = false;
        keys[' '] = false; 
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
        ballY = groundY;
        speedY = 0;
        isGrounded = true; 
        if (!isTestMap) {
            LevelChunk* chunksToCheck[2] = {&currChunk, &nextChunk};
            for (int c = 0; c < 2; c++) {
                if (!chunksToCheck[c]->active) {
                    continue;
                }
                for (int i = 0; i < JUMLAH_PLATFORM; i++) {
                    if (chunksToCheck[c]->type[i] == 0) {
                        float minX = chunksToCheck[c]->x[i] - (chunksToCheck[c]->sx[i] / 2.0f);
                        float maxX = chunksToCheck[c]->x[i] + (chunksToCheck[c]->sx[i] / 2.0f);
                        float minZ = chunksToCheck[c]->z[i] - (chunksToCheck[c]->sz[i] / 2.0f);
                        float maxZ = chunksToCheck[c]->z[i] + (chunksToCheck[c]->sz[i] / 2.0f);

                        if (ballX >= minX && ballX <= maxX && ballZ >= minZ && ballZ <= maxZ) {
                            respawnX = chunksToCheck[c]->x[i];
                            respawnY = chunksToCheck[c]->y[i] + 0.25f;
                            respawnZ = chunksToCheck[c]->z[i];

                            if (!chunksToCheck[c]->visited[i]) {
                                chunksToCheck[c]->visited[i] = true;
                            }                            
                        }
                    }
                }
            }
        }
    } 
    else {
        ballY = nextY; 
        isGrounded = false; 
    }

    if (ballY < (isTestMap ? -15.0f : -30.0f)) {
        if (isTestMap) {
            ballX = 0;
            ballY = 5.0f;
            ballZ = 0;

            velX = 0;
            velZ = 0;
            speedY = 0.0f;
        }
        else {
            if (!chaseStarted) {
                deathType = 1;
            }
            else {
                deathType = 2;
            }
        }
    }
}

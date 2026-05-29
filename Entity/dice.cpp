#include "../Include/dice.h"
#include "../Include/globals.h"
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

Dice dice;

static float randFloat(float minValue, float maxValue) {
    return minValue + ((float)rand() / (float)RAND_MAX) * (maxValue - minValue);
}

void initDice() {
    dice.x = 0.0f;
    dice.y = 0.0f;
    dice.z = 0.0f;
    dice.vx = 0.0f;
    dice.vy = 0.0f;
    dice.vz = 0.0f;
    dice.size = 3.8f;
    dice.rotX = 0.0f;
    dice.rotY = 0.0f;
    dice.rotZ = 0.0f;
    dice.active = false;
    dice.hitCooldown = 0;
}

void spawnDiceFromSide() {
    if (!chaseStarted || isTestMap || deathType > 0 || gameWon) {
        return;
    }

    float dirX = sin(yaw);
    float dirZ = -cos(yaw);
    float rightX = cos(yaw);
    float rightZ = sin(yaw);

    // Big dice, slower movement, and close enough to be visible.
    float frontDist = randFloat(12.0f, 18.0f);
    float sideDist = randFloat(10.0f, 15.0f);
    int side = (rand() % 2 == 0) ? -1 : 1;

    float centerX = playerX + dirX * frontDist;
    float centerZ = playerZ + dirZ * frontDist;

    dice.x = centerX + rightX * sideDist * side;
    dice.y = playerY + 1.8f;
    dice.z = centerZ + rightZ * sideDist * side;

    float speed = randFloat(0.075f, 0.115f);
    dice.vx = -rightX * side * speed;
    dice.vz = -rightZ * side * speed;
    dice.vy = 0.0f;

    dice.size = randFloat(3.2f, 4.3f);
    dice.rotX = randFloat(0.0f, 360.0f);
    dice.rotY = randFloat(0.0f, 360.0f);
    dice.rotZ = randFloat(0.0f, 360.0f);
    dice.active = true;
    dice.hitCooldown = 0;
}

static bool diceHitPlayer() {
    float dx = dice.x - playerX;
    float dz = dice.z - playerZ;
    float horizontalDist = sqrt(dx * dx + dz * dz);

    float hitRadius = dice.size * 0.55f + currentRadiusX + 0.55f;

    float playerBottom = playerY;
    float playerTop = playerY + currentHeight;
    float diceBottom = dice.y - dice.size * 0.5f;
    float diceTop = dice.y + dice.size * 0.5f;
    bool yOverlap = (diceTop >= playerBottom && diceBottom <= playerTop);

    return horizontalDist <= hitRadius && yOverlap;
}

static void knockPlayerByDice() {
    velX += dice.vx * 5.5f;
    velZ += dice.vz * 5.5f;
    speedY = 0.20f;
    isGrounded = false;
    isSliding = false;
    isCrouching = false;
    currentHeight = 1.2f;
}

void updateDice() {
    if (!dice.active) {
        return;
    }

    dice.x += dice.vx;
    dice.y += dice.vy;
    dice.z += dice.vz;

    float moveSpeed = sqrt(dice.vx * dice.vx + dice.vz * dice.vz);
    dice.rotX += moveSpeed * 170.0f;
    dice.rotY += moveSpeed * 90.0f;
    dice.rotZ += moveSpeed * 140.0f;

    if (dice.hitCooldown > 0) {
        dice.hitCooldown--;
    }

    if (dice.hitCooldown == 0 && diceHitPlayer()) {
        knockPlayerByDice();
        dice.hitCooldown = 80;
        dice.active = false;
        return;
    }

    float dx = dice.x - playerX;
    float dz = dice.z - playerZ;
    float dist = sqrt(dx * dx + dz * dz);
    if (dist > 36.0f || dice.y < -30.0f) {
        dice.active = false;
    }
}

static void drawPip(float x, float y, float z, float r) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(r, 16, 16);
    glPopMatrix();
}

static void drawDicePips(float s) {
    float h = s / 2.0f;
    float r = s * 0.075f;
    float o = s * 0.23f;
    float eps = 0.035f;

    glColor3f(0.0f, 0.0f, 0.0f);

    // FRONT = 1
    drawPip(0.0f, 0.0f, h + eps, r);

    // BACK = 6
    drawPip(-o,  o, -h - eps, r);
    drawPip(-o,  0, -h - eps, r);
    drawPip(-o, -o, -h - eps, r);
    drawPip( o,  o, -h - eps, r);
    drawPip( o,  0, -h - eps, r);
    drawPip( o, -o, -h - eps, r);

    // RIGHT = 2
    drawPip(h + eps,  o,  o, r);
    drawPip(h + eps, -o, -o, r);

    // LEFT = 5
    drawPip(-h - eps,  0,   0, r);
    drawPip(-h - eps,  o,   o, r);
    drawPip(-h - eps,  o,  -o, r);
    drawPip(-h - eps, -o,   o, r);
    drawPip(-h - eps, -o,  -o, r);

    // TOP = 3
    drawPip( 0, h + eps,  0, r);
    drawPip(-o, h + eps, -o, r);
    drawPip( o, h + eps,  o, r);

    // BOTTOM = 4
    drawPip(-o, -h - eps, -o, r);
    drawPip(-o, -h - eps,  o, r);
    drawPip( o, -h - eps, -o, r);
    drawPip( o, -h - eps,  o, r);
}

void drawDice() {
    if (!dice.active) {
        return;
    }

    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(dice.x, dice.y, dice.z);
    glRotatef(dice.rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(dice.rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(dice.rotZ, 0.0f, 0.0f, 1.0f);

    glColor3f(0.97f, 0.97f, 0.92f);
    glutSolidCube(dice.size);

    glColor3f(0.0f, 0.0f, 0.0f);
    glutWireCube(dice.size + 0.035f);

    drawDicePips(dice.size);

    glPopMatrix();
    glPopAttrib();
}

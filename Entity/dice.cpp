#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#include "../Include/dice.h"
#include "../Include/globals.h"

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

    dice.size = 1.55f;

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

    // DEKET PLAYER:
    // dadu muncul di depan player sedikit, bukan jauh di ujung map.
    float frontDist = randFloat(6.0f, 10.0f);
    float sideDist = randFloat(6.0f, 8.5f);

    int side = (rand() % 2 == 0) ? -1 : 1;

    float centerX = playerX + dirX * frontDist;
    float centerZ = playerZ + dirZ * frontDist;

    dice.x = centerX + rightX * sideDist * side;
    dice.y = playerY + 0.9f;
    dice.z = centerZ + rightZ * sideDist * side;

    float speed = randFloat(0.26f, 0.34f);

    // gerak dari kanan ke kiri / kiri ke kanan
    dice.vx = -rightX * side * speed;
    dice.vz = -rightZ * side * speed;
    dice.vy = 0.0f;

    dice.size = randFloat(1.45f, 1.75f);

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

    float hitRadius = dice.size * 0.85f + currentRadiusX + 0.45f;

    float playerBottom = playerY;
    float playerTop = playerY + currentHeight;

    float diceBottom = dice.y - dice.size * 0.5f;
    float diceTop = dice.y + dice.size * 0.5f;

    bool yOverlap = (diceTop >= playerBottom && diceBottom <= playerTop);

    return horizontalDist <= hitRadius && yOverlap;
}

static void knockPlayerByDice() {
    // Bikin player kedorong/jatoh, bukan langsung game over.
    velX = dice.vx * 4.2f;
    velZ = dice.vz * 4.2f;

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

    // Rolling effect.
    dice.rotX += moveSpeed * 300.0f;
    dice.rotY += moveSpeed * 120.0f;
    dice.rotZ += moveSpeed * 220.0f;

    if (dice.hitCooldown > 0) {
        dice.hitCooldown--;
    }

    if (dice.hitCooldown == 0 && diceHitPlayer()) {
        knockPlayerByDice();
        dice.hitCooldown = 60;
        dice.active = false;
        return;
    }

    float dx = dice.x - playerX;
    float dz = dice.z - playerZ;
    float dist = sqrt(dx * dx + dz * dz);

    // kalau sudah kelewat jauh, hilang lalu nanti respawn lagi dari timer engine.
    if (dist > 26.0f || dice.y < -20.0f) {
        dice.active = false;
    }
}

// =======================
// DICE VISUAL
// =======================

static void drawPip(float x, float y, float z, float r) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(r, 14, 14);
    glPopMatrix();
}

static void drawDicePips(float s) {
    float h = s / 2.0f;
    float r = s * 0.10f;
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

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);

    glPushMatrix();

    glTranslatef(dice.x, dice.y, dice.z);

    glRotatef(dice.rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(dice.rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(dice.rotZ, 0.0f, 0.0f, 1.0f);

    // badan dadu putih agak creamy
    glColor3f(0.96f, 0.96f, 0.92f);
    glutSolidCube(dice.size);

    // outline hitam
    glColor3f(0.0f, 0.0f, 0.0f);
    glutWireCube(dice.size + 0.025f);

    // titik-titik dadu
    drawDicePips(dice.size);

    glPopMatrix();
    glPopAttrib();
}

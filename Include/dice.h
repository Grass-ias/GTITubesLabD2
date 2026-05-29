#ifndef DICE_H
#define DICE_H

struct Dice {
    float x, y, z;
    float vx, vy, vz;
    float size;
    float rotX, rotY, rotZ;
    bool active;
    int hitCooldown;
};

extern Dice dice;

void initDice();
void spawnDiceFromSide();
void updateDice();
void drawDice();

#endif

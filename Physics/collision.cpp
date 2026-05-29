#include "../Include/collision.h"
#include "../Include/globals.h"

bool checkWallCollision(float testX, float testY, float testZ, float rX, float rZ, float h) {
    if (isTestMap) {
        float minX = -2.0f;
        float maxX = 2.0f;
        float minZ = -15.0f;
        float maxZ = -10.0f;
        float platTop = 3.0f;
        float platBottom = 0.8f; 
        if (testX + rX > minX && testX - rX < maxX && testZ + rZ > minZ && testZ - rZ < maxZ) {
            if (testY < platTop - 0.05f && testY + h > platBottom) {
                return true;
            }
        }
        minX = -5.0f;
        maxX = -3.0f;
        minZ = -10.0f;
        maxZ = -8.0f;
        platTop = 1.0f;
        platBottom = -1.0f;
        if (testX + rX > minX && testX - rX < maxX && testZ + rZ > minZ && testZ - rZ < maxZ) {
            if (testY < platTop - 0.05f && testY + h > platBottom) {
                return true; 
            }
        }
        minX = 3.0f;
        maxX = 6.0f;
        minZ = -12.0f;
        maxZ = -9.0f;
        platTop = 6.0f;
        platBottom = -50.0f; 
        if (testX + rX > minX && testX - rX < maxX && testZ + rZ > minZ && testZ - rZ < maxZ) {
            if (testY < platTop - 0.05f && testY + h > platBottom) {
                return true;
            }
        }
        return false;
    }
    LevelChunk* chunks[3];
    chunks[0] = &prevChunk;
    chunks[1] = &currChunk;
    chunks[2] = &nextChunk;
    for (int c = 0; c < 3; c++) {
        if (!chunks[c]->active) {
            continue; 
        }
        for (int i = 0; i < JUMLAH_PLATFORM; i++) {
            float minX = chunks[c]->x[i] - (chunks[c]->sx[i] / 2.0f);
            float maxX = chunks[c]->x[i] + (chunks[c]->sx[i] / 2.0f);
            float minZ = chunks[c]->z[i] - (chunks[c]->sz[i] / 2.0f);
            float maxZ = chunks[c]->z[i] + (chunks[c]->sz[i] / 2.0f);
            float platTop = chunks[c]->y[i] + 0.25f; 
            float platBottom = chunks[c]->y[i] - chunks[c]->sy[i] + 0.25f; 
            if (testX + rX > minX && testX - rX < maxX && testZ + rZ > minZ && testZ - rZ < maxZ) {
                if (testY < platTop - 0.05f && testY + h > platBottom) {
                    return true; 
                }
            }
        }
    }
    return false; 
}

float getGroundY(float testX, float testZ, float oldY, float rX, float rZ) {
    if (isTestMap) {
        float abyss = -10.0f;
        float highestGround = abyss;
        if (testX > -50.0f && testX < 50.0f && testZ > -50.0f && testZ < 50.0f) {
            highestGround = 0.0f; 
        }
        if (testX > -2.0f && testX < 2.0f && testZ > -15.0f && testZ < -10.0f) { 
            if (oldY >= 3.0f - 0.1f) {
                highestGround = 3.0f;
            }
        }
        if (testX > -5.0f && testX < -3.0f && testZ > -10.0f && testZ < -8.0f) {
            if (oldY >= 1.0f - 0.1f) { 
                highestGround = 1.0f;
            }
        }
        if (testX > 3.0f && testX < 6.0f && testZ > -12.0f && testZ < -9.0f) {
            if (oldY >= 6.0f - 0.1f) {
                highestGround = 6.0f;
            }
        }
        return highestGround;
    }
    float abyss = -100.0f;
    float highestGround = abyss;
    LevelChunk* chunks[3];
    chunks[0] = &prevChunk;
    chunks[1] = &currChunk;
    chunks[2] = &nextChunk;
    for (int c = 0; c < 3; c++) {
        if (!chunks[c]->active) continue;
        for (int i = 0; i < JUMLAH_PLATFORM; i++) {
            float minX = chunks[c]->x[i] - (chunks[c]->sx[i] / 2.0f);
            float maxX = chunks[c]->x[i] + (chunks[c]->sx[i] / 2.0f);
            float minZ = chunks[c]->z[i] - (chunks[c]->sz[i] / 2.0f);
            float maxZ = chunks[c]->z[i] + (chunks[c]->sz[i] / 2.0f);
            float platTop = chunks[c]->y[i] + 0.25f;
            if (testX + rX > minX && testX - rX < maxX && testZ + rZ > minZ && testZ - rZ < maxZ) {
                if (oldY >= platTop - 0.1f) {
                    if (platTop > highestGround) {
                        highestGround = platTop;
                    }
                }
            }
        }
    }
    return highestGround; 
}

bool checkClimbableWall(float testX, float testY, float testZ, float rX, float rZ, float h) {
    if (isTestMap) return false;
    LevelChunk* chunks[3];
    chunks[0] = &prevChunk;
    chunks[1] = &currChunk;
    chunks[2] = &nextChunk;
    for (int c = 0; c < 3; c++) {
        if (!chunks[c]->active) continue;
        for (int i = 0; i < JUMLAH_PLATFORM; i++) {
            float minX = chunks[c]->x[i] - (chunks[c]->sx[i] / 2.0f);
            float maxX = chunks[c]->x[i] + (chunks[c]->sx[i] / 2.0f);
            float minZ = chunks[c]->z[i] - (chunks[c]->sz[i] / 2.0f);
            float maxZ = chunks[c]->z[i] + (chunks[c]->sz[i] / 2.0f);
            float platTop = chunks[c]->y[i] + 0.25f;
            float platBottom = chunks[c]->y[i] - chunks[c]->sy[i] + 0.25f;
            if (testX + rX > minX && testX - rX < maxX && testZ + rZ > minZ && testZ - rZ < maxZ) {
                if (testY < platTop - 0.05f && testY + h > platBottom) {
                    return true;
                }
            }
        }
    }
    return false;
}

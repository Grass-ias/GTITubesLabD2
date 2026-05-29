#include "../Include/map.h"
#include <stdlib.h>
#include <math.h>

LevelChunk generateLevelChunk(float startX, float startY, float startZ) {
    LevelChunk chunk; 
    chunk.active = true;
    float currentX = startX;
    float currentY = startY;
    float currentZ = startZ;
    chunk.visited[0] = false;
    chunk.type[0] = 0;
    chunk.x[0] = startX;
    chunk.y[0] = startY;
    chunk.z[0] = startZ;
    chunk.sx[0] = 4.0f; 
    chunk.sy[0] = 20.0f;
    chunk.sz[0] = 4.0f;
    chunk.hasPipe[0] = false;
    chunk.color[0] = 1.0f;
    chunk.platTexIdx[0] = rand() % 3;
    int consecutiveStones = 0;
    for (int i = 1; i < JUMLAH_PLATFORM; i++) {
        chunk.platTexIdx[i] = rand() % 3;
        chunk.visited[i] = false;
        float dx = 0.0f;
        float dy = 0.0f;
        float gapZ = 0.0f;
        if (i == 6 || i == 12 || i == 18) {
            chunk.type[i] = 0;
            chunk.sx[i] = 4.0f;
            chunk.sy[i] = 20.0f;
            chunk.sz[i] = 4.0f; 
            chunk.hasPipe[i] = false;
            chunk.color[i] = 1.0f;
            consecutiveStones = 0;
            gapZ = 3.0f; 
            dx = ((rand() % 200) / 100.0f) - 1.0f;
            dy = ((rand() % 100) / 100.0f) - 0.5f; 
        } 
        else {
            chunk.type[i] = 1; 
            chunk.sx[i] = 4.0f; 
            chunk.sy[i] = ((rand() % 1500) / 100.0f) + 15.0f;
            chunk.hasPipe[i] = false;
            chunk.color[i] = ((rand() % 61) / 100.0f) + 0.3f;
            if (consecutiveStones > 0) {
                chunk.sz[i] = 3.0f;
                gapZ = 1.5f; 
                consecutiveStones--;
            } 
            else {
                int r = rand() % 2;
                if (r == 0) {
                    chunk.sz[i] = 15.0f;
                    gapZ = ((rand() % 601) / 100.0f) + 6.0f;
                } 
                else {
                    chunk.sz[i] = 3.0f;
                    gapZ = 1.5f; 
                    consecutiveStones = 2; 
                }
            }
            dy = ((rand() % 100) / 100.0f) - 0.3f; 
            float maxOffsetX = 2.0f;
            dx = ((rand() % (int)(maxOffsetX * 200)) / 100.0f) - maxOffsetX; 
        }
        float dist = sqrt(dx * dx + dy * dy + gapZ * gapZ);
        if (dist < 6.0f) {
            dist = 6.0f;
        }
        if (dist > 12.0f) {
            dist = 12.0f;
        }
        gapZ = sqrt(dist * dist - dx * dx - dy * dy);
        float prevScaleZ = chunk.sz[i-1];
        float halfSizeZ = chunk.sz[i] / 2.0f;
        chunk.x[i] = chunk.x[i-1] + dx;
        chunk.y[i] = chunk.y[i-1] + dy;
        chunk.z[i] = chunk.z[i-1] - (prevScaleZ / 2.0f) - gapZ - halfSizeZ;
        currentX = chunk.x[i];
        currentY = chunk.y[i];
        currentZ = chunk.z[i];
    }
    float leftZ = startZ;
    float rightZ = startZ;
    for (int bgIdx = 0; bgIdx < 40; bgIdx++) {
        chunk.bgTexIdx[bgIdx] = rand() % 3;
        chunk.bgWidth[bgIdx] = ((rand() % 2000) / 100.0f) + 15.0f;
        chunk.bgHeight[bgIdx] = 800.0f + ((rand() % 20000) / 100.0f);
        chunk.bgY[bgIdx] = -400.0f;
        float gap = -((rand() % 800) / 100.0f) - 2.0f;
        if (bgIdx < 20) {
            chunk.bgX[bgIdx] = -38.0f - (chunk.bgWidth[bgIdx] / 2.0f) - ((rand() % 500) / 100.0f);
            chunk.bgZ[bgIdx] = leftZ - (chunk.bgWidth[bgIdx] / 2.0f);
            leftZ -= (chunk.bgWidth[bgIdx] + gap);
        } else {
            chunk.bgX[bgIdx] = 38.0f + (chunk.bgWidth[bgIdx] / 2.0f) + ((rand() % 500) / 100.0f);
            chunk.bgZ[bgIdx] = rightZ - (chunk.bgWidth[bgIdx] / 2.0f);
            rightZ -= (chunk.bgWidth[bgIdx] + gap);
        }
    }
    return chunk; 
}

void shiftChunks() {
    prevChunk = currChunk; 
    currChunk = nextChunk;
    float nextStartX = currChunk.x[18];
    float nextStartY = currChunk.y[18];
    float nextStartZ = currChunk.z[18];
    nextChunk = generateLevelChunk(nextStartX, nextStartY, nextStartZ);
}

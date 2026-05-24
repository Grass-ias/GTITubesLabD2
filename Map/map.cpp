#include "../Include/map.h"
#include <stdlib.h>

LevelChunk generateLevelChunk(float startX, float startY, float startZ) {
    LevelChunk chunk; chunk.active = true;
    float currentX = startX, currentY = startY, currentZ = startZ; 
    
    // Starting platform initialization
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

    int consecutiveStones = 0;

    for (int i = 1; i < JUMLAH_PLATFORM; i++) {
        chunk.visited[i] = false;

        if (i == 6 || i == 12 || i == 18) {
            // Checkpoint Platform (Standard large, safe size)
            chunk.type[i] = 0;
            chunk.sx[i] = 4.0f;
            chunk.sy[i] = 20.0f;
            chunk.sz[i] = 4.0f; 
            chunk.hasPipe[i] = false;
            chunk.color[i] = 1.0f;
            consecutiveStones = 0; // Reset consecutive pattern tracking

            // Calculate currentZ by subtracting exactly half of previous, a fixed safe gap, and half of current
            float prevScaleZ = chunk.sz[i-1];
            float gapZ = 3.0f; 
            chunk.z[i] = chunk.z[i-1] - (prevScaleZ / 2.0f) - gapZ - (chunk.sz[i] / 2.0f);

            // Checkpoints remain relatively centered
            chunk.x[i] = chunk.x[i-1] + ((rand() % 200) / 100.0f) - 1.0f;
            chunk.y[i] = chunk.y[i-1] + ((rand() % 100) / 100.0f) - 0.5f; 
        } 
        else {
            // Regular Platform with Pattern A/B chunk pattern generator
            chunk.type[i] = 1; 
            chunk.sx[i] = 4.0f; // Fixed standard width
            chunk.sy[i] = ((rand() % 1500) / 100.0f) + 15.0f;
            chunk.hasPipe[i] = false;
            chunk.color[i] = ((rand() % 61) / 100.0f) + 0.3f;

            float gapZ = 0.0f;

            if (consecutiveStones > 0) {
                // Pattern B: Stepping Stones (continuation)
                chunk.sz[i] = 3.0f;
                gapZ = 1.5f; // Small gapZ jumps
                consecutiveStones--;
            } 
            else {
                // Determine new pattern type
                int r = rand() % 2;
                if (r == 0) {
                    // Pattern A: The Bridge (1 very long platform)
                    chunk.sz[i] = 15.0f;
                    gapZ = ((rand() % 601) / 100.0f) + 6.0f;
                } 
                else {
                    // Pattern B: Stepping Stones (3 consecutive short platforms)
                    chunk.sz[i] = 3.0f;
                    gapZ = 1.5f; // Small gapZ jumps
                    consecutiveStones = 2; // Sets the next 2 platforms to be stepping stones too
                }
            }

            float prevScaleZ = chunk.sz[i-1];
            float halfSizeZ = chunk.sz[i] / 2.0f;

            // Safety Bubble: Ensure gapZ + halfSizeZ >= 5.0f for Pattern A (non-stepping stones)
            if (consecutiveStones == 0 && gapZ + halfSizeZ < 5.0f) {
                gapZ = 5.0f - halfSizeZ;
            }
            if (gapZ < 1.2f) {
                gapZ = 1.2f;
            }

            chunk.z[i] = chunk.z[i-1] - (prevScaleZ / 2.0f) - gapZ - halfSizeZ;
            chunk.y[i] = chunk.y[i-1] + ((rand() % 100) / 100.0f) - 0.3f; 

            float maxOffsetX = 2.0f;
            chunk.x[i] = chunk.x[i-1] + ((rand() % (int)(maxOffsetX * 200)) / 100.0f) - maxOffsetX; 
        }
    }

    // Background scenery buildings generation
    float totalLength = startZ - chunk.z[JUMLAH_PLATFORM - 1];
    for (int bgIdx = 0; bgIdx < 10; bgIdx++) {
        if (bgIdx < 5) {
            // Left side: X between -25.0f and -50.0f
            chunk.bgX[bgIdx] = -(((rand() % 2500) / 100.0f) + 25.0f);
        } else {
            // Right side: X between 25.0f and 50.0f
            chunk.bgX[bgIdx] = ((rand() % 2500) / 100.0f) + 25.0f;
        }
        chunk.bgZ[bgIdx] = startZ - (((rand() % 1000) / 1000.0f) * totalLength);
        chunk.bgWidth[bgIdx] = ((rand() % 1201) / 100.0f) + 8.0f;
        chunk.bgHeight[bgIdx] = ((rand() % 5001) / 100.0f) + 50.0f;
    }

    return chunk; 
}

void shiftChunks() {
    prevChunk = currChunk; currChunk = nextChunk;
    float nextStartX = currChunk.x[18];
    float nextStartY = currChunk.y[18];
    float nextStartZ = currChunk.z[18];
    nextChunk = generateLevelChunk(nextStartX, nextStartY, nextStartZ);
}

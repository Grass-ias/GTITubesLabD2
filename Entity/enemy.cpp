#include "enemy.h"
#include "../Core/globals.h"

void updateEnemy() {
    if (!isTestMap) {
        enemyZ -= enemySpeed; 

        if (ballY < -30.0f || ballZ > enemyZ) { 
            ballX = respawnX;
            ballY = respawnY + 2.0f;
            ballZ = respawnZ; 

            velX = 0;
            velZ = 0;
            speedY = 0.0f; 
            enemyZ = respawnZ + 15.0f; 
        }
    }
    else {
        if (ballY < -15.0f) { 
            ballX = 0;
            ballY = 5.0f;
            ballZ = 0;

            velX = 0;
            velZ = 0;
            speedY = 0.0f; 
        }
    }
}
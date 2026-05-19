#include "enemy.h"
#include "../Core/globals.h"

void updateEnemy() {
    if (gameWon || deathType > 0) {
        return;
    }

    if (!isTestMap) {
        if (chaseStarted) {
            enemyZ -= enemySpeed; 
        }

        if (chaseStarted && ballZ > enemyZ - 24.5f) {
            deathType = 3;
        }

        float distanceToEnemy = enemyZ - ballZ;
        
        if (chaseStarted && !frontEnemyActive && distanceToEnemy > 150.0f && checkpointsPassed >= 5) {
            frontEnemyActive = true;
            frontEnemyZ = ballZ - 100.0f; 
        }

        if (frontEnemyActive) {
            frontEnemyZ += enemySpeed; 

            if (ballZ < frontEnemyZ + 24.5f) {
                if (keys['w']) {
                    gameWon = true;
                } 
                else {
                    deathType = 4;
                }
            }
        }
    }
}
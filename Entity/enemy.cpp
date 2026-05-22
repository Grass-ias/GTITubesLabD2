#include "enemy.h"
#include "../Core/globals.h"

void updateEnemy() {
    if (gameWon || deathType > 0) {
        return;
    }

    if (!isTestMap) {
        if (chaseStarted) {
            backEnemyZ -= backEnemySpeed; 
        }

        if (chaseStarted && ballZ > backEnemyZ - 24.5f) {
            if (frontEnemyActive) {
                gameWon = true;
            }
            else {
                deathType = 3;
            }
        }

        float distanceToEnemy = backEnemyZ - ballZ;
        
        if (chaseStarted && !frontEnemyActive && distanceToEnemy > 150.0f && checkpointsPassed >= 5) {
            frontEnemyActive = true;
            frontEnemyZ = ballZ - 100.0f; 
        }
        
        if (frontEnemyActive) {
            frontEnemyZ += backEnemySpeed; 

            if (ballZ < frontEnemyZ + 24.5f) {
                deathType = 4;
            }
        }
    }
}

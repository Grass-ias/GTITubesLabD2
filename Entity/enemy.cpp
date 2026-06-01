#include "../Include/enemy.h"
#include "../Include/globals.h"
#include "../Include/texture.h"
#include <math.h>

void updateEnemy() {
    if (gameWon || deathType > 0) {
        return;
    }

    if (!isTestMap) {
        if (chaseStarted) {
            enemyZ -= backEnemySpeed; 
        }

        if (chaseStarted && playerZ > enemyZ - 24.5f) {
            float dirZ = -cos(yaw);
            if (frontEnemyActive || (keys['w'] && dirZ > 0.0f)) {
                gameWon = true;
            }
            else {
                deathType = 3;
            }
        }

        float distanceToEnemy = enemyZ - playerZ;
        
        if (chaseStarted && !frontEnemyActive && distanceToEnemy > 150.0f && checkpointsPassed >= 5) {
            frontEnemyActive = true;
            frontEnemyZ = playerZ - 100.0f; 
        }
        
        if (frontEnemyActive) {
            frontEnemyZ += backEnemySpeed; 

            if (playerZ < frontEnemyZ + 24.5f) {
                deathType = 4;
            }
        }

        if (playerY - enemyY > 5.0f) {
            enemyYTrackingTimer += 0.016f;
        } else {
            enemyYTrackingTimer = 0.0f;
        }

        if (enemyYTrackingTimer > 1.0f) {
            enemyY += (playerY - enemyY) * 0.02f;
        } else {
            if (enemyY > 5.0f) {
                enemyY += (5.0f - enemyY) * 0.01f;
            }
        }
    }
}

void initEnemy() {
    quadricEnemy = gluNewQuadric();
    gluQuadricDrawStyle(quadricEnemy, GLU_FILL);
    gluQuadricNormals(quadricEnemy, GLU_SMOOTH);
    gluQuadricTexture(quadricEnemy, GL_TRUE); 
    enemyTexture = loadTexture("C:\\Users\\TaiBalap\\Documents\\GitHub\\GTITubesLabD2\\Graphics\\monster.png");
}

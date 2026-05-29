#include "../Include/display.h"
#include "../Include/globals.h"
#include "../Include/texture.h"
#include "../Include/lighting.h"
#include "../Include/dice.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void setupBuildingMaterial(float brightness) {
    float mat_ambient[4];
    float mat_diffuse[4];
    float mat_specular[4];
    float mat_emission[4];
    float mat_shininess[1];
    float ambVal = brightness;
    if (ambVal < 0.20f) {
        ambVal = 0.20f;
    }
    mat_ambient[0] = ambVal;
    mat_ambient[1] = ambVal;
    mat_ambient[2] = ambVal;
    mat_ambient[3] = 1.0f;
    mat_diffuse[0] = brightness;
    mat_diffuse[1] = brightness;
    mat_diffuse[2] = brightness;
    mat_diffuse[3] = 1.0f;
    mat_specular[0] = 0.8f;
    mat_specular[1] = 0.8f;
    mat_specular[2] = 0.8f;
    mat_specular[3] = 1.0f;
    mat_emission[0] = 0.0f;
    mat_emission[1] = 0.0f;
    mat_emission[2] = 0.0f;
    mat_emission[3] = 1.0f;
    mat_shininess[0] = 50.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}

void resetBuildingMaterial() {
    float reset_emission[4];
    reset_emission[0] = 0.0f;
    reset_emission[1] = 0.0f;
    reset_emission[2] = 0.0f;
    reset_emission[3] = 1.0f;
    glMaterialfv(GL_FRONT, GL_EMISSION, reset_emission);
}


static bool findPlatformTopAt(float x, float z, float oldY, float *outY) {
    float bestY = -1000.0f;
    bool found = false;
    LevelChunk* chunks[3];
    chunks[0] = &prevChunk;
    chunks[1] = &currChunk;
    chunks[2] = &nextChunk;

    for (int c = 0; c < 3; c++) {
        if (!chunks[c]->active) continue;
        for (int i = 0; i < JUMLAH_PLATFORM; i++) {
            float minX = chunks[c]->x[i] - chunks[c]->sx[i] / 2.0f;
            float maxX = chunks[c]->x[i] + chunks[c]->sx[i] / 2.0f;
            float minZ = chunks[c]->z[i] - chunks[c]->sz[i] / 2.0f;
            float maxZ = chunks[c]->z[i] + chunks[c]->sz[i] / 2.0f;
            float topY = chunks[c]->y[i] + 0.25f;

            if (x >= minX && x <= maxX && z >= minZ && z <= maxZ && oldY >= topY - 2.0f) {
                if (!found || topY > bestY) {
                    bestY = topY;
                    found = true;
                }
            }
        }
    }

    if (found) {
        *outY = bestY;
    }
    return found;
}

static void drawFlatShadow(float x, float y, float z, float radiusX, float radiusZ, float alpha) {
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glColor4f(0.0f, 0.0f, 0.0f, alpha);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(x, y + 0.035f, z);
        for (int i = 0; i <= 40; i++) {
            float a = 2.0f * PI * (float)i / 40.0f;
            glVertex3f(x + cos(a) * radiusX, y + 0.035f, z + sin(a) * radiusZ);
        }
    glEnd();

    glDepthMask(GL_TRUE);
    glPopAttrib();
}

static void drawEntityShadows() {
    float shadowY;

    // Shadow pemain: bikin arah cahaya kebaca meskipun player first-person tidak kelihatan.
    if (findPlatformTopAt(playerX, playerZ, playerY + currentHeight, &shadowY)) {
        float alpha = chaseStarted ? 0.42f : 0.30f;
        drawFlatShadow(playerX + 0.35f, shadowY, playerZ + 0.35f, currentRadiusX * 2.4f, currentRadiusZ * 2.0f, alpha);
    }

    // Shadow enemy belakang saat chase.
    if (chaseStarted && findPlatformTopAt(playerX, enemyZ, enemyY + 2.0f, &shadowY)) {
        drawFlatShadow(playerX, shadowY, enemyZ, 4.8f, 3.6f, 0.50f);
    }

    // Shadow front enemy kalau aktif.
    if (frontEnemyActive && findPlatformTopAt(playerX, frontEnemyZ, 8.0f, &shadowY)) {
        drawFlatShadow(playerX, shadowY, frontEnemyZ, 4.4f, 3.3f, 0.45f);
    }
}

static void drawSkyLightObject() {
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glDisable(GL_DEPTH_TEST);

    float skyX = playerX + 24.0f;
    float skyY = playerY + 44.0f;
    float skyZ = playerZ - 75.0f;

    glPushMatrix();
    glTranslatef(skyX, skyY, skyZ);

    if (!chaseStarted) {
        // Matahari: solid + aura sederhana.
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.88f, 0.15f, 0.18f);
        glutSolidSphere(10.0f, 24, 24);
        glDisable(GL_BLEND);

        glColor3f(1.0f, 0.93f, 0.0f);
        glutSolidSphere(6.0f, 32, 32);
    } else {
        // Bulan sabit: sphere terang ditutup sphere sewarna langit malam.
        glColor3f(0.78f, 0.82f, 1.0f);
        glutSolidSphere(5.2f, 32, 32);
        glTranslatef(2.0f, 0.25f, 0.0f);
        glColor3f(0.02f, 0.02f, 0.04f);
        glutSolidSphere(5.2f, 32, 32);
    }

    glPopMatrix();
    glPopAttrib();
}

void drawTexturedBuilding(float sizeX, float sizeY, float sizeZ, GLuint texSide, GLuint texTop) {
    float hx = sizeX / 2.0f;
    float hy = sizeY / 2.0f;
    float hz = sizeZ / 2.0f;

    // TEXTURE DIBIKIN GEDE:
    // 0.12 = zoom in. Kalau masih kecil, turunin ke 0.08.
    float repeatX = 0.65f;
	float repeatY = 0.85f;
	float repeatZ = 0.65f;

    // =======================
    // SISI GEDUNG: PAKAI TEXTURE
    // =======================
    if (texSide != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texSide);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.45f, 0.45f, 0.45f);
    }

    glBegin(GL_QUADS);

    // DEPAN
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);              glVertex3f(-hx, -hy,  hz);
    glTexCoord2f(repeatX, 0.0f);           glVertex3f( hx, -hy,  hz);
    glTexCoord2f(repeatX, repeatY);        glVertex3f( hx,  hy,  hz);
    glTexCoord2f(0.0f, repeatY);           glVertex3f(-hx,  hy,  hz);

    // BELAKANG
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);              glVertex3f( hx, -hy, -hz);
    glTexCoord2f(repeatX, 0.0f);           glVertex3f(-hx, -hy, -hz);
    glTexCoord2f(repeatX, repeatY);        glVertex3f(-hx,  hy, -hz);
    glTexCoord2f(0.0f, repeatY);           glVertex3f( hx,  hy, -hz);

    // KANAN
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);              glVertex3f( hx, -hy,  hz);
    glTexCoord2f(repeatZ, 0.0f);           glVertex3f( hx, -hy, -hz);
    glTexCoord2f(repeatZ, repeatY);        glVertex3f( hx,  hy, -hz);
    glTexCoord2f(0.0f, repeatY);           glVertex3f( hx,  hy,  hz);

    // KIRI
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);              glVertex3f(-hx, -hy, -hz);
    glTexCoord2f(repeatZ, 0.0f);           glVertex3f(-hx, -hy,  hz);
    glTexCoord2f(repeatZ, repeatY);        glVertex3f(-hx,  hy,  hz);
    glTexCoord2f(0.0f, repeatY);           glVertex3f(-hx,  hy, -hz);

    glEnd();

    // =======================
    // ATAS GEDUNG: POLOS, BIAR TIDAK STATIC
    // =======================
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.42f, 0.40f, 0.32f);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-hx, hy,  hz);
    glVertex3f( hx, hy,  hz);
    glVertex3f( hx, hy, -hz);
    glVertex3f(-hx, hy, -hz);
    glEnd();

    // =======================
    // BAWAH GEDUNG: GELAP
    // =======================
    glColor3f(0.08f, 0.08f, 0.07f);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-hx, -hy, -hz);
    glVertex3f( hx, -hy, -hz);
    glVertex3f( hx, -hy,  hz);
    glVertex3f(-hx, -hy,  hz);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
}

GLuint getBuildingTexture(int index) {
    if (index % 3 == 0) return textureBuilding1;
    if (index % 3 == 1) return textureBuilding2;
    return textureBuilding3;
}

void drawGame3D() {
    if (deathType > 0 || gameWon) {
        return;
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    float eyeX = playerX; 
    float eyeY = playerY + currentHeight; 
    float eyeZ = playerZ;
    float targetX = eyeX + (cos(pitch) * sin(yaw)); 
    float targetY = eyeY + sin(pitch); 
    float targetZ = eyeZ - (cos(pitch) * cos(yaw));
    float distanceToIT = fabs(playerZ - enemyZ);
    if ((introTimer > 0) || (chaseStarted && distanceToIT < 25.0f && !isTestMap)) {
        float shakeIntensity = (introTimer > 0) ? 0.04f : (25.0f - distanceToIT) * 0.002f; 
        float randomOffsetX = ((rand() % 100) / 100.0f - 0.5f) * shakeIntensity;
        float randomOffsetY = ((rand() % 100) / 100.0f - 0.5f) * shakeIntensity;
        targetX += randomOffsetX;
        targetY += randomOffsetY;
    }
    gluLookAt(eyeX, eyeY, eyeZ, targetX, targetY, targetZ, 0.0f, 1.0f, 0.0f);
    
    updateLighting();
    
    glFogi(GL_FOG_MODE, GL_LINEAR);
    if (!chaseStarted) {
        glFogf(GL_FOG_START, 15.0f);
        glFogf(GL_FOG_END, 70.0f);
        float fogColorArray[4];
        fogColorArray[0] = 0.1f;
        fogColorArray[1] = 0.15f;
        fogColorArray[2] = 0.18f;
        fogColorArray[3] = 1.0f;
        glFogfv(GL_FOG_COLOR, fogColorArray);
    } else {
        glFogf(GL_FOG_START, currentFogStart);
        glFogf(GL_FOG_END, currentFogEnd);
        float fogColorArray[4];
        fogColorArray[0] = 0.02f;
        fogColorArray[1] = 0.02f;
        fogColorArray[2] = 0.02f;
        fogColorArray[3] = 1.0f;
        glFogfv(GL_FOG_COLOR, fogColorArray);
    }
    
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    if (!chaseStarted) {
        glColor3f(0.1f, 0.15f, 0.18f);
    } else {
        glColor3f(0.02f, 0.02f, 0.02f);
    }
    glBegin(GL_QUADS);
    glVertex3f(-1000.0f, -60.0f, 1000.0f);
    glVertex3f(1000.0f, -60.0f, 1000.0f);
    glVertex3f(1000.0f, -60.0f, -1000.0f);
    glVertex3f(-1000.0f, -60.0f, -1000.0f);
    glEnd();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    
    drawSkyLightObject();
    glDisable(GL_FOG);
    if (isTestMap) {
        glPushMatrix();
        glTranslatef(0, -0.5f, 0);
        glScalef(100.0, 1.0, 100.0);
        setupBuildingMaterial(0.8f);
        glutSolidCube(1.0);
        resetBuildingMaterial();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0, 1.9f, -12.5f);
        glScalef(4.0, 2.2, 5.0);
        float red_diffuse[4];
        red_diffuse[0] = 1.0f; red_diffuse[1] = 0.2f; red_diffuse[2] = 0.2f; red_diffuse[3] = 1.0f;
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);
        float test_specular[4];
        test_specular[0] = 1.0f; test_specular[1] = 1.0f; test_specular[2] = 1.0f; test_specular[3] = 1.0f;
        float test_shininess[1];
        test_shininess[0] = 100.0f;
        glMaterialfv(GL_FRONT, GL_SPECULAR, test_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, test_shininess);
        glutSolidCube(1.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-4.0f, 0.0f, -9.0f);
        glScalef(2.0, 2.0, 2.0);
        float blue_diffuse[4];
        blue_diffuse[0] = 0.2f; blue_diffuse[1] = 0.2f; blue_diffuse[2] = 1.0f; blue_diffuse[3] = 1.0f;
        glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, test_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, test_shininess);
        glutSolidCube(1.0);
        glPopMatrix();
        glPushMatrix(); 
        glTranslatef(4.5f, 3.0f, -10.5f);
        glScalef(3.0, 6.0, 3.0);
        float yellow_diffuse[4];
        yellow_diffuse[0] = 0.8f; yellow_diffuse[1] = 0.8f; yellow_diffuse[2] = 0.2f; yellow_diffuse[3] = 1.0f;
        glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, test_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, test_shininess);
        glutSolidCube(1.0);
        glPopMatrix();
        return; 
    }
    if (chaseStarted) {
        glPushMatrix();
        glTranslatef(playerX, enemyY, enemyZ); 
        GLfloat enemySpecular[4];
        enemySpecular[0] = 1.0f; enemySpecular[1] = 0.0f; enemySpecular[2] = 0.0f; enemySpecular[3] = 1.0f;
        GLfloat enemyShininess[1];
        enemyShininess[0] = 50.0f;
        glMaterialfv(GL_FRONT, GL_SPECULAR, enemySpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, enemyShininess);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D); 
        glBindTexture(GL_TEXTURE_2D, enemyTexture);
        glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
        if (quadricEnemy) {
            gluSphere(quadricEnemy, 25.0f, 32, 32); 
        }
        glDisable(GL_TEXTURE_2D);
        glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
        if (quadricEnemy) {
            gluSphere(quadricEnemy, 26.0f, 16, 16); 
        }
        glDisable(GL_BLEND);
        GLfloat defaultSpecular[4];
        defaultSpecular[0] = 0.0f; defaultSpecular[1] = 0.0f; defaultSpecular[2] = 0.0f; defaultSpecular[3] = 1.0f;
        GLfloat defaultShininess[1];
        defaultShininess[0] = 0.0f;
        glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, defaultShininess);
        glPopMatrix();
    }
    if (frontEnemyActive) {
        glPushMatrix();
        glTranslatef(playerX, 5.0f, frontEnemyZ);
        GLfloat enemySpecular[4];
        enemySpecular[0] = 1.0f; enemySpecular[1] = 0.0f; enemySpecular[2] = 0.0f; enemySpecular[3] = 1.0f;
        GLfloat enemyShininess[1];
        enemyShininess[0] = 50.0f;
        glMaterialfv(GL_FRONT, GL_SPECULAR, enemySpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, enemyShininess);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.1f, 0.0f, 0.0f, 0.9f);
        if (quadricEnemy) {
            gluSphere(quadricEnemy, 25.0f, 32, 32);
        }
        glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
        if (quadricEnemy) {
            gluSphere(quadricEnemy, 26.0f, 16, 16);
        }
        glDisable(GL_BLEND);
        GLfloat defaultSpecular[4];
        defaultSpecular[0] = 0.0f; defaultSpecular[1] = 0.0f; defaultSpecular[2] = 0.0f; defaultSpecular[3] = 1.0f;
        GLfloat defaultShininess[1];
        defaultShininess[0] = 0.0f;
        glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, defaultShininess);
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_FOG);
    glFogf(GL_FOG_START, 80.0f);
    glFogf(GL_FOG_END, 250.0f);
    glBindTexture(GL_TEXTURE_2D, sideTex[0]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    float mat_ambient[4];
    mat_ambient[0] = 0.6f;
    mat_ambient[1] = 0.6f;
    mat_ambient[2] = 0.6f;
    mat_ambient[3] = 1.0f;
    float mat_diffuse[4];
    mat_diffuse[0] = 0.6f;
    mat_diffuse[1] = 0.6f;
    mat_diffuse[2] = 0.6f;
    mat_diffuse[3] = 1.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    LevelChunk* chunks[3];
    chunks[0] = &prevChunk;
    chunks[1] = &currChunk;
    chunks[2] = &nextChunk;
    for (int c = 0; c < 3; c++) {
        if (!chunks[c]->active) {
            continue;
        }
        for (int i = 0; i < 40; i++) {
		    glPushMatrix();
		
		    glTranslatef(chunks[c]->bgX[i], chunks[c]->bgY[i], chunks[c]->bgZ[i]);
		
		    glEnable(GL_TEXTURE_2D);
		    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
		    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		
		    GLuint buildingTex = getBuildingTexture(i + c * 40);
		
		    drawTexturedBuilding(
		        chunks[c]->bgWidth[i],
		        chunks[c]->bgHeight[i],
		        chunks[c]->bgWidth[i],
		        buildingTex,
		        0
		    );
		
		    glPopMatrix();
		}
    }
    glFogf(GL_FOG_START, chaseStarted ? currentFogStart : 15.0f);
    glFogf(GL_FOG_END, chaseStarted ? currentFogEnd : 70.0f);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    if (!chaseStarted) {
        glFogf(GL_FOG_START, 15.0f);
        glFogf(GL_FOG_END, 70.0f);
        float fogColorArray[4];
        fogColorArray[0] = 0.1f;
        fogColorArray[1] = 0.15f;
        fogColorArray[2] = 0.18f;
        fogColorArray[3] = 1.0f;
        glFogfv(GL_FOG_COLOR, fogColorArray);
    } else {
        glFogf(GL_FOG_START, currentFogStart);
        glFogf(GL_FOG_END, currentFogEnd);
        float fogColorArray[4];
        fogColorArray[0] = 0.02f;
        fogColorArray[1] = 0.02f;
        fogColorArray[2] = 0.02f;
        fogColorArray[3] = 1.0f;
        glFogfv(GL_FOG_COLOR, fogColorArray);
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int c = 0; c < 3; c++) {
        if (!chunks[c]->active) {
            continue;
        }
        for (int i = 0; i < JUMLAH_PLATFORM; i++) {
            if (c == 2 && i == 0) {
                continue; 
            }
            if (c == 0 && i == 18) {
                continue;
            }
            glPushMatrix();
            glTranslatef(chunks[c]->x[i], chunks[c]->y[i] - chunks[c]->sy[i] / 2.0f + 0.25f, chunks[c]->z[i]); 
            float col = chunks[c]->color[i];
            glEnable(GL_TEXTURE_2D);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            setupBuildingMaterial(col);
            drawTexturedBuilding(chunks[c]->sx[i], chunks[c]->sy[i], chunks[c]->sz[i], sideTex[chunks[c]->platTexIdx[i]], topTex[chunks[c]->platTexIdx[i]]);
            resetBuildingMaterial();
            glPopMatrix();
        }
    }
    // Dadu obstacle saat chase.
    drawDice();

    // Shadow sederhana yang stabil: blob shadow di atas platform.
    drawEntityShadows();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_FOG);
}

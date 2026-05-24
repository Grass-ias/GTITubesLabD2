#include "../Include/display.h"
#include "../Include/globals.h"
#include "../Include/texture.h"
#include "../Include/lighting.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void drawImage(GLuint tex, float x, float y, float w, float h) {
    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, tex); 
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS); 
    glTexCoord2f(0, 1);
    glVertex2f(x, y); 
    glTexCoord2f(1, 1);
    glVertex2f(x + w, y); 
    glTexCoord2f(1, 0);
    glVertex2f(x + w, y + h); 
    glTexCoord2f(0, 0);
    glVertex2f(x, y + h); 
    glEnd();
    
    glDisable(GL_TEXTURE_2D); 
    glDisable(GL_BLEND);
}

void drawTexturedBuilding(float sizeX, float sizeY, float sizeZ) {
    float hx = sizeX / 2.0f;
    float hy = sizeY / 2.0f;
    float hz = sizeZ / 2.0f;
    float texScale = 2.0f;

    glBegin(GL_QUADS);
    
    // Front Face (normal: 0, 0, 1)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);                          glVertex3f(-hx, -hy, hz);
    glTexCoord2f(sizeX / texScale, 0.0f);              glVertex3f(hx, -hy, hz);
    glTexCoord2f(sizeX / texScale, sizeY / texScale);  glVertex3f(hx, hy, hz);
    glTexCoord2f(0.0f, sizeY / texScale);              glVertex3f(-hx, hy, hz);

    // Back Face (normal: 0, 0, -1)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(sizeX / texScale, 0.0f);              glVertex3f(hx, -hy, -hz);
    glTexCoord2f(0.0f, 0.0f);                          glVertex3f(-hx, -hy, -hz);
    glTexCoord2f(0.0f, sizeY / texScale);              glVertex3f(-hx, hy, -hz);
    glTexCoord2f(sizeX / texScale, sizeY / texScale);  glVertex3f(hx, hy, -hz);

    // Top Face (normal: 0, 1, 0)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);                          glVertex3f(-hx, hy, hz);
    glTexCoord2f(sizeX / texScale, 0.0f);              glVertex3f(hx, hy, hz);
    glTexCoord2f(sizeX / texScale, sizeZ / texScale);  glVertex3f(hx, hy, -hz);
    glTexCoord2f(0.0f, sizeZ / texScale);              glVertex3f(-hx, hy, -hz);

    // Bottom Face (normal: 0, -1, 0)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);                          glVertex3f(-hx, -hy, -hz);
    glTexCoord2f(sizeX / texScale, 0.0f);              glVertex3f(hx, -hy, -hz);
    glTexCoord2f(sizeX / texScale, sizeZ / texScale);  glVertex3f(hx, -hy, hz);
    glTexCoord2f(0.0f, sizeZ / texScale);              glVertex3f(-hx, -hy, hz);

    // Right Face (normal: 1, 0, 0)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);                          glVertex3f(hx, -hy, hz);
    glTexCoord2f(sizeZ / texScale, 0.0f);              glVertex3f(hx, -hy, -hz);
    glTexCoord2f(sizeZ / texScale, sizeY / texScale);  glVertex3f(hx, hy, -hz);
    glTexCoord2f(0.0f, sizeY / texScale);              glVertex3f(hx, hy, hz);

    // Left Face (normal: -1, 0, 0)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);                          glVertex3f(-hx, -hy, -hz);
    glTexCoord2f(sizeZ / texScale, 0.0f);              glVertex3f(-hx, -hy, hz);
    glTexCoord2f(sizeZ / texScale, sizeY / texScale);  glVertex3f(-hx, hy, hz);
    glTexCoord2f(0.0f, sizeY / texScale);              glVertex3f(-hx, hy, -hz);

    glEnd();
}

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
}

void begin2D() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void end2D() {
    glMatrixMode(GL_MODELVIEW); 
    glPopMatrix(); 
    glMatrixMode(GL_PROJECTION); 
    glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW); 
    glEnable(GL_DEPTH_TEST); 
    if (chaseStarted) {
        glEnable(GL_FOG);
    }
}

void drawMenu() {
    begin2D(); 
    drawImage(titleTexture, 230, 330, 440, 220); 
    glColor3f(1, 1, 1);
    drawText(400, 250, "START"); 
    drawText(405, 200, "HELP"); 
    drawText(407, 150, "QUIT");
    drawText(275, 80, "ENTER = Start | H = Help | ESC = Quit"); 
    end2D();
}

void drawHelp() {
    begin2D(); 
    glColor3f(1, 1, 1); 
    drawText(420, 520, "Key");
    drawText(220, 430, "WASD  = Move | Q = Sprint");
    drawText(220, 390, "MOUSE = Look | C = Crouch/Slide");
    drawText(220, 350, "SPACE = Jump | = = Map Test");
    drawText(220, 310, "B     = Back to Menu");
    drawText(220, 270, "ESC   = Back / Quit");
    drawText(210, 140, "Press B or ESC to return to menu");
    end2D();
}

void drawHUD() {
    begin2D(); 
    glColor3f(1, 1, 1);

    if (deathType > 0) {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(windowWidth, 0);
        glVertex2f(windowWidth, windowHeight);
        glVertex2f(0, windowHeight);
        glEnd();

        glColor3f(1.0f, 0.0f, 0.0f);
        
        if (deathType == 1) {
            drawText(windowWidth / 2 - 120, windowHeight / 2 + 20, "YOU FELL INTO THE ABYSS.");
        } 
        else if (deathType == 2) {
            drawText(windowWidth / 2 - 140, windowHeight / 2 + 20, "THE VOID OFFERS NO ESCAPE.");
        } 
        else if (deathType == 3) {
            drawText(windowWidth / 2 - 130, windowHeight / 2 + 20, "YOU WERE CONSUMED BY 'IT'.");
        } 
        else if (deathType == 4) {
            drawText(windowWidth / 2 - 80, windowHeight / 2 + 20, "YOU HESITATED.");
        }

        drawText(windowWidth / 2 - 70, windowHeight / 2 - 20, "PRESS ESC TO QUIT.");
        end2D();
        return; 
    }

    if (gameWon) {
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(windowWidth, 0);
        glVertex2f(windowWidth, windowHeight);
        glVertex2f(0, windowHeight);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        drawText(windowWidth / 2 - 120, windowHeight / 2 + 20, "YOU FACED YOUR FEARS.");
        drawText(windowWidth / 2 - 40, windowHeight / 2 - 20, "YOU WIN.");
        end2D();
        return; 
    }

    if (introTimer > 0) {
        if ((introTimer / 10) % 2 == 0) {
            glColor3f(1.0f, 0.0f, 0.0f);
            drawText(windowWidth / 2 - 140, windowHeight / 2 + 50, "YOU CAN'T RUN FROM 'IT'");
        }
    }

    float currentSpeed = sqrt(velX*velX + velZ*velZ) * 100.0f; 
    char debugText[100]; 
    
    if (chaseStarted) {
        float distanceToIT = fabs(playerZ - enemyZ);
        sprintf(debugText, "Speed: %.0f | Distance to IT: %.1f m", currentSpeed, distanceToIT);
        if (distanceToIT < 15.0f && !isTestMap) glColor3f(1.0f, 0.2f, 0.2f);
    } 
    else {
        sprintf(debugText, "Speed: %.0f | Map: %s", currentSpeed, isTestMap ? "TEST ROOM" : "INFINITE MAP");
    }
    
    drawText(20, windowHeight - 30, debugText);
    glColor3f(1, 1, 1);
    
    char stateText[50];
    if (isHanging) sprintf(stateText, "STATE: Peeking (W = Vault, S/C = Drop)");
    else if (isClimbing) {
        sprintf(stateText, "STATE: Wall Climbing!");
    }
    else if (isSliding) {
        sprintf(stateText, "STATE: SLIDING!!!");
    }
    else if (isCrouching) {
        sprintf(stateText, "STATE: Crouching");
    }
    else if (isSprinting) {
        sprintf(stateText, "STATE: Sprinting");
    }
    else {
        sprintf(stateText, "STATE: Walking");
    }

    drawText(20, windowHeight - 60, stateText);

    drawText(20, 20, "Q=Sprint | C=Slide | '='=Map");
    drawText(windowWidth/2 - 5, windowHeight/2 - 5, "+"); 
    end2D();
}

void drawGame3D() {
    if (deathType > 0 || gameWon) {
        return;
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float eyeX = playerX; float eyeY = playerY + currentHeight; float eyeZ = playerZ;
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

    // Draw Abyss Plane
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    if (!chaseStarted) {
        glColor3f(0.8f, 0.9f, 1.0f);
    } else {
        glColor3f(0.02f, 0.02f, 0.02f);
    }
    glBegin(GL_QUADS);
    glVertex3f(-1000.0f, -40.0f, 1000.0f);
    glVertex3f(1000.0f, -40.0f, 1000.0f);
    glVertex3f(1000.0f, -40.0f, -1000.0f);
    glVertex3f(-1000.0f, -40.0f, -1000.0f);
    glEnd();
    glEnable(GL_LIGHTING);
    
    glDisable(GL_FOG);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    glTranslatef(playerX, playerY + 80.0f, playerZ - 150.0f);
    glDisable(GL_LIGHTING);
    if (!chaseStarted) {
        glColor3f(1.0f, 1.0f, 0.9f);
        GLUquadricObj* sunQuad = gluNewQuadric();
        gluSphere(sunQuad, 8.0f, 32, 32);
        gluDeleteQuadric(sunQuad);
    } else {
        glColor3f(0.0f, 0.0f, 0.0f);
        GLUquadricObj* innerQuad = gluNewQuadric();
        gluSphere(innerQuad, 8.0f, 32, 32);
        gluDeleteQuadric(innerQuad);

        glColor3f(1.0f, 0.4f, 0.0f);
        glutSolidTorus(2.0, 9.0, 16, 32);
    }
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FOG);

    if (isTestMap) {
        glPushMatrix();
        glTranslatef(0, -0.5f, 0);
        glScalef(100.0, 1.0, 100.0);
        glColor3f(0.8f, 0.8f, 0.8f);
        glutSolidCube(1.0);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0, 1.9f, -12.5f);
        glScalef(4.0, 2.2, 5.0);
        glColor3f(1.0f, 0.2f, 0.2f);
        glutSolidCube(1.0);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-4.0f, 0.0f, -9.0f);
        glScalef(2.0, 2.0, 2.0);
        glColor3f(0.2f, 0.2f, 1.0f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix(); 
        glTranslatef(4.5f, 3.0f, -10.5f);
        glScalef(3.0, 6.0, 3.0);
        glColor3f(0.8f, 0.8f, 0.2f);
        glutSolidCube(1.0);
        glPopMatrix();
        return; 
    }

    if (chaseStarted) {
        glPushMatrix();
        glTranslatef(playerX, enemyY, enemyZ); 
        
        GLfloat enemySpecular[] = {1.0f, 0.0f, 0.0f, 1.0f}; 
        GLfloat enemyShininess[] = {50.0f};
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
        
        GLfloat defaultSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat defaultShininess[] = {0.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, defaultShininess);
        glPopMatrix();
    }

    if (frontEnemyActive) {
        glPushMatrix();
        glTranslatef(playerX, 5.0f, frontEnemyZ);
        
        GLfloat enemySpecular[] = {1.0f, 0.0f, 0.0f, 1.0f}; 
        GLfloat enemyShininess[] = {50.0f};
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
        
        GLfloat defaultSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat defaultShininess[] = {0.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, defaultShininess);
        glPopMatrix();
    }

    glDisable(GL_COLOR_MATERIAL);

    // Draw Background Scenery Buildings
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bgTexture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    LevelChunk* chunks[3] = {&prevChunk, &currChunk, &nextChunk};
    for (int c = 0; c < 3; c++) {
        if (!chunks[c]->active) {
            continue;
        }
        for (int i = 0; i < 10; i++) {
            glPushMatrix();
            glTranslatef(chunks[c]->bgX[i], -20.0f, chunks[c]->bgZ[i]);
            
            GLfloat bg_ambient[4];
            GLfloat bg_diffuse[4];
            GLfloat bg_specular[4];
            GLfloat bg_emission[4];
            GLfloat reset_emission[4];
            GLfloat bg_shininess[1];
            
            bg_ambient[0] = 0.05f; bg_ambient[1] = 0.05f; bg_ambient[2] = 0.05f; bg_ambient[3] = 1.0f;
            bg_diffuse[0] = 0.1f;  bg_diffuse[1] = 0.1f;  bg_diffuse[2] = 0.1f;  bg_diffuse[3] = 1.0f;
            bg_specular[0] = 0.05f; bg_specular[1] = 0.05f; bg_specular[2] = 0.05f; bg_specular[3] = 1.0f;
            bg_emission[0] = 0.08f; bg_emission[1] = 0.08f; bg_emission[2] = 0.08f; bg_emission[3] = 1.0f;
            reset_emission[0] = 0.0f; reset_emission[1] = 0.0f; reset_emission[2] = 0.0f; reset_emission[3] = 1.0f;
            bg_shininess[0] = 10.0f;
            
            glMaterialfv(GL_FRONT, GL_AMBIENT, bg_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, bg_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, bg_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, bg_shininess);
            glMaterialfv(GL_FRONT, GL_EMISSION, bg_emission);
            
            drawTexturedBuilding(chunks[c]->bgWidth[i], chunks[c]->bgHeight[i], chunks[c]->bgWidth[i]);
            
            glMaterialfv(GL_FRONT, GL_EMISSION, reset_emission);
            glPopMatrix();
        }
    }

    // Draw Foreground Platforms
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
            
            GLfloat mat_ambient[4];
            GLfloat mat_diffuse[4];
            GLfloat mat_specular[4];
            GLfloat mat_emission[4];
            GLfloat reset_emission[4];
            GLfloat high_shininess[1];
            
            float col = chunks[c]->color[i];
            
            mat_ambient[0] = col * 0.2f;
            mat_ambient[1] = col * 0.2f;
            mat_ambient[2] = col * 0.2f;
            mat_ambient[3] = 1.0f;
            
            mat_diffuse[0] = col;
            mat_diffuse[1] = col;
            mat_diffuse[2] = col;
            mat_diffuse[3] = 1.0f;
            
            mat_specular[0] = 0.3f;
            mat_specular[1] = 0.3f;
            mat_specular[2] = 0.3f;
            mat_specular[3] = 1.0f;
            
            mat_emission[0] = 0.15f;
            mat_emission[1] = 0.15f;
            mat_emission[2] = 0.15f;
            mat_emission[3] = 1.0f;
            
            reset_emission[0] = 0.0f;
            reset_emission[1] = 0.0f;
            reset_emission[2] = 0.0f;
            reset_emission[3] = 1.0f;
            
            high_shininess[0] = 50.0f;
            
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, buildingTexture);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
            glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
            
            drawTexturedBuilding(chunks[c]->sx[i], chunks[c]->sy[i], chunks[c]->sz[i]);
            
            glMaterialfv(GL_FRONT, GL_EMISSION, reset_emission);
            
            float topY = chunks[c]->sy[i] / 2.0f + 0.01f;
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_LIGHTING);
            glColor3f(0.0f, 1.0f, 1.0f); // Bright neon cyan
            glBegin(GL_LINE_LOOP);
            glVertex3f(-chunks[c]->sx[i] / 2.0f, topY, -chunks[c]->sz[i] / 2.0f);
            glVertex3f(chunks[c]->sx[i] / 2.0f, topY, -chunks[c]->sz[i] / 2.0f);
            glVertex3f(chunks[c]->sx[i] / 2.0f, topY, chunks[c]->sz[i] / 2.0f);
            glVertex3f(-chunks[c]->sx[i] / 2.0f, topY, chunks[c]->sz[i] / 2.0f);
            glEnd();
            glEnable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);
            
            glPopMatrix();
        }
    }
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
}

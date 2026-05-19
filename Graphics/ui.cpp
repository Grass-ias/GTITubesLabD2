#include "ui.h"
#include "../Core/globals.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint loadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) { 
        printf("Gagal load image: %s\n", filename); 
        return 0; 
    }
    GLuint texture; 
    glGenTextures(1, &texture); 
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data); 
    return texture;
}

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
        float distanceToIT = fabs(ballZ - enemyZ);
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

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat lightPos[] = {0.0f, 20.0f, 0.0f, 1.0f}; 
    GLfloat lightAmbient[] = {0.1f, 0.1f, 0.2f, 1.0f};
    GLfloat lightDiffuse[] = {0.5f, 0.5f, 0.6f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

void drawGame3D() {
    if (deathType > 0 || gameWon) {
        return;
    }

    float eyeX = ballX; float eyeY = ballY + currentHeight; float eyeZ = ballZ;
    float targetX = eyeX + (cos(pitch) * sin(yaw)); 
    float targetY = eyeY + sin(pitch); 
    float targetZ = eyeZ - (cos(pitch) * cos(yaw));

    float distanceToIT = fabs(ballZ - enemyZ);
    if ((introTimer > 0) || (chaseStarted && distanceToIT < 25.0f && !isTestMap)) {
        float shakeIntensity = (introTimer > 0) ? 0.04f : (25.0f - distanceToIT) * 0.002f; 
        
        float randomOffsetX = ((rand() % 100) / 100.0f - 0.5f) * shakeIntensity;
        float randomOffsetY = ((rand() % 100) / 100.0f - 0.5f) * shakeIntensity;
        targetX += randomOffsetX;
        targetY += randomOffsetY;
    }

    gluLookAt(eyeX, eyeY, eyeZ, targetX, targetY, targetZ, 0.0f, 1.0f, 0.0f);

    if (isTestMap) {
        glPushMatrix();
        glTranslatef(0, -0.5f, 0);
        glScalef(100.0, 1.0, 100.0);
        glColor3f(0.8f, 0.8f, 0.8f);
        glutSolidCube(1.0);
        glColor3f(0,0,0);
        glutWireCube(1.0);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0, 1.9f, -12.5f);
        glScalef(4.0, 2.2, 5.0);
        glColor3f(1.0f, 0.2f, 0.2f);
        glutSolidCube(1.0);
        glColor3f(0,0,0);
        glutWireCube(1.0);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-4.0f, 0.0f, -9.0f);
        glScalef(2.0, 2.0, 2.0);
        glColor3f(0.2f, 0.2f, 1.0f);
        glutSolidCube(1.0);
        glColor3f(0,0,0);
        glutWireCube(1.0); 
        glPopMatrix();

        glPushMatrix(); 
        glTranslatef(4.5f, 3.0f, -10.5f);
        glScalef(3.0, 6.0, 3.0);
        glColor3f(0.8f, 0.8f, 0.2f);
        glutSolidCube(1.0);
        glColor3f(0,0,0);
        glutWireCube(1.0);
        glPopMatrix();
        return; 
    }

    if (chaseStarted) {
        glPushMatrix();
        glTranslatef(ballX, 5.0f, enemyZ); 
        
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

    if (frontEnemyActive) {
        glPushMatrix();
        glTranslatef(ballX, 5.0f, frontEnemyZ);
        
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

    LevelChunk* chunks[3] = {&prevChunk, &currChunk, &nextChunk};
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
            glTranslatef(chunks[c]->x[i], chunks[c]->y[i], chunks[c]->z[i]); 
            glScalef(chunks[c]->sx[i], 0.5, chunks[c]->sz[i]); 
            
            if (chunks[c]->type[i] == 0) { 
                if (i == 18) {
                    glColor3f(0.0, 0.0, 1.0);
                }
                else {
                    glColor3f(0.0, 1.0, 0.0);
                }
            } 
            else {
                if (!chaseStarted) {
                    glColor3f(1.0f, 0.5f, 0.0f);
                }
                else {
                    glColor3f(0.3f, 0.3f, 0.3f);
                }
            }            
            
            glutSolidCube(1.0); 
            
            if (!chaseStarted) {
                glColor3f(0.0, 0.0, 0.0);
            } 
            else {
                glColor3f(0.0, 1.0, 1.0);
            }
            
            glutWireCube(1.01); 
            glPopMatrix();
        }
    }
}
#include "../Include/ui.h"
#include "../Include/globals.h"
#include "../Include/texture.h"
#include "../Include/lighting.h"
#include "../Include/display.h"
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

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
}

void drawShadowText(float x, float y, const char* text, float r, float g, float b) {
    glColor3f(0.0f, 0.0f, 0.0f); 
    drawText(x + 2.0f, y - 2.0f, text);
    glColor3f(r, g, b); 
    drawText(x, y, text);
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
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.5f, 1.65f, 1.75f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(windowWidth, 0);
    glVertex2f(windowWidth, windowHeight);
    glVertex2f(0, windowHeight);
    glEnd();
    
    float tw = 440.0f;
    float th = 220.0f;
    float tx = (windowWidth - tw) / 2.0f;
    float ty = (windowHeight - th) / 2.0f + 60.0f;
    drawImage(titleTexture, tx, ty, tw, th); 
    
    float textY1 = ty - 60.0f;
    float textX1 = (windowWidth - 370.0f) / 2.0f;
    bool showBlink = ((glutGet(GLUT_ELAPSED_TIME) / 500) % 2 == 0);
    if (showBlink) {
        drawShadowText(textX1, textY1, "> INITIATE SYSTEM [PRESS ENTER]", 1.0f, 1.0f, 1.0f);
    }
    
    float textY2 = textY1 - 40.0f;
    float textX2 = (windowWidth - 160.0f) / 2.0f;
    drawShadowText(textX2, textY2, "[H] HELP MENU", 1.0f, 1.0f, 1.0f);
    
    float textY3 = textY2 - 30.0f;
    float textX3 = (windowWidth - 250.0f) / 2.0f;
    drawShadowText(textX3, textY3, "[ESC] SHUTDOWN SYSTEM", 1.0f, 1.0f, 1.0f);
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
    glColor3f(1.0f, 1.0f, 1.0f);
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
    
    char cpText[100];
    sprintf(cpText, "CHECKPOINTS CLEARED: %d", checkpointsPassed);
    drawShadowText(20.0f, windowHeight - 40.0f, cpText, 1.0f, 1.0f, 1.0f);
    
    drawShadowText(windowWidth - 240.0f, windowHeight - 40.0f, "SYS_OP: NOMINAL", 0.0f, 0.8f, 1.0f);
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    float cx = windowWidth / 2.0f;
    float cy = windowHeight / 2.0f;
    glBegin(GL_LINES);
    glVertex2f(cx - 12.0f, cy);
    glVertex2f(cx - 4.0f, cy);
    glVertex2f(cx + 4.0f, cy);
    glVertex2f(cx + 12.0f, cy);
    glVertex2f(cx, cy - 12.0f);
    glVertex2f(cx, cy - 4.0f);
    glVertex2f(cx, cy + 4.0f);
    glVertex2f(cx, cy + 12.0f);
    glEnd();
    glDisable(GL_BLEND);
    
    end2D();
}

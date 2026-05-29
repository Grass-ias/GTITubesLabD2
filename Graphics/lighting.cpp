#include "../Include/lighting.h"
#include <GL/glut.h>
#include "../Include/globals.h"
#include <math.h>

void setupLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    GLfloat baseAmbient[4] = {0.18f, 0.18f, 0.18f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, baseAmbient);
}

void updateLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Matikan lampu lain supaya hasil lighting tidak campur-campur.
    for (int l = 1; l < 8; l++) {
        glDisable(GL_LIGHT0 + l);
    }

    if (!chaseStarted) {
        // SUN MODE: cahaya hangat dari arah depan-atas pemain.
        GLfloat globalAmbient[4] = {0.30f, 0.28f, 0.22f, 1.0f};
        GLfloat lightPos[4]      = {playerX + 28.0f, playerY + 55.0f, playerZ - 70.0f, 1.0f};
        GLfloat ambient[4]       = {0.28f, 0.24f, 0.16f, 1.0f};
        GLfloat diffuse[4]       = {1.00f, 0.86f, 0.45f, 1.0f};
        GLfloat specular[4]      = {1.00f, 0.95f, 0.70f, 1.0f};

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.000f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.000f);

        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_START, 55.0f);
        glFogf(GL_FOG_END, 170.0f);
        GLfloat fogColor[4] = {0.20f, 0.55f, 0.78f, 1.0f};
        glFogfv(GL_FOG_COLOR, fogColor);
    }
    else {
        // MOON MODE: cahaya dingin dan rendah untuk suasana chase.
        GLfloat globalAmbient[4] = {0.05f, 0.06f, 0.10f, 1.0f};
        GLfloat lightPos[4]      = {playerX - 28.0f, playerY + 48.0f, playerZ - 65.0f, 1.0f};
        GLfloat ambient[4]       = {0.04f, 0.05f, 0.10f, 1.0f};
        GLfloat diffuse[4]       = {0.25f, 0.30f, 0.58f, 1.0f};
        GLfloat specular[4]      = {0.45f, 0.50f, 0.80f, 1.0f};

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.004f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00002f);

        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_START, currentFogStart);
        glFogf(GL_FOG_END, currentFogEnd);
        GLfloat fogColor[4] = {0.02f, 0.02f, 0.04f, 1.0f};
        glFogfv(GL_FOG_COLOR, fogColor);
    }

    GLfloat matSpecular[4] = {0.45f, 0.45f, 0.45f, 1.0f};
    GLfloat matShininess[1] = {35.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}

void glShadowProjection(float * l, float * e, float * n) {
    float d, c;
    float shadowMat[16];
    d = n[0]*l[0] + n[1]*l[1] + n[2]*l[2];
    c = e[0]*n[0] + e[1]*n[1] + e[2]*n[2] - d;
    shadowMat[0] = l[0]*n[0]+c; 
    shadowMat[4] = l[0]*n[1]; 
    shadowMat[8] = l[0]*n[2]; 
    shadowMat[12] = -l[0]*(e[0]*n[0]+e[1]*n[1]+e[2]*n[2]);
    shadowMat[1] = l[1]*n[0]; 
    shadowMat[5] = l[1]*n[1]+c; 
    shadowMat[9] = l[1]*n[2]; 
    shadowMat[13] = -l[1]*(e[0]*n[0]+e[1]*n[1]+e[2]*n[2]);
    shadowMat[2] = l[2]*n[0]; 
    shadowMat[6] = l[2]*n[1]; 
    shadowMat[10] = l[2]*n[2]+c; 
    shadowMat[14] = -l[2]*(e[0]*n[0]+e[1]*n[1]+e[2]*n[2]);
    shadowMat[3] = n[0]; 
    shadowMat[7] = n[1]; 
    shadowMat[11] = n[2]; 
    shadowMat[15] = -d;
    glMultMatrixf(shadowMat);
}

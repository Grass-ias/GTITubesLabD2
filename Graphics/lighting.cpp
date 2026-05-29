#include "../Include/lighting.h"
#include "../Include/globals.h"
#include <GL/glut.h>

static void setFogColor(float r, float g, float b, float startFog, float endFog) {
    GLfloat fogColor[4];
    fogColor[0] = r;
    fogColor[1] = g;
    fogColor[2] = b;
    fogColor[3] = 1.0f;

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, startFog);
    glFogf(GL_FOG_END, endFog);
    glFogfv(GL_FOG_COLOR, fogColor);
}

void setupLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    GLfloat globalAmbient[4];
    globalAmbient[0] = 0.16f;
    globalAmbient[1] = 0.16f;
    globalAmbient[2] = 0.18f;
    globalAmbient[3] = 1.0f;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    setFogColor(0.10f, 0.15f, 0.18f, 15.0f, 70.0f);
}

void updateLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    for (int l = 1; l < 8; l++) {
        glDisable(GL_LIGHT0 + l);
    }

    glEnable(GL_LIGHT0);

    if (!chaseStarted) {
        // DAY / NORMAL MAP: warm sun light.
        GLfloat sunPos[4];
        sunPos[0] = playerX + 35.0f;
        sunPos[1] = playerY + 90.0f;
        sunPos[2] = playerZ - 80.0f;
        sunPos[3] = 1.0f;

        GLfloat ambient[4];
        ambient[0] = 0.22f;
        ambient[1] = 0.22f;
        ambient[2] = 0.20f;
        ambient[3] = 1.0f;

        GLfloat diffuse[4];
        diffuse[0] = 0.95f;
        diffuse[1] = 0.86f;
        diffuse[2] = 0.62f;
        diffuse[3] = 1.0f;

        GLfloat specular[4];
        specular[0] = 0.80f;
        specular[1] = 0.74f;
        specular[2] = 0.55f;
        specular[3] = 1.0f;

        GLfloat globalAmbient[4];
        globalAmbient[0] = 0.18f;
        globalAmbient[1] = 0.18f;
        globalAmbient[2] = 0.18f;
        globalAmbient[3] = 1.0f;

        glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

        setFogColor(0.10f, 0.15f, 0.18f, 15.0f, 75.0f);
    }
    else {
        // CHASE: moon light, blue and darker, not red eclipse.
        GLfloat moonPos[4];
        moonPos[0] = playerX - 32.0f;
        moonPos[1] = playerY + 70.0f;
        moonPos[2] = playerZ - 62.0f;
        moonPos[3] = 1.0f;

        GLfloat ambient[4];
        ambient[0] = 0.08f;
        ambient[1] = 0.09f;
        ambient[2] = 0.16f;
        ambient[3] = 1.0f;

        GLfloat diffuse[4];
        diffuse[0] = 0.36f;
        diffuse[1] = 0.40f;
        diffuse[2] = 0.62f;
        diffuse[3] = 1.0f;

        GLfloat specular[4];
        specular[0] = 0.45f;
        specular[1] = 0.50f;
        specular[2] = 0.80f;
        specular[3] = 1.0f;

        GLfloat globalAmbient[4];
        globalAmbient[0] = 0.045f;
        globalAmbient[1] = 0.050f;
        globalAmbient[2] = 0.085f;
        globalAmbient[3] = 1.0f;

        glLightfv(GL_LIGHT0, GL_POSITION, moonPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

        // Small cold fill around player so platforms are still visible.
        glEnable(GL_LIGHT1);
        GLfloat fillPos[4];
        fillPos[0] = playerX;
        fillPos[1] = playerY + 8.0f;
        fillPos[2] = playerZ + 8.0f;
        fillPos[3] = 1.0f;

        GLfloat fillAmbient[4];
        fillAmbient[0] = 0.02f;
        fillAmbient[1] = 0.02f;
        fillAmbient[2] = 0.04f;
        fillAmbient[3] = 1.0f;

        GLfloat fillDiffuse[4];
        fillDiffuse[0] = 0.12f;
        fillDiffuse[1] = 0.14f;
        fillDiffuse[2] = 0.22f;
        fillDiffuse[3] = 1.0f;

        GLfloat fillSpecular[4];
        fillSpecular[0] = 0.05f;
        fillSpecular[1] = 0.06f;
        fillSpecular[2] = 0.10f;
        fillSpecular[3] = 1.0f;

        glLightfv(GL_LIGHT1, GL_POSITION, fillPos);
        glLightfv(GL_LIGHT1, GL_AMBIENT, fillAmbient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, fillDiffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, fillSpecular);
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.025f);
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.002f);

        setFogColor(0.015f, 0.020f, 0.040f, currentFogStart, currentFogEnd);
    }
}

void glShadowProjection(float * l, float * e, float * n) {
    float d, c;
    float shadowMat[16];

    d = n[0]*l[0] + n[1]*l[1] + n[2]*l[2];
    c = e[0]*n[0] + e[1]*n[1] + e[2]*n[2] - d;

    shadowMat[0]  = l[0]*n[0] + c;
    shadowMat[4]  = l[0]*n[1];
    shadowMat[8]  = l[0]*n[2];
    shadowMat[12] = -l[0]*(e[0]*n[0] + e[1]*n[1] + e[2]*n[2]);

    shadowMat[1]  = l[1]*n[0];
    shadowMat[5]  = l[1]*n[1] + c;
    shadowMat[9]  = l[1]*n[2];
    shadowMat[13] = -l[1]*(e[0]*n[0] + e[1]*n[1] + e[2]*n[2]);

    shadowMat[2]  = l[2]*n[0];
    shadowMat[6]  = l[2]*n[1];
    shadowMat[10] = l[2]*n[2] + c;
    shadowMat[14] = -l[2]*(e[0]*n[0] + e[1]*n[1] + e[2]*n[2]);

    shadowMat[3]  = n[0];
    shadowMat[7]  = n[1];
    shadowMat[11] = n[2];
    shadowMat[15] = -d;

    glMultMatrixf(shadowMat);
}

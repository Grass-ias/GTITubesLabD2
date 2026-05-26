#include "../Include/lighting.h"
#include <GL/glut.h>
#include "../Include/globals.h"
#include <math.h>

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 15.0f);
    glFogf(GL_FOG_END, 70.0f);
    float fogColorArray[4];
    fogColorArray[0] = 0.1f;
    fogColorArray[1] = 0.15f;
    fogColorArray[2] = 0.18f;
    fogColorArray[3] = 1.0f;
    glFogfv(GL_FOG_COLOR, fogColorArray);
}

void updateLighting() {
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 15.0f);
    glFogf(GL_FOG_END, 70.0f);
    float fogColorArray[4];
    if (!chaseStarted) {
        fogColorArray[0] = 0.1f;
        fogColorArray[1] = 0.15f;
        fogColorArray[2] = 0.18f;
        fogColorArray[3] = 1.0f;
    } else {
        fogColorArray[0] = 0.08f;
        fogColorArray[1] = 0.0f;
        fogColorArray[2] = 0.0f;
        fogColorArray[3] = 1.0f;
    }
    glFogfv(GL_FOG_COLOR, fogColorArray);
    if (!chaseStarted) {
        glEnable(GL_LIGHT0);
        for (int l = 1; l < 8; l++) {
            glDisable(GL_LIGHT0 + l);
        }

        GLfloat sunPos[4];
        sunPos[0] = 0.0f;
        sunPos[1] = 100.0f;
        sunPos[2] = playerZ;
        sunPos[3] = 1.0f;

        GLfloat sunAmbient[4];
        sunAmbient[0] = 0.15f;
        sunAmbient[1] = 0.18f;
        sunAmbient[2] = 0.2f;
        sunAmbient[3] = 1.0f;

        GLfloat sunDiffuse[4];
        sunDiffuse[0] = 0.9f;
        sunDiffuse[1] = 0.85f;
        sunDiffuse[2] = 0.6f;
        sunDiffuse[3] = 1.0f;

        GLfloat sunSpecular[4];
        sunSpecular[0] = 1.0f;
        sunSpecular[1] = 0.8f;
        sunSpecular[2] = 0.8f;
        sunSpecular[3] = 1.0f;

        glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular);

        GLfloat standardAmbient[4];
        standardAmbient[0] = 0.1f;
        standardAmbient[1] = 0.1f;
        standardAmbient[2] = 0.1f;
        standardAmbient[3] = 1.0f;

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, standardAmbient);
    }
    else {
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);

        GLfloat sunPos[4];
        sunPos[0] = 0.0f;
        sunPos[1] = 100.0f;
        sunPos[2] = playerZ;
        sunPos[3] = 1.0f;

        GLfloat sunAmbient[4];
        sunAmbient[0] = 0.2f;
        sunAmbient[1] = 0.0f;
        sunAmbient[2] = 0.0f;
        sunAmbient[3] = 1.0f;

        GLfloat sunDiffuse[4];
        sunDiffuse[0] = 0.9f;
        sunDiffuse[1] = 0.2f;
        sunDiffuse[2] = 0.05f;
        sunDiffuse[3] = 1.0f;

        GLfloat sunSpecular[4];
        sunSpecular[0] = 0.3f;
        sunSpecular[1] = 0.05f;
        sunSpecular[2] = 0.0f;
        sunSpecular[3] = 1.0f;

        glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular);

        GLfloat glowPos[4];
        glowPos[0] = 0.0f;
        glowPos[1] = 1.0f;
        glowPos[2] = 0.0f;
        glowPos[3] = 0.0f;

        GLfloat glowAmbient[4];
        glowAmbient[0] = 0.05f;
        glowAmbient[1] = 0.05f;
        glowAmbient[2] = 0.15f;
        glowAmbient[3] = 1.0f;

        GLfloat glowDiffuse[4];
        glowDiffuse[0] = 0.05f;
        glowDiffuse[1] = 0.05f;
        glowDiffuse[2] = 0.15f;
        glowDiffuse[3] = 1.0f;

        GLfloat glowSpecular[4];
        glowSpecular[0] = 0.0f;
        glowSpecular[1] = 0.0f;
        glowSpecular[2] = 0.0f;
        glowSpecular[3] = 1.0f;

        glLightfv(GL_LIGHT1, GL_POSITION, glowPos);
        glLightfv(GL_LIGHT1, GL_AMBIENT, glowAmbient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, glowDiffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, glowSpecular);

        GLfloat eclipseAmbient[4];
        eclipseAmbient[0] = 0.15f;
        eclipseAmbient[1] = 0.05f;
        eclipseAmbient[2] = 0.05f;
        eclipseAmbient[3] = 1.0f;

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, eclipseAmbient);

        struct ActiveLight {
            float x, y, z;
            float distSq;
        };

        ActiveLight activeLights[6];
        int numActiveLights = 0;

        LevelChunk* chunks[3];
        chunks[0] = &prevChunk;
        chunks[1] = &currChunk;
        chunks[2] = &nextChunk;
        for (int c = 0; c < 3; c++) {
            if (!chunks[c]->active) continue;
            for (int i = 0; i < JUMLAH_PLATFORM; i++) {
                float fx = chunks[c]->x[i];
                float fy = chunks[c]->y[i] + 0.25f + 0.2f;
                float fz = chunks[c]->z[i];
                
                float dx = fx - playerX;
                float dy = fy - playerY;
                float dz = fz - playerZ;
                float distSq = dx*dx + dy*dy + dz*dz;

                if (numActiveLights < 6) {
                    ActiveLight light;
                    light.x = fx;
                    light.y = fy;
                    light.z = fz;
                    light.distSq = distSq;
                    activeLights[numActiveLights++] = light;

                    for (int j = numActiveLights - 1; j > 0; j--) {
                        if (activeLights[j].distSq < activeLights[j-1].distSq) {
                            ActiveLight temp = activeLights[j];
                            activeLights[j] = activeLights[j-1];
                            activeLights[j-1] = temp;
                        }
                    }
                }
                else if (distSq < activeLights[5].distSq) {
                    ActiveLight light;
                    light.x = fx;
                    light.y = fy;
                    light.z = fz;
                    light.distSq = distSq;
                    activeLights[5] = light;

                    for (int j = 5; j > 0; j--) {
                        if (activeLights[j].distSq < activeLights[j-1].distSq) {
                            ActiveLight temp = activeLights[j];
                            activeLights[j] = activeLights[j-1];
                            activeLights[j-1] = temp;
                        }
                    }
                }
            }
        }

        for (int l = 0; l < 6; l++) {
            GLuint lightEnum = GL_LIGHT2 + l;
            if (l < numActiveLights) {
                glEnable(lightEnum);

                GLfloat lightPos[4];
                lightPos[0] = activeLights[l].x;
                lightPos[1] = activeLights[l].y;
                lightPos[2] = activeLights[l].z;
                lightPos[3] = 1.0f;

                GLfloat spotDir[3];
                spotDir[0] = 0.0f;
                spotDir[1] = -1.0f;
                spotDir[2] = 0.0f;

                float spotCutoff = 45.0f;
                float spotExponent = 10.0f;

                GLfloat diffuse[4];
                diffuse[0] = 1.0f;
                diffuse[1] = 0.7f;
                diffuse[2] = 0.2f;
                diffuse[3] = 1.0f;

                GLfloat specular[4];
                specular[0] = 1.0f;
                specular[1] = 1.0f;
                specular[2] = 1.0f;
                specular[3] = 1.0f;

                GLfloat ambient[4];
                ambient[0] = 0.0f;
                ambient[1] = 0.0f;
                ambient[2] = 0.0f;
                ambient[3] = 1.0f;

                glLightfv(lightEnum, GL_POSITION, lightPos);
                glLightfv(lightEnum, GL_SPOT_DIRECTION, spotDir);
                glLightf(lightEnum, GL_SPOT_CUTOFF, spotCutoff);
                glLightf(lightEnum, GL_SPOT_EXPONENT, spotExponent);
                glLightfv(lightEnum, GL_AMBIENT, ambient);
                glLightfv(lightEnum, GL_DIFFUSE, diffuse);
                glLightfv(lightEnum, GL_SPECULAR, specular);

                glLightf(lightEnum, GL_CONSTANT_ATTENUATION, 1.0f);
                glLightf(lightEnum, GL_LINEAR_ATTENUATION, 0.05f);
                glLightf(lightEnum, GL_QUADRATIC_ATTENUATION, 0.01f);
            }
            else {
                glDisable(lightEnum);
            }
        }
    }
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

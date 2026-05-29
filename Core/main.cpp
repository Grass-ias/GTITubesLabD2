#include <GL/glut.h>
#include "../Include/globals.h"
#include "../Include/engine.h"

int main(int argc, char **argv) {
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("YOU CAN'T RUN FROM IT"); 
    
    initEngine(); 
    setupCallbacks();
    
    glutMainLoop(); 
    return 0;
}

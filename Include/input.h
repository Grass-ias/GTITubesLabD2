#ifndef INPUT_H
#define INPUT_H

void mouseMotion(int x, int y);
void mouseClick(int button, int state, int x, int y);
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);

#endif
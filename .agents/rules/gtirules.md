---
trigger: always_on
---

Rule 1: Absolute Main.cpp Purity. main.cpp is ONLY allowed to contain #include statements and the int main() function. It cannot contain ANY global variables, timers, display callbacks, or logic. It simply calls initEngine(), setupCallbacks(), and glutMainLoop().

Rule 2: The Core Engine Hub. All GLUT callbacks (timer, display, reshape, keyboardDown, mouseClick) must be encapsulated within a new Core/engine.cpp (and engine.h) file.

Rule 3: Hyper-Modular Separation of Concerns.
Graphics/texture.cpp: Must exclusively handle loadTexture and stb_image implementations.
Graphics/display.cpp: Must exclusively handle rendering (drawGame3D, drawHUD, drawMenu, etc.).
Audio/sound.cpp: Must hold all audio initialization and playback functions.
Physics/collision.cpp: Must hold checkWallCollision, getGroundY, and any spatial math.
Entity/player.cpp & Entity/enemy.cpp: Must contain both their creation logic (initPlayer, initEnemy) AND their update logic (updatePlayer, updateEnemy).

Rule 4: Header Consolidation. All .h files belong in a unified Include/ folder at the root. All .cpp files must correctly route their includes to this folder (e.g., #include "../Include/globals.h"

Rule 5: NO COMPILATION: Do NOT execute any build commands (like g++ or make). Do NOT attempt to run the code. Just refactor the text and file structure.
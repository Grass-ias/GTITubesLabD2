Building texture repeat patch

What changed:
- Assets/Textures/textureBuilding1.jpg
- Assets/Textures/textureBuilding2.jpg
- Assets/Textures/textureBuilding3.jpg
- Graphics/texture.cpp now loads textures with GL_REPEAT and mipmaps.
- Graphics/display.cpp now applies textureBuilding1/2/3 to the buildings on both sides of the map.
- The building walls use repeated texture coordinates based on building width/height/depth.
- Roofs are plain dark gray so the window facade texture is not stretched onto the top.
- Texture upload uses glPixelStorei(GL_UNPACK_ALIGNMENT, 1) to avoid striped/corrupted JPG output.

How to run:
1. Close Dev-C++ completely.
2. Extract this zip into a new folder.
3. Open RAPGv1.dev only. Do not open Makefile.win directly.
4. Execute -> Clean.
5. Execute -> Rebuild All.

To tune window size:
Open Graphics/display.cpp, find:
repeatX = sizeX / 10.0f
repeatY = sizeY / 55.0f
repeatZ = sizeZ / 10.0f

Bigger divisor = larger window texture tiles.
Smaller divisor = smaller/more repeated window texture tiles.

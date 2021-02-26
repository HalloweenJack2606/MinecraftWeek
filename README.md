#Minecraft Week 
![terrain_screenshot](Screenshots/terrain.jpg)

#### Features:
 - Infinite terrain generation
 - Frustum Culling
 - Chunk Batching System (16x256x16)
 - Placing & Breaking blocks using raycasting
 - Others...

#### Building
There are a few libraries which are required to build this project.
 - GLFW
 - GLEW
 - GLM
 - SOIL

It might not work when building with some compilers since it includes <mutex>, which is not supported by a few. All the multi-threading is disabled (World::ChunkLoading() but still works perfectly, already tested it), it would still be necessary to remove the related includes. Visual Studios's MsBuild should work perfectly.
  
The .exe should be inside 'bin/Release-x64/Game'. Only works for Window at the time of writing.

![wooden_house](Screenshots/woodenhouse.png)

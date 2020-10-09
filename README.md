RenderBoi
=========

A simple rendering engine with a modest feature set.  
Originally a pet project to get started with real-time rendering app 
architecture and the 3D graphics software stack.

**Example screenshot:** (click to open a video)  

[![example screenshot](https://i.imgur.com/2xz4x0M.png)][video]  

All meshes are entirely parametrically generated. The tetrahedron is a child 
of the golden torus, itself being a child of the emerald, larger torus. It is 
apparent that children objects are affected by their parent. The cube shows the
position of a point light. The tetrahedron is being rendered with a different 
shader, which does not take into account light.

## Info

* Requirements:
  * an OpenGL 4.2-capable environment,
  * which supports GL_ARB_debug_output.
* I am still figuring out cross-platformability. If the project does not work
on your setup, please file an issue with details so I can have a look at it.
* See this [sort of a diagram][diag1] as well as this [class diagram][diag2] to get a basic understanding of how everything works.

## Current features

* **Meshes**
* **Materials**
* **Textures**
* **Shaders**
* **UBO handling**
* **Lighting:**
  * Point lights
  * Light maps
  * Spotlights
  * Directional lights
* **Lighting methods:**
  * Phong
* **Scene graph** and Unity-like **component system**:
  * Scene objects are arranged in a tree
  * The world transforms of scene objects depend on their successive parents'
  * Scene objects can have several components:
    * **Mesh component:** the scene object has a mesh and will be rendered;
    * **Light component:** the scene object emits light;
    * **Camera component:** a camera is attached to the scene object;
    * **Script component:** a script class controls the aspects of the scene object based on time, user input, etc.
* **FPS camera and key bindings**, implemented as a script component attached to the scene camera

## Roadmap

* Project health: tests and CI
* `.obj` file loading
* Shadows
* Transparency
* Dynamic meshes
* Particles
* Portals
* PBR

See file [`list.todo`][todo] for a more concise view of what is likely to be worked on at present.

[video]: https://streamable.com/vrq3u8
[diag1]: https://github.com/deqyra/RenderBoi/blob/master/system_diagram.png
[diag2]: https://github.com/deqyra/RenderBoi/blob/master/class_diagram.png
[todo]:  https://github.com/deqyra/RenderBoi/blob/master/list.todo
RenderBoi
=========

A simple rendering engine with a modest feature set.  
Originally a pet project to get started with real-time rendering app architecture.

**Example screenshot:** (click to open a video)  

[![example screenshot](https://i.imgur.com/2xz4x0M.png)](https://streamable.com/mkce4)  

All meshes here are entirely parametrically generated.
The tetrahedron is a child of the golden torus, itself being a child of the emerald, larger torus.
It is apparent that children objects are affected by their parent's transform.
The cube shows the position of a point light.

## Info

* Development conducted using OpenGL 4.6 (core profile) on a GTX 1660 Ti.
* To use the project, [generate a loader](https://glad.dav1d.de/) that suits your platform, and replace `include/glad/glad.h` and `src/glad.cpp` with it.
* See this [sort of a diagram](https://github.com/deqyra/RenderBoi/blob/master/system_diagram.png) as well as this [class diagram](https://github.com/deqyra/RenderBoi/blob/master/class_diagram.png) to get a basic understanding of how everything works.

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
  * Children scene object global positions depend on their successive parents'
  * Scene objects can have several components:
    * **Mesh component:** a mesh is rendered at the scene object's position;
    * **Light component:** a light source is simulated at the scene object's position;
    * **Camera component:** a camera is attached to the scene object and provides the view-projection matrix;
    * **Script component:** a script controls the aspects of the scene object based on time, user input, etc.
* **FPS camera and key bindings**, implemented as a script component attached to the scene camera

## Roadmap

* `.obj` file loading
* Shadows
* Transparency
* Dynamic meshes
* Particles
* Portals
* PBR

See file [`list.todo`](https://github.com/deqyra/RenderBoi/blob/master/list.todo) for a more concise view of what is likely to be worked on at present.

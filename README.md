RenderBoi
=========

A simple rendering engine with a modest feature set.  
Originally a pet project to get started with real-time rendering app architecture.

**Example screenshot:**  

![example screenshot](https://i.imgur.com/h9Y7qJM.png)  

The torus mesh is entirely parametrically generated and rendered with a (so-called) emerald material. The coloured cube shows the position of a point light.

![wireframe view](https://i.imgur.com/m7bdLME.png)

## Info

* Development conducted using OpenGL 4.6 (core profile) on a GTX 1660 Ti.
* To use the project, [generate a loader](https://glad.dav1d.de/) that suits your platform, and replace `include/glad/glad.h` with it.

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

// ! \\\\ Massive code cleanup needed // ! \\\\

* `.obj` file loading
* Shadows
* Transparency
* Portals
* Particles
* PBR

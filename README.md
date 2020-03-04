GLSandbox
=========

A dumb test project to play around with OpenGL and experiment with the architecture of a real-time rendering app.  
Aiming to evolve into a more nicely-featured rendering engine.  

**Example screenshot:**  

![example screenshot](https://i.imgur.com/h9Y7qJM.png)  

The torus mesh is entirely parametrically generated and has a (so-called) emerald material. The coloured cube shows the position of a point light.

![wireframe view](https://i.imgur.com/m7bdLME.png)

## Info

* Development conducted using OpenGL 4.6 (core profile) on a GTX 1660 Ti.
* If your graphics driver does not implement OpenGL 4.6 core, [generate a loader](https://glad.dav1d.de/) that suits your platform, and replace `include/glad/glad.h` with it.

## Current features

* **Transforms**
* **FPS camera & key bindings**
* **Self-drawing meshes**
* **Per-mesh shaders**
* **UBOs**
* **Textures**
* **Materials**
* **Lighting methods:**
  * Phong
* **Lighting:**
  * Point lights
  * Light maps
* **Scene graph** and Unity-like **component system**:
  * Scene objects are arranged in a tree
  * Children scene object global position depends on their successive parents
  * Scene objects can have several components:
    * **Mesh component:** a mesh is rendered at the scene object's position;
	* **Light component:** a light source is simulated at the scene object's position;
	* **Camera component:** a camera is attached to the scene object and provides the view-projection matrix;
	* **Script component:** a script controls the aspects of the scene object based on time, user input, etc.

## Roadmap

* Lighting:
  * Spotlights
  * Directional lights
* `.obj` file handling
* Shadows
* Transparency
* Portals
* Particles
* PBR

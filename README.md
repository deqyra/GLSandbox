GLSandbox
=========

A dumb test project to play around with OpenGL and experiment with the architecture of a real-time rendering app.  
Aiming to evolve into a more nicely-featured rendering engine.  

Example screenshot:  

![example screenshot](https://i.imgur.com/h9Y7qJM.png)  

The torus mesh is entirely parametrically generated and has a (so-called) emerald material. The coloured cube shows the position of a point light.

### Requirements

* A graphics card capable with OpenGL 4.6 or higher

### Current features

* Transforms
* FPS camera & key bindings
* Self-drawing meshes
* Per-mesh shaders
* UBOs
* Textures
* Materials
* Lighting methods:
  * Phong
* Lighting:
  * Point lights

### Roadmap

* Lighting:
  * Spotlights
  * Directional lights
  * Light maps
* `.obj` file handling
* Shadows
* Scene graph
* PBR

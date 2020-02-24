GLSandbox
=========

A dumb test project to play around with OpenGL and experiment with the architecture of a real-time rendering app.  
Aiming to evolve into a more nicely-featured rendering engine.  

Example screenshot:  

![example screenshot](https://i.imgur.com/h9Y7qJM.png)  

The torus mesh is entirely parametrically generated and has a (so-called) emerald material. The coloured cube shows the position of a point light.

### Current features

* Transforms
* FPS camera & key bindings
* Self-drawing meshes
* Per-mesh shaders
* A horrible thing called ShaderAdapter which attempts to abstract shader uniform names for the app
* Textures (kind of)
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
* Actually decent texture handling
* UBOs
* `.obj` file handling
* Shadows
* Scene graph
* PBR

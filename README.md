RenderBoi
=========

A simple rendering engine with a modest feature set.  
Originally a pet project to get started with real-time rendering app 
architecture and the 3D graphics software stack.

**Example screenshot:** (click to open a video)  

[![example screenshot](https://i.imgur.com/2xz4x0M.png)][video]  

All meshes are entirely parametrically generated. The tetrahedron is a child 
of the golden torus, itself being a child of the emerald, larger torus. It is 
apparent that the transforms of children objects are being affected by that of
their parent. The cube shows the position of a point light. The tetrahedron is
being rendered with a different shader, which does not take lighting into
account.

## Info

* Simply requires:
  * an OpenGL 4.2-capable environment;
  * a C++20-compliant compiler.
* GL extensions used:
  * `GL_ARB_debug_output`.
* I am still figuring out cross-platformability. If the project does not work
on your setup, please file an issue with details so I can have a look at it.
* Original development environments:
  * **Windows 10** / Nvidia GeForce GTX 1650 Ti
  * **Windows 10** / Intel (Core i7 10610U) UHD Graphics
  * **Arch Linux** (kernel 5.6.15) / **GNOME with Wayland** (EGL backend) / Nvidia GeForce GT630M
* Here are links to some UML diagrams which give a decent overview of how everything adds up:
  * [Package diagram][pkg_diag]
  * [Core class diagram][core_diag]
  * [Window class diagram][window_diag]
  * [Toolbox class diagram][toolbox_diag]
  * [StarUML file][mdj]

## Building

If you're lucky, it should be fast and painless:  
```sh
$ git clone git@github.com:deqyra/RenderBoi.git
$ cd RenderBoi
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

This will fetch and build all dependencies, then build RenderBoi as a set of 
libraries, and finally build an executable which when run will display a series 
of example scenes. The executable must be run from the same directory as 
`assets/` (a symlink is fine), or must be provided the path to `assets/` from 
the current directory as a command line argument:  
```sh
$ RenderBoi [(-a|--assets) <path>]
```

If you run into trouble, refer to that wiki page which I haven't even started 
writing yet.

## Current features

* **Meshes**
* **Textures**
* **Materials:**
  * Diffuse maps
  * Specular maps
* **On-the-fly parametric shader generation**
* **UBO handling**
* **Lighting:**
  * Point lights
  * Spotlights
  * Directional lights
* **Lighting models:**
  * Phong
  * Blinn-Phong
* **Scene graph** and Unity-like **component system**:
  * Scene objects are arranged in a tree
  * The world transforms of scene objects depend on that of their successive parents
  * Scene objects can have several components:
    * **Mesh component:** the scene object has a mesh and will be rendered;
    * **Light component:** the scene object emits light;
    * **Camera component:** a camera is attached to the scene object;
    * **Script component:** a script class controls the aspects of the scene object based on time, user input, etc.
* Classes for **easy key binding management**

### Provided scripts:
* Input logger
* First person camera control (mouse)
* Directional movement (keyboard)
* Camera aspect ratio auto fit

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
[pkg_diag]: https://raw.githubusercontent.com/deqyra/RenderBoi/master/uml/package_diagram.png
[core_diag]: https://raw.githubusercontent.com/deqyra/RenderBoi/master/uml/core_class_diagram.png
[window_diag]: https://raw.githubusercontent.com/deqyra/RenderBoi/master/uml/window_class_diagram.png
[toolbox_diag]: https://raw.githubusercontent.com/deqyra/RenderBoi/master/uml/toolbox_class_diagram.png
[mdj]: https://github.com/deqyra/RenderBoi/blob/GL15-shader-improvements/uml/RenderBoi%20model.mdj
[todo]:  https://github.com/deqyra/RenderBoi/blob/master/list.todo
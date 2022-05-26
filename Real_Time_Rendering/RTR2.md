- [II: Pipelining](#ii-pipelining)
  - [Application Stage](#application-stage)
  - [Geometry Stage:](#geometry-stage)
  - [Rasterization:](#rasterization)
  - [Pixel Shading:](#pixel-shading)

# II: Pipelining

- *def.* pipelining: separate different stages for parallelism
## Application Stage
- determines the primities to render, happens in the CPU
## Geometry Stage:
   1. vertex shading:
      1. model transform: model $\to$ world space
      2. view transform: world $\to$ view/camera space
      3. projection + clipping: view/camera space $\to$ unit cube
       - *def* shading: operation determining the effect of light on an object 
   2. vertex processing optionals
      1. tessellation: optimally breaking curved surfaces to polygon primitives, based on level of detail and on-screen size
      2. geometry shader: geometric new vertices around key points
      3. stream output: output processed vertices as an array
   3. clipping
       * discard primitives wholly outside the volume
       * keep primitives fully in
       * clip primitives partially in
   4. screen mapping
       * map view volume to unit cube
        * openGL: $z \in [-1, 1]$, origin lower left
        * directX: $z \in [0, 1]$, origin upper left
## Rasterization:
1. triangle setup: computer shape data
2. triangle traversal: generate fragments
## Pixel Shading:
* *def.* frame buffer: color buffer, z buffer, alpha better, stencil buffer
* *def.* double buffering: write to back buffer, only swap front-back buffer when finish the current frame
1. fragment shader: texturing
2. merging: use shaded fragments to update the color buffer
     * z-buff alg (O(n)): for pixel $i$ belonging to fragment $f$, only write $i$ to the color buffer if $z_f$ is closer than what was already on the z-buffer at $i$
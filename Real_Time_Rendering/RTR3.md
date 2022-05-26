- [III. Graphics Processing Unit](#iii-graphics-processing-unit)
  - [Minimizing Latency](#minimizing-latency)
  - [Models for the GPU pipeline](#models-for-the-gpu-pipeline)
  - [Unified Shader Design](#unified-shader-design)
  - [Vertex Shader](#vertex-shader)
  - [Tessellation](#tessellation)
  - [Geometry Shader](#geometry-shader)
  - [Stream Output](#stream-output)
  - [Pixel (Fragment) Shader](#pixel-fragment-shader)
  - [Merging Stage](#merging-stage)
  - [Compute Shader](#compute-shader)

# III. Graphics Processing Unit
## Minimizing Latency
  - cpu: caches, branch prediction, ...
  - gpu: multiple shader cores, non-interdependent tasks, ...
    - maximizes throughput (jobs per time), but latency is high
    - each core has some local memory (registers)
    - constantly switching across different fragments to fill waiting time
      - *ex.* do some fragment shading while texturing awaits for memory fetch
  - SIMD (Single Instruction Multiple Data):
    - GPU thread: entity consisting of input + register specifications
    - similar threads are bundled, then assigned to *warp/wavefront*
    - for each warp/wavefront, split into single threads and assign to *SIMD Lanes*
    - each warp/wavefront is scheduled for 32 shader cores
      - when stalled, the entire warp is swapped out
  - *def.* occupancy: # threads currently on the GPU
    - low occupancy = low throughput
    - occupancy limited by register count and shared memory size
  - *def.* thread divergence: when branched, both versions are calculated while awaiting for branch evaluation. Too many subsequent branchings make branch counts out of control
## Models for the GPU pipeline
  - logical: the model of the GPU exposed *to* the API (*ex.* OpenGL, Vulcan, ...)
  - physical: actual implementation of the instructions
  - Programmable stages:
    - vertex shader, geometry shader, tessellation
  - Fixed Function stages:
    - clipping, triangle setup, triangle traversal
  - Configurable:
    - *def.*: fixed function stage but with a large amount of settings
    - merging stage

## Unified Shader Design
- same ISA, leading to good predictors for scheduler about computational load
  - DirectX: HLSL $\to$ byte code $\to$ ISA
  - OpenGL: GLSL
- *def.* draw call: invocation of API on a group of primitives
  - *def.* unifying input: constant within draw call, but may vary between different calls
    - *ex.* material color
  - *def.* varying input: varying within draw call
    - *ex.* current pixel
  - inputs are assigned to constant and varying registeres respectively
    - usually the GPU has more constant registers
  - operations, intrinsic operators (sqrt, ...), functions (normalize, ...)
  - flow control: static branching (for dynamic in.), dynamic branching (for varying in.)
  - other GPU languages
    - Volkan (SPIRV): lower overhead
    - OpenGL ES: embedded systems
      - WebGL: interpreted from JS

## Vertex Shader
- input asseembler handles data before vertex shader
- vertex shaders deal exclusively with vertex properties
- cannot create or delete primitives

## Tessellation
- render curved surface efficiently by adjusting level of detail
- components
  1. tessellation control shader
  2. primitive generator
  3. tessellation evaluation shader

## Geometry Shader
- primitive $\to$ vertices
- moficiation & copying of data (under preserved order)
- not often used since performance is slow

## Stream Output
- used for iterative processing
  - feed generated vertices back to vertex shader and start over

## Pixel (Fragment) Shader
- col, z, ... $\to$ result
- modify z and alpha, but mostly produces color
- interpolate values
- can discard fragment
- multiple render targets (outputs outside the merging stage)
- computation of gradients:
  - access adjacent fragments *in the same warp*
  - not compatible with dynamic branching:
    - all four vertices of the quad must pass at once, not stalled
  - random access mechanisms:
    - Unordered Access View / Shader Storage Buffer Object
  - Rasterizer Order Views: enforace layer order

## Merging Stage
- to same resource, processors usually do merge testing *before* pixel shading ("early z")
    - this is incompatible with pixel shaders that change z, since merging (occlusion) is z-based

## Compute Shader
- GPU as parallel processor, through interfaces like CUDA
- access bufferes on the GPU
- post processing (*ex.* average luminance)
- minimize CPU-GPU communication

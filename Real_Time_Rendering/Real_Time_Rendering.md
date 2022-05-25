**Real Time Rendering (4'th Edition)**

- [I: Symbols & Conventions](#i-symbols--conventions)
  - [coordinates](#coordinates)
- [II: Pipelining](#ii-pipelining)
  - [Application Stage](#application-stage)
  - [Geometry Stage:](#geometry-stage)
  - [Rasterization:](#rasterization)
  - [Pixel Shading:](#pixel-shading)
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
- [IV. Transforms](#iv-transforms)
  - [Matrix-based](#matrix-based)
  - [Quaternions](#quaternions)
  - [Vertex Blending](#vertex-blending)
  - [Morphing](#morphing)
  - [Geometry Cache Playback](#geometry-cache-playback)
  - [Projection](#projection)
    - [Orthographic Projection](#orthographic-projection)
    - [Perspective Projection](#perspective-projection)
- [V. Shading Models](#v-shading-models)
  - [Lambertian Model](#lambertian-model)
  - [Types of Light](#types-of-light)
    - [Directional Light](#directional-light)
    - [Punctual Light](#punctual-light)
  - [Implementing Shading Models](#implementing-shading-models)
  - [Example Implementation](#example-implementation)
    - [Shader Inputs and Outputs](#shader-inputs-and-outputs)
    - [Pixel Shader](#pixel-shader)
    - [Vertex Shader](#vertex-shader-1)
    - [Material Systems](#material-systems)
  - [Aliasing & Anti-Aliasing](#aliasing--anti-aliasing)
    - [Sampling & Filtering Theory](#sampling--filtering-theory)
    - [Morphological Methods](#morphological-methods)
  - [Transparency](#transparency)
    - [Screen-door Transparency](#screen-door-transparency)
    - [Alpha Blending](#alpha-blending)
    - [Blending ORder](#blending-order)
    - [Order Independent Transparency](#order-independent-transparency)
    - [Premultiplied Alphas & Compositing](#premultiplied-alphas--compositing)
  - [Display Encoding](#display-encoding)

# I: Symbols & Conventions
## coordinates
- $z$ out, $x$ right, $y$ up
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

# IV. Transforms
## Matrix-based
- Linear (3x3)
- Affine (Linear + Translate)
  - parallel preserved
- Homogeneous coordinates
  - direction: $w=0$
  - location: $w=1$
- rigid body: $X = T(\bold{t})R$
    - no similarity
- scaling in a certain direction
  - for basis $F = \begin{pmatrix} \hat \bold{f}_x & \hat \bold{f}_y & \hat \bold{f}_z & 0 \\ 0 & 0 & 0 & 1\end{pmatrix}$, do $FS(\bold{s})F^T\bold{v}$
- normal transform (transforming a direction)
  - use transpose of adjoint, then re-normalize
  - other than projection, $w$ is unchanged, so usually taking the upper 3x3 for tr. adj. is enough
- simple inverse transforms
    1. Reverse using given properties
        * $R(\phi)^{-1} = R(-\phi)$
        * $T(\bold{t})^{-1} = T(-\bold{t})$
        * $S(\bold{s})^{-1} = S(\bold{s}^{-1})$
    1. A known orthogonal matrix can be inverted by transposing
    2. Inverse of concatenated transforms: $(T_1 T_2)^{-1} = T_2^{-1} T_1^{-1}$
- Euler transforms
  - $E(h, p, r) = R_z(r) R_x(p) R_y(h)$
  - however, Euler matrices are non-unique so interpolation could be erroneous
  - extraction could be done using inverse trig (mtrx $\to$ hpr)
  - *def.* gimbal lock: method cannot determine when degree of freedom is lost
- Rotation about Arbitrary Axis
  - let axis $\hat \bold r$, angle $\alpha$
  - find $\bold{s} = \bold{r}$ with smallest component set to 0, and the other two swapped, one of the two negated (this approximates the tangent)
  - $\hat \bold{s} = normalized(\bold{s})$, $\hat\bold t = \hat\bold r \times \hat\bold s$
  - basis $M = \begin{pmatrix}\hat\bold r & \hat\bold s & \hat\bold t\end{pmatrix}$
  - $R_{\hat\bold r}(\alpha) = M^TR_x(\alpha) M$
## Quaternions
- stable and interpolable, but more expensive
- *def.* $\hat q = (\bold q_v, \bold q_w) = \bold i q_x + \bold j q_y + \bold k q_z + q_w$
- where
  - $\bold i^2 = \bold j^2 = \bold k^2 = -1$
  - $\bold{jk = -kj = i}$
  - $\bold{ki = -ik = j}$
  - $\bold{ij = -jk = k}$
- multiplication: $\hat q \hat r = (\bold i q_x + ...)(\bold i r_x + ...) = (\bold q_v \times \bold r_v + r_w \bold q_v + q_w \bold r_v, q_w r_w - \bold q_v \cdot \bold r_v)$
- conjugate: $\hat q^* = (-\bold q_v, q_w)$
  - $\hat q \hat q^* = \hat 1$
- norm: $n(\hat q) = \sqrt{q_x^2 + q_y ^2 + q_z ^ 2 + q_w ^ 2}$
- identity: $\hat 1 = (\bold 0, 1)$
- inverse: $\hat q^{-1} \hat q = \hat q \hat q^{-1} = \hat 1$
  - *thm.*
    - $\hat q \hat q^* = (0, \bold q_w \bold q_w + \bold q_v \cdot \bold q_v) = (0, n(\hat q)^2) = n(\hat q)^2 \hat 1$
    - $$\frac{\hat q \hat q^*}{n(\hat q)^2} = \hat 1$$
    - $$\hat q^{-1} = \frac{\hat q^*}{n(\hat q)^2}$$
- scalar mult.: $\hat s = <\bold 0, s>$, $\hat q \hat s = (s \bold q_v, s \bold q_w)$
- other properties
  - $(\hat q^*)^*=\hat q$
  - $n(\hat q^*) = n(\hat q)$
  - linear: left and right multiplication distribute, as long as on the original side
  - associative
- unit quaternions
  - $n(\hat q) = 1$
  - another representation as $\hat q = (\sin{(\phi)} \bold u_q, \cos{(\phi)})$, with $||\bold u_q|| = 1$
  - note that $\cos{(\phi)}+\bold i \sin{(\phi)} = e^{\bold i \phi}$
  - which means $\hat q = e^{\bold u_q \phi} = \cos{(\phi)} + \sin{(\phi)}\bold u_q * (\bold i, \bold j, \bold k)$
  - following this definition of exponential: $\ln {\hat q} = \bold u_q \phi$
  - power: $\hat q^t = e^{\bold u_q t \phi} = \sin (t\phi)\bold u_q + \cos (t\phi)$
  - $\hat q$ encodes the transform that rotates $2\phi$ radians around $\bold u_q$
    - to apply, use quaternion multiplication $\hat q \bold p \hat q^{-1} = \hat q \bold p \hat q^*$
    - although $\bold p$ is a homogenous vector, treat it as though it is a quaternion
    - for multiple rotations: $(... \hat r \hat q) \bold p (\hat q^* \hat r^* ...)$
  - matrix representation: $\hat q \to M^q$
  - *proc.* spherical linear interpolation
    - let unit quaternions $\hat q$, $\hat r$, $t \in [0, 1]$
    - $s(\hat q, \hat r, t) = (\hat r \hat q^{-1})^t \hat q$
      - implemented in software, it is $s = \frac{\sin(\phi(1 - t))}{\sin \phi}\hat q + \frac{\sin(\phi t)}{\sin \phi} \hat r$, for $\cos \phi = \hat q \cdot \hat r$
    - pro. slerp always traverses the shortest arc around a fix axis at constant speed
    - con. computationally expensive
  - *proc.* spherical cubic interpolation
    - generates smooth spline from multiple $\hat q_i$
    - calculate augmentations $\hat a_i$ and $\hat a_{i+1}$ between $\hat q_i$ and $\hat q_{i+1}$
      - $$\hat a_i = \hat q_i e^{-\frac{\ln(\hat q^{-1}_i \hat q_{i-1}) + \ln(\hat q^{-1}_i \hat q_{i+1})}{4}}$$
      - $$squad(\hat q^i, \hat q^i+1, \hat a^i, \hat a_{i+1}, t) = slerp \begin{pmatrix} slerp(\hat q_i, \hat q_{i+1}, t) \\ slerp(\hat a_i, \hat a_{i+1}, 2t(1-t)) \end{pmatrix}$$
        - $2t(1-t)$ is a downward parabola with intercepts at $x=0, 1$ and $y_{max} = 0.5$
  - *proc.* unit evctor rotation interpolation
    - a rotation from $\hat \bold s$ to $\hat \bold t$ can be represented by...
    - let $\bold u = \hat \bold s \times \hat \bold t$, $\hat\bold u = normalize(\bold u)$
    - let $2\phi$ be the angle between
      - this means $\cos(\phi) = \hat\bold s \cdot \hat\bold t$, $\sin(\phi) = ||\bold u||$
    - $\hat q = (\sin \phi \hat\bold u, \cos\phi) = e^{\hat\bold u \phi}$
      - here, the sines can be eliminated with the half angle formula
      - a matrix representation of this is also possible, but is unstable near $0, 2\pi$ due to trig issues
## Vertex Blending
- *appl.* skin-bone systems (skin mesh deformations based on bone movement)
- *proc.* for $n$ bones with non-negative weights $w_i$ adding up to 1, original skin point $\bold p$ is transformed to point $\bold u$ following $\bold u = \sum_{i=1}^n w_iB_iM_i^{-1} \bold p$
- where
  - $B_i$ is the bone deformation matrix relatiev to the bone's POV
  - $M_i$ is the bone's original basis
- blending can be improved to prevent unwanted deformations
  - *ex.* center of rotation blending: retains local rigidity

## Morphing
- *appl.* smooth transition between key frames of motion capture
- *proc.* vertex correspondence: establish key-point correspondence between subsequent frames, often done just by keeping track of key-point identities
- *proc.* interpolation: (linear) lerp between keypoint states
- morph targets / blend shapes
  - for poses $\mathcal{P_1} ... \mathcal{P_k}$, neutral pose $\mathcal{N}$
  - there are difference poses $\mathcal{D_i} = \mathcal{P_i} - \mathcal{N}$
  - a morphed model for weights $w_1 .. .w_k$ is $\mathcal{M} = \mathcal{N} + \sum_{i=1}^n w_i \mathcal{D}_i$

## Geometry Cache Playback
- find a space-efficient way to store vertex data for video
- optimizations
  1. quantization
  2. spatial predictions + deviations (low entropy for good predictions $\sim$ very sparse derivation data $\to$ utilize sparse data structures)
  3. temporal prediction
     * *ex.* assume items remain in the same motion between frames

## Projection
### Orthographic Projection
- orthographic, canonical $P = \begin{pmatrix} I^2 & ... \\ ... \\ ... & 1\end{pmatrix}$
- ortographic with clipping:
  - represented by $l, r, b, t, n, f$ (left, right, bottom, top, near, far) for the 6 faces of the view volume
    - by convention, $-z$ is into the page, so $n > f$
    - in general, $P=ST$ for some $S$, $T$
    - for OpenGL, $z$ is from $-1$ to $1$:
      - $S_{O[-1, 1]} = S(2<r-l, t-b, f-n>^{-1})$
      - $T_{O[-1, 1]} = T(-\frac{1}{2}<r+l, b+t, n+f>)$
    - to convert back to $z$-inward convention for computer graphics, add a $z$ mirroring transform $M_z$

### Perspective Projection
- $P_p = \begin{pmatrix} I_3 & ... \\ ... & -\frac{1}{d} & 0 \end{pmatrix}$
- asymmetric frustrua: $t \neq -b$ and or $r \neq -l$
- field of view
  - $\phi = 2\tan^{-1}(\frac{w}{2d})$, for
    - $w =$ width perpendicular to sight $d =$ distance from camera
- In general
  - $$P_p = \begin{pmatrix} \frac{2n}{r-l} & 0 & -\frac{r+l}{r-l} & 0 \\ 0 & \frac{2n}{t-b} & -\frac{t+b}{t-b} & 0 \\ 0 & 0 & \frac{f+n}{f-n} & -\frac{2fn}{f-n} \\ 0 & 0 & 1 & 0 \end{pmatrix}$$
  - for infinite far plane ($f \to \infty$), replace third row with $<0, 0, 1, -2n>$
  - $\bold q_{xyzw} = P_p \bold p_{xyzw}$, $\bold q_{xyz} = \bold q_{xyzw} / \bold q_w$
- note that depth of the projected point increases non-linearly
  - evaluate canonical depth (normalized device coordinates' depth) and write to z buffer (depth01)
  - ![](../res/real_time_rendering/100.1.png)

# V. Shading Models
- unit directions
  - $\hat\bold n$ vertex normal
  - $\hat\bold l_i$ vertex to light $i$
  - $\hat\bold v$ vertex to view
- in general, a shading model produces color $\bold c \in [0, 1]^3$
- lit-unlit split:
  - $$c=f_{unlit}(\hat\bold n, \hat\bold v) + \sum_{i} f_{lit}(\bold c_i, \bold c_{surf}, \hat\bold n, \hat\bold{v}, \hat\bold{l}_i)$$
## Lambertian Model
  - $f_{lit} = (\hat\bold l_i \cdot \hat\bold n)^+ \bold c_i * \bold c_{surf}$

## Types of Light
### Directional Light
- $\hat\bold l, \bold c$ constant, no physical location
### Punctual Light
- *def.* has position $\bold p$
1. Point Light:
   * $\hat\bold l = normalize(\bold p - \bold p_0)$
   * $r = ||\bold p - \bold p_0||$
   * distance attenuation: $\bold c(r) = \bold c_{0} att(r)$
     * inverse square attenuation: $att(r) = (r_{0} / r)^2$
     * solving divide by 0
       * $r+\epsilon$ instead for denominator
       * $max(r, r_{min})$ instead for denominator, with $r_{min}$ being physical light size
     * clip to 0 at far distance
       * $\bold c(r) = \bold c_{0} att(r)f_{window}(r)$
       * a popular choice is $f_{window} = (1 - (\frac{r}{r_{max}})^4)^{+^2}$
       * another notation has $f_{dist}(r) = att \circ f_{window}(r)$, named "distance fall-off"

2. Spot Light
    * $\bold c = c_0 f_{dist}(r) f_{dir}(\hat\bold l)$
    * $f_{dir}$ depends on design
      * umbra angle $\theta_u$, penumbra angle $\theta_p$
      * ![](../res/real_time_rendering/115.1.png)
      * $t=(\frac{\cos \theta_s - \cos \theta_u}{cos \theta_p - cos \theta_u})^\mp$
      * *ex.1* $f_{dir}(\hat \bold l) = t^2$
      * *ex.2* $f_{dir}(\hat \bold l) = smoothstep(t)$, where smoothstep is a cubic polynomial lerp with ease-in and ease-out
3. IES Light
    * IES profiles are standards for representing light patterns
4. Other: area, capsule, ...

## Implementing Shading Models
- Frequency of Evaluation
  - once per ever (like constants)
  - once per a while (aka. amortized): values changing slowly, like world lighting data
  - once per frame
  - ...
  - corresponding to shader stages
    1. vertex shader: per pre-tessellation vertex
    2. hull shader: per surface patch
    3. domain shader: per post-tessellation vertex
    4. geometry shader: per primitive
    5. pixel (frag) shader: per pixel
  - most actual evaluations are per pixel
- difference between per-pixel shading and per-vertex shading
  - for dense meshes, differences are small
  - for sparse meshes, pixel is more accurate
    - for this reason, vertex shaders are used mostly for non-shading operations
- for pixel shading:
  - interpolating normal vectors result in non-unit lengths, needing re-normalization
  - $\hat\bold l$ and $\hat\bold v$ are calculated from interpolated $\bold p$, instead of being interpolated themselves
- vertex shader space:
  - the vertex shader transforms entities to an appropritate coordinate system
  - using world space avoids the need to transform light positions, which helps when there are lots of lights
  - using camera space helps calculate the view vector, and may improve precision
- flat shading: done in vertex shader by disabling interpolation

## Example Implementation
### Shader Inputs and Outputs
```glsl
// varying inputs are marked 'in'
in vec3 vPos;
in vec3 vColor;
out vec4 outColor;

// exerpt of uniform inputs 
// ...
struct Light {
    // vec4 used for standard std140 data layout
    vec4 position;
    vec4 color;
};

uniform LightUBlock { Light uLights[MAXLIGHTS]; }; // at compile, MAXLIGHTS is replaced in a C-directive-like manner
uniform uint uLightCount;

// ...
```

### Pixel Shader
```glsl
vec3 lit(vec3 l, vec3 n, vec3 v) { ... }
void main() {
    // calculate n, v from world coords (vNormal, vPos)
    // relative to uEyePosition
    outColor = vec4(uFUnlit, 1.0); //initialize as unlit
    for (uint i = 0u; i < uLightCount; i++) {
        // calculate l
        // evaluate f_lit and add to outColor's rgb. this includes
        //  1. getting light intensity by projecting l onto n
        //  2. multiplying intensity to result of lit(l, n, v),
        //     then to uLights[i]'s color
    }
}
```

### Vertex Shader
```glsl
// same uniform definitions
layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
out vec3 vPos;
out vec3 vNormal;

void main() {
  vec4 worldPosition = uModel * position; // world space
  vPos = worldPosition.xyz;
  vNormal = (uModel * normal).xyz; //normal vector does not need normalization at this stage
  gl_Position = viewProj * worldPosition; // clip space
}
```

### Material Systems
- materials are more artist-facing than shaders
- implemented with shaders
- *def.* parametrization: materials with specific parameters are instantiated from templates
  - certain engines like Unreal allows for multiple levels of template hierarchies
  - parameters may also be passed at runtime as uniform inputs
- often, a material system divides various shader functions into separate elements, and controls the mixing of these elements
  - use cases include
    - separating surface shading (material-based) from geometric processing (mesh-based)
    - compositing operations, especially for mobile GPU
    - increase reusability of one shader implementation
    - selectable material features
    - light interactions
- graphics API usually doesn't provide control on post-compilation code, so the modular design can only be achieved at source code level
  - this is often done through C-style directives such as `include`, `if` and `define`
  - before, these modular features afe often handled at compile time due to bad dynamic branching performance
  - modern GPUs handle dynamic branching better, so usually things like light count is handled dynamically
    - it is still common practice to compile separate variants for cases where there is 0 lights, and cases where there are >0 lights (unlit vs. lit)
    - for >0 lights, light type and count are both handled dynamically
    - in Unity, some built-in shaders can have more than 100B variants, so only the ones used are compiled
- techniques for material systems
  - code reuse via `include`ing shared files
  - subtractive use of **supershader/ubershader**s
  - additive use of shared code, commonly via graph nodes (this is similar to `include` but with more structure)
  - template-based: adding another layer of structure on top of the additive strategy, such as Unreal's support for different "domains"
- material systems often separate inputs on different update frequencies for optimization

## Aliasing & Anti-Aliasing
- aliasing is caused by binary presence of a fragment
  - it is either there or not there, causing clear and jagged borders
### Sampling & Filtering Theory
- works analogous in one and two dimensions
- smooth signals are *sampled* at discrete levels, and the samples are used to reconstruct the signal via *filtering*
- aliasing occurs when signal is sampled at low frequency
  - **sampling theorem**: to prevent aliasing, the sampling frequency needs to be >2x the targeted maximum frequency
  - the signal has to be smooth enough relative to the spacnuig between neighboring samples
  - note that solid edges have infinite frequency, so sampling at higher frequencies won't really help
  - however, this theorem does apply for things like surface texture
    - sometimes, there might be too many texture samples per pixel, causing the pixel sampling frequency to be too low for the texture's frequency
    - in that case, techniques are used to "band-limit" the texture (i.e. lower the maximum frequency of the texture so that the per-pixel sampling frequency is adequate)
- reconstruction techniques
  - uses filters
    - box filter (1 between -1 and 1, otherwise 0)
    - tent filter ((-1, 0) -> (0, 1) -> (1, 0))
    - sinc filter (smoother tent, with flat top at (1, 0))
      - aka. low pass filter, $(sin \pi x)/\pi x$
      - a box filter in the frequency domain, covoluted with the the sampled signal
      - removes all frequencies more than half of the sampling frequency
  - for a filter $f$ and samples $s_i$
    - create $f_i$ where the maximum is the value of $s_i$, and the center is the position of $s_i$
    - the reconstructed $\hat s = \sum f_i$
  - most practical applications choose a middleground between sinc and box/tent
    - sinc is the theoretical maximum
    - box/tent are cheaper
    - typically, a group called Gaussian filter are used, named as such because they resemble gaussian curves
    - obtains a continuous signal
- resampling
  - magnify/minify a sampled signal for display
  - magnification/upsampling samples the reconstructed sample at a higher frequency
  - to minify, simply resampling at a different frequency won't work
    - the signal under the original reconstruction (say with sampling frequency 1) contains frequencies higher than required
    - a wider filter $sinc(x/a)$, for $a>1$ being the interval between new samples) must be applied to the original sampled points to created a "blurred" signal
    - the blurred signal is then resampled at lower frequencies 

- techniques for antialiasing
  - *def.* **screen based** means the techniques only operate on the output of the pipeline, and not at earlier stages
  - more samples per pixel
    - spread out more samples per pixel
    - for 1 sample, whether the fragment crosses the center of the pixel dictates whether it is displayed
    - for >1 samples, the number of times the fragment crosses the sample contributes to its opacity
    - a common practice is to weigh different samples
      - $p(x, y) = \sum w_i c(i, x, y)$ for samples 1 to $n$
      - $c$ can be seen as a composite $c'(s_i(x, y))$ where $c'$ is the sampling action and $s$ shifts the original point based on which sample it is
      - note that $w_i$ sums up to 1, by default $w_i = 1/n$
  - **supersampling/oversampling**
    - for example, full-scene antialiasing (FSAA) 2x requires doubling the resolution and then sampling each $2 \times 2$ neighborhood as one actual pixel via a box filter
    - very costly (buffers are much larger), but simple implementation
  - **accumulation buffer**
    - same resolution buffer but more bits of color per channel used to store separate results
    - rarely used because each sample is rendered in its own pass per frame (the more samples, the more passes per frame)
  - other techniques must accompany
    - making light and shadow less sharp to avoid high frequencies
    - scaling up objects to make sure they take up at least 1 pixel
  - **multisampling antialiasing** (MSAA)
    - optimizes by computing surface shade once per pixel, sharing among samples
    - *centroid sampling*
      - pick center the fragment crosses the center, otherwise pick a better spot
    - only depth and stencil are "truly" supersampled
  - **enhanced quality antialising** (EQAA)
    - memory optimization on top of MSAA, since the lower buffer size the faster
    - has settings like "2f4x"
      - 2 stands for the number of fragments shaded per pixel, say for fragments A and B
      - 4x stands for number of supersamples per pixel
      - the result is a coverage table of size 4, each entry with a 1 bit index for indicating whether the corresponding supersample is an A or B
      - usually there is very few cases where there are more than 3 fragments per pixel with drastically different shading
  - the **resolve** operation is used after supersampling to determine what each pixel actually looks like
    - various different features were introduced and then supplanted that supports choosing different filters in the resolve operation
    - modern GPUs often support emulating resolve in pixel/compute shaders by accessing MSAA samples, although with high cost
    - issue with HDR images
  - **temporal antialiasing**, **multi frame antialiasing** (MFAA)
    - blend in previous images, often 2 or 4 frames, with exponentially decaying weight
      - significantly cheaper than supersampling
    - "shimmering" artifacts on static scenes when frames are not weighted equally
    - "ghosting" artifacts (trailing image) when camera moves rapidly
      - a solution is to only perform MFAA on slow objects
      - a velocity buffer is used for such decisions
    - a variety of fixes have been proposed
  - sampling patterns
    - experiments have been conducted for effects of different sampling patterns on visual comfort
    - avoid near-vertical, near-horizontal, and 45 degrees
    - development of rotated grid supersampling (RGSS) that looks more comfortable but usually creating a lot more even hits than other patterns, nearly ignoring the odd levels (for example, 1 and 3 in a 2x2 RGSS)
    - "N-Rooks" sampling: everything lines up on a diagonal, doesn't perform well for cases just covering the diagonal
    - **stratified sampling**: make sure samples are uniform across the pixel
      - usually these sampling patterns are hard wired into the GPU
    - **stochastic sampling**
      - more randomized samples
      - yields noise for repetitive aliasing effects
      - artifacts when patterns line up with the repeated sampling patterns
      - interleaved sampling: different pattern per pixel, to avoid repetition
    - **quincunx sampling**
      - a supersample with 4 on the corner (weighing 1/8) and 1 on the center (weighing 1/2)
      - approximates tent filter
      - shares samples on the edge, thereby reducing cost
      - quicunx temporal AA
        - shift consecutive frames back and forth by half a pixel and take the average
        - the previous frame provides pixel corners, and the current frame provides the pixel center
      - optimized as FLIPQUAD (RGSS) to improve visual comfort, while keeping the on-border sharing between adjacent pixels
### Morphological Methods
- aliasing usually occurs atedges and sharp shadows/highlights
- use post-processing to determine where edges are and blur it in the right orientation
- several techniques require additional buffers to be populated to aid edge detection
- generic techniques only take the color buffer
- not universal, especially for text quality
  - often used for low memory/computational power devices
- popular examples are fast approximate antialiasing (FXAA) and subpixel morphological antialiasing (SMAA), both only taking color input

## Transparency
- different resulting effects
  - **light-based effects**: the semitransparent object changes light and causes other objects to look different
  - **view-based effects**: the semitransparent object itself appears diffeerent
- the following considers semitransparent objects to *attenuate* for the color objects behind it
  - this does not include more advanced effects like frosted glass, refraction, etc.
### Screen-door Transparency
  - render a checkerboard pattern where every few pixels is the semitransparent fragment itself, and the rest are the stuff behind it
  - **stochastic transparency**
    - combine screen-door with stochastic sampling, the result is usually noisy until a large samples per pixel, but the advantage is no need for blending
### Alpha Blending
- *def.* **alpha** may represent opacity, coverage, or both
  - coverage stands for the coverage within a pixel for MSAA-like schemes
  - for example, a 75% coverage with 0.1 opacity produces 0.075 alpha
### Blending ORder
- **over operator**
  - $lerp(c_s, c_d, \alpha)$, for source color $c_s$ (of the semitransparent fragment), destination color $c_d$ (of the scene below)
- **additive blending**
  - $\alpha c_s + c_d$, usually for glowing effects
- the over operator usually requires back-to-front render order
  - this is commonly done by sorting by centroid depth per object (note that this is not per-fragment)
    - will produce incorrect results for inter-penetrating meshes
    - centroids are only approximations
  - z-buffer replacement
    - turn off to make sure all transparent objects appear "in some form" instead of disappaering entirely
- **under operator**
  - a derivative of the over operator to make sure front and back blending yields the same result
  - $c = lerp(c_d, \alpha_s c_s, \alpha_d)$
  - $\alpha = lerp(1, \alpha_s, \alpha_d)$
  - note that $\alpha_d$ stands for the alpha of the surface below
  - the alpha calculation is order-independent
  - the color calculation is basically the over operator but with source and destination swapped
  - results of under are often put in another buffer and then merged with the scene using over (using the resultant $c$ and $\alpha$ from under)

### Order Independent Transparency
- **depth peeling**: 
  - no sorting, but multiple buffers and passes
  - each pass is a depth peel pass for rendering different transparent entities
  - *ex.* 1st closest transparent layer per pixel, 2nd, 3rd, etc.
    - the first is usually just the front facing fragments
    - the subsequent layers depend on the actual depth relations of the meshes involved
  - a challenge is knowing how many passes are "enough"
    - hardware solution: pixel draw counter
  - back to front peeling
    - blends each layer immediately, no need for extra alpha channel
  - front to back peeling
    - can stop the process when the pixel rendered falls below some threshold, without impacting visuals significantly (since the top layers are already rendered)
    - by definition, each layer has to have equal or less pixels than the prior
  - slow
- **a-buffer**
  - each pixel stores a list of all fragments that fully/partially covers it
  - culling can happen within the linked list
  - several newer features of GPUs help map this algorithm to actual devices
    - done via atomic operations and pointerse
  - the storage is not preevaluated as overlapping semitransparent fragments add to the count
  - **multi-layer alpha blending**
    - uses pixel synchroniuzation
    - degrades gracefully when memory runs out
    - later transported to DirectX rasterizer order views (see chapter 3)
  - **k-buffer**
    - first k visible layers are saved and sorted, the reset are discareded
    - alternatively, use a weighted average to account for deeper layers
    - the visual performance increases for lower opacities, because the less opaque the less viewing order matters
    - weighted sum: $c = \sum(\alpha_i c_i) + c_d(1 - \sum \alpha_i)$
      - the first term may saturate
    - weighted average:
      - $c_{sum} = \sum \alpha_i c_i$
      - $c_{wavg}=c_{sum}/\sum \alpha_i$
      - $u = (1 - \alpha_{avg})^n$ estimated visibility of the destination
      - $c = lerp(c_{wavg}, c_d, u)$
### Premultiplied Alphas & Compositing
- blending multiple photographs/synthetic renderings together\
  - each layer is RGBA, with the alpha channel called **matte**
  - premultiplying means to use $c'_s = c_s \alpha_s$ instead of using a semitransparent layer
    - i.e. the RGB channels account for emission instead of surface color
  - $c = c'_s + (1 - \alpha_s)c_d$
  - this effectively optimizes the over operator
  - the matte layer often also describes coverage
- **unmultiplied alpha**
  - RGB is detached from A and needs to be multiplied
- premultiplying works with filtering and blending
  - the problem is unmultipiled alphas don't work well with interpolation
- premultiplying reduces image precision, especially with lower alphas on darker colors due to the multiplication
- chroma-keying
  - drawback is that the alpha channel is usually just 1 or 0
  - determined from certain RGB values/ranges

## Display Encoding
- shader outputs are typically linear
- **gamma correction**
  - fixes various artifacts by raising the shader output (in range [0, 1]) by 1/2.2
  - humans sense different light intensities following an inverse squared pattern
    - starting with CRT screens, intensity is essentially voltage (pixel value) squared, which resembled the inverse of of light perception
    - the result is the two relations cancel out to form a perceptually uniform color encoding
      - that is, for some $\Delta$, no matter what $c$ is, $c+\Delta$ and $c$ has similar perceived differences
    - LCD screens manually covert to this power relation to mimic CRT
      - display transfer function/electrical optical transfer function (EOTF)k, as part of hardware
  - minimizes banding artifacts
    - banding occurs when color spaces are not perceptually uniform, such that certain color ranges preserve more detail, while others (particularly darker sections) look more quantized
  - the goal of gamma correction is to cancel out the EOTF by converting linear shader output to radiance
    - this is usually done in the GPU to fit the sRGB standard
  - to correctly alpha blend, colors must be converted back to linear first, blended, and then encoded again
  - post-process before gamma correction
> the textbook outlines several more scenarios for paying attention to gamma correction
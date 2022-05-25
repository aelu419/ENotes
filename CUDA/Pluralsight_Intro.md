- [Architecture & Tools](#architecture--tools)
- [CUDA C](#cuda-c)
  - [Sample Loop Parallelization](#sample-loop-parallelization)
  - [Location Qualifiers](#location-qualifiers)
  - [Execution Model](#execution-model)
- [Parallel Programming Patterns](#parallel-programming-patterns)
  - [Element Addressing](#element-addressing)
  - [Mapping](#mapping)
  - [Gather](#gather)
  - [Scatter](#scatter)
  - [Reduce](#reduce)
  - [Scan](#scan)

# Architecture & Tools
- *def.* **shader**: small program on the GPU that runs in a highly parallel manner
  - roughly C-like, not suitable for general purpose
- GPGPU Frameworks
  - Compute Unified Driver Architecture (CUDA)
    - extension to programming languages (C/C++)
    - wrappers for other languages
  - Open Computing Language (OpenCL)
    - general purpose computation on ATI devices
  - C++ Accelerated Massive Programming (AMP)
    - both ATI, NV, and CPU's, but only Microsoft
  - in NV terminology, a computational device within the GPGPU framework is fcalled a Streaming Multiprocessor (SM)
- Hardware
  - GPU contains
    - Streaming Multiprocessor (an array  of) contains
      - CUDA Cores (aka. Streaming Processor, Shader Unit; an array of) accesses
      - Shared Memory, Constant Memory,  Texture Memory
      - Registers
    - Device memory
  - performance: usually look at FLOPS (pay attension to single/double percision)
- Software
  - CUDA
  - NSight (for debugging, profiling; server-client structure)
  - Cuda C
    - certain ``#include``able header files provided by CUDA
    - Compiled with NVidia Cuda Compiler (NVCC) 
      - uses host C/C++ compiler, not a compiler by itself
      - splits code into GPU and non-GPU parts (only the GPU part is run on the GPU)
      - accepts project-defined GPU settings
      - translates code from CUDA C to Parallel Thread Execution code (PTX) (then turned to binary code by Graphics Driver)
        - similar to bytecode
# CUDA C
## Sample Loop Parallelization
- ```c++
  // c is a out array
  void addArrays(int* a, int* b, int* c, int cnt) {
      for (int i = 0;  i < cnt; i++) c[i] = a[i] + b[i];
  }
  ```
- ```c++
  #include "cuda_runtime.h"
  #include "device_launch_parameters.h"
  #include "stdio.h"

  // defines a kernel to be ran on a shader unit
  __global__  void addArrays(int *a, int *b, int *c) {
      int i = threadIdx.x;

  }

  void main() {

      const int count = 5;
      const int size = count * sizeof(int);

      int ha[] {1, 3, 5, 7, 9}; // host a
      int hb[] {2, 4, 6, 8, 10}; // host b
      int hc[count];

      // transfer data to CUDA memory
      int *da, *db, *dc;
      cudaMalloc(&da, size);
      cudaMalloc(&db, size);
      cudaMalloc(&dc, size);

      cudaMemcpy(da, ha, size, cudaMemcpyKind::cudaMemcpyHostToDevice); // can be shortened to just cudaMemcpyHostToDevice

      //  carry out operation on 1 block and count threads
      addArrays<<<1, count>>>(da, db, dc);

      cudaMemcpy(hc, dc, size, cudaMemcpyKind::cudaMemcpyDeviceToHost);
  }
  ```

## Location Qualifiers
- ``__global__`` invoked in CPU, ran in GPU
  - executed with ``<<<dim3>>>(args)``
    - ``<<<dim3>>>`` specifies grid $\to$ block $\to$ thread dimensions
    - programs
      - $(a \times b \times c) \to (x \times y \times z)$
      - leaving certain parameters empty automatically converts them to 1
        - <<<1, count>>> is a (1, 1, 1) \times (count, 1, 1) thread program
- ``__device__``: runs on the GPU, called from the GPU
- ``__host__``: runs on the CPU, called from the GPU
- qualifiers can be mixed, and the specific use is dependent on the current context (i.e. if it is currently called from the CPU, if so run on the CPU)

## Execution Model
- thread blocks are scheduled to run on SM, one per SM at a time
- thread block divided to WARPs
- WARPSs contain various threads
- each thread has access to
  - execution parameters
  - current position in the thread block
      - limitations on grid & block size: ``MAX_BLOCK_DIM_X``, etc.
      - ``gridDim`` size of the grid
      - ``blockDim`` size of thread block
      - ``blockIdx`` position of current thread in thread block, size of thread block
      - ``threadIdx`` position of thread in thread block
- cuda functions throws ``cudaError_t`` via return
  - check against ``cudaSuccess``
  - get description with ``cudaGetErrorString()``
- other demo functions
  - ``cudaGetDeviceCount(*int)``
  - ``cudaGetDeviceProperties(*cudaDeviceProp,  int count)``
    - index (in case of multiple devices), compute capability, thread count, etc
  - ``curand.h`` generate random floats
    - ```c++
      curandGenerator_t gen;
      curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_MTGP32);
      curandSetPseudoRandomGeneratorSeed(gen, time(0)); // current time

      const int count = 123456;
      const int size = count * sizeof(float);
      float *d;

      cudaMalloc(&d, size);
      curandGenerateUniform(gen, d, count);
      ```

# Parallel Programming Patterns
## Element Addressing
- typically, both the grid and thread addresses are 3D, adding up to a 6D coordinate in total
  - however,  problem spaces are typically 1D
## Mapping
- apply the same function to each entry in a collection
- *ex.*
  setup  in ``main``
  ```c++
    const int count = 123456;
    const int size = count * sizeof(float);
    // ... generate d* -> array of <count> random floats
    float h[count]; // allocate array of size count
    dim3 block(8, 8, 8);
    dim3 grid(16, 16); // implicitely 16, 16, 1
    // the product of all numbers above is above <count>, which makes the allocated thread count sufficient
  ```
  operation outside ``main``
  ```c++
  __global__ void addTen(float* d, int count) {
    int threadPerBlock = blockDim.x * blockDim.y * blockDim.z;
    int threadPosInBlock = threadIdx.x + blockDim.x * threadIdx.y + blockDim.x * blockDim.y * threadIdx.z;
    int blockPosInGrid = blockIdx.x + gridDim.x * blockIdx.y + gridDim.x * gridDim.y * blockIdx.z;

    int tid = blockPosInGrid * threadsPerBlock + threadPosInBlock;

    if (tid < count) {
      // the operation itself
      d[tid] = d[tid]+10;
    }
  }
  ```
  usage in ``main``
  ```c++
    addTen<<<grid, block>>>(d, count);
    cudaMemcpy(h, d, size, cudaMemcpyDeviceToHost);
    cudaFree(d);
    // use the new values by addressing h[i]
  ```
## Gather
- multiple inputs in an orderly manner for the same function
- *ex.*
  - problem: Black Scholes Formula with 5 parameters and 2 outputs
  - ```c++
    // initialize count and then size
    // ...
    // initialize 5 pointers for arrays of args
    float *args[5];
    // initialize random number generator
    //...
    for (int i = 0; i < 5; i++) {
      cudaMalloc(&args[i], size);
      curandGenerateUniform(gen, args[i], count); // fill arg array with randon numbers
    }

    float *dc, *dp; // initialize 2 output arrays
    cudaMalloc(&dc, size);
    cudaMalloc(&dp, size);
    
    price<<<1, count>>>(args[0], args[1], args[2], args[3], args[4], dc, dp); // for the sake of simplicity, here only 1 
    ```
  - ```c++
    // to simplify actual logic, this function is a gateway to the actual computing function
    __global__ void price(float* k, float* s, float* t, float* r, float* v, float*c, float* p) {
      int idx = threadIdx.x; // more advanced argument addressing could be done here
      price(k[idx], ..., &p[idx]);
    }
    ```
  - ```c++
    __device__ __host__ void price(float k, ..., float* p) {
      // compute using math.h functions such as sqrtf and etc.
      ...
      *c = N(d1) * s - N(d2)*kert; // here, N is an integral that requires cuda libraries to run
                                   // note that c is just a intermediate variable, but it is also allocated outside of the function itself as an array
      *p = ... // note that p is the final return variable
    }
    ```

  - ```c++
    #define _USE_MATH_DEFINES
    __device__ __host__ __inline__ float N(float x) {
      return 0.5 + 0.5 * erf(x * M_SQRT1_2);
    }
    ```

## Scatter
- opposite of gather: one input multiple outputs

## Reduce
- go from larger buffer to smaller buffer over time (iterations)
- for example, sum $N$ elements, initialize $T = N / 2$
  - for each iteration, $T \leftarrow T / 2$, sum every $2^i$'th value in the buffer
  - *ex.*
    ```c++
    // with one single TB handling the entire thread count
    __global__ void sumSingleBlock(int *d) {
      int tid = threadIdx.x;

      // cut down step size per iteration
      for(int tc = blockDim.x, stepsize = 1; tc > 0; tc /= 2, stepSize *= 2) { // the original code uses bit shifting
        if (tid < tc) {
          int pa = tid * stepSize * 2; // head
          int pb = pa + stepSize; // next after head
          d[pa] += d[pb]
        }
      }
    }
    ```
  - after $\lceil\log(N)\rceil$ iterations, the first item in the buffer is the sum of the entire buffer

## Scan
- $N-1$ threads (at first iteration)
- per iteration, step size doubles, number of threads -= step size
  - step size: 1, 2, 4, 8, ...
  - number of threads: 4, 3, 1 if starting at 4
  - from ``i``, ``arr[i]`` $\leftarrow$ ``f(arr[i - step_size], arr[i])``
  - terminate upon $T < 1$, that is, the only operation on the last iteration is ``arr[N-1]`` $\leftarrow$ ``f(arr[0], arr[N-1])``
- upon completion, ``arr[i]`` = ``f(arr[0], ..., arr[i])``
  - ex. let $f = +$, ``arr = {1, 2, 3, 4, 5}``, ``scan(arr, f) = {1, 3, 6, 10, 15}``
- another way to view it is that the result ``r[i]`` broadens its perspective to the left (decrement) direction per step
  - at iteration 0, ``r[N-1]`` "looks at" itself and the element 1 to the left
  - at iteration 1, itself and 2 to the left
  - ...
- ```c++
  // with 1 TB handling the entire thread count
  __global__ void runningSum(int* d) {

    int threads = blockDim.x;
    int tid = threadIdx.x;
    for (int tc = threads; tc > 0;) {
      if (tid < tc) {
        d[tid+step] += d[tid];
      }
      tc -= step;
    }

  }
  ```
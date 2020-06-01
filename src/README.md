# VCL

- [Introduction](#Introduction)
- [Compile the library](#Compilation)
- [Basic usage of VCL structures](#Usage)
  - [Vec](#vec)
  - [Mat](#mat)
  - [Transformations](#Transformations)
  - [Buffers](#Buffers)


<a name="Introduction"></a>
## Introduction


VCL - Visual Computing Library - is a simple lightweight library on top of OpenGL provided to ease learning of 3D programming while avoiding re-coding everything from scratch (matrices, mesh structures, etc.). The library provides several helper structure and functions to set up 3D scene with interactive and/or animated elements.


The objective of the library is to be simple as simple as possible to read and use.
The majority of VCL structures and functions are aimed to be minimalistic without hidden states. The code remains fully compatible with direct raw OpenGL calls and user-defined shaders. The animation loop itself is entirely defined by the user.


The code contains two main parts:
* The VCL library itself in `vcl/` directory - contains the helper functions and structures
* Some example 3D scenes in `scenes/` directory. Each scene is fully defined in its subdirectory, and the switch between different scene is set using a keyword defined in `scene/current_scene.hpp` file.
 * Link to [INF443 scenes exercises](https://imagecomputing.net/damien.rohmer/teaching/2019_2020/semester_2/INF443_graphique_3D/TD/02_graphique_3D/content/000_introduction/index.html)

<br>

<a name="Compilation"></a>
## Compile the library



The compilation can be done either using the provided
* **Makefile**: Linux/MacOS only 
* **CMakeLists.txt** using CMake (Linux/MacOS/Windows)


The library has one external dependency: [GLFW](https://www.glfw.org/) which can be installed through standard packages in Linux/MacOS (see the provided detailed tutorials).



* [Command lines to compile in Linux/MacOS](doc/compilation.md#command_line)
* **Detailed tutorials** to set up your system and compile on
  * [Linux/Ubuntu](doc/compilation.md#Ubuntu)
  * [MacOS](doc/compilation.md#MacOS)
  * [Windows](doc/visual_studio.md)



You may edit the code using
* QtCreator on Linux/MacOS: [Setting up and using QtCreator](doc/qtcreator.md)
* Visual Studio on Windows

<br>

<a name="Usage"></a>
## Basic usage of VCL structures

Note that a _still incomplete_ [Doxygen documentation is available online](https://imagecomputing.net/damien.rohmer/teaching/general/vcl_doc/html/).

_You may also generate this documentation locally in calling Doxygen on the file `doc/config_doxygen`._


<a name="vec"></a>
### vec

Basic structure and functions associated to 2/3/4D vectors are provided as `vec2`, `vec3` and `vec4` (following mostly GLSL naming convention).

`vec3` (and similarily with `vec2` and `vec4`) are lightweight structure storing 3 floating values (x,y,z). `vec3` are used throughout the scenes to define typically 3D vectors and positions coordinates.

```c++
// Declaring 2D/3D/4D vectors
vec2 p0 = {1.1f, 2.0f};
vec3 p1 = {1.1f, 2.0f, -2.5f};
vec4 p2 = {1.1f, 2.0f, -2.5f, 8.1f};

vec3 p3; // Default values initialized at (0,0,0)

// alternative declaration: constructor call
vec3 p4 = vec3(1.1f, 2.0f, -2.5f);
```

Components can be accessed via named (.x, .y, .z) or indexed ([0], [1], [2]) syntax.

```c++
vec3 p = {1.1f, 2.5f, -2.0f};
p.x = 0.5f;   // Access component as .x, .y, .z
p[1] = -2.5f; // Access component as [0]/[1]/[2]

// Check equality between two vec3 using is_equal(vec3,vec3) function
assert( is_equal(p, {0.5f, -2.5f, -2.0f}) );
```

vec3 (and all other structures) can be safely copied (no pointer or references involved).

```c++
vec3 p0 = {1.1f, 2.5f, -2.0f};
vec3 p1 = {5.0f, 1.1f,  3.0f};

// Copy p1.x/y/z = p0.x/y/z
p1 = p0;

p1.x = -12.0f;

// modifying p1 doesn't impact p0
assert( is_equal(p0, {  1.1f, 2.5f, -2.0f}) );
assert( is_equal(p1, {-12.0f, 2.5f, -2.0f}) );
```

Standard vector operators +-*/ are directly available

```c++
vec3 p = {1.0f, -1.5f, 2.0f};
p = 4 * p;           // p = {4,-6,8}
p = p / 2;           // p = {2,-3,4}
p = p + vec3(1,1,1); // p = {3,-2,5}
p = -p;              // p = {-3,2,-5}

// Operators can be chained
vec3 p2 = ( 2*p + vec3(1,0,2)/2.0f ) / 1.2f;

// Display the coordinates
std::cout<< p << std::endl;
```

As well as helping mathematical functions

```c++
vec3 a = {1,2,3};
vec3 b = {1,2,-1};

vec3  c = a * b;        // pointwise multiplication c = {1,4,-3}
float d = dot(a, b);    // dot product a.b = 2
vec3  e = cross(a,b);   // cross product a x b = {-8,4,0}
float f = norm(a);      // norm ||a|| = sqrt(dot(a,a))
vec3  g = normalize(a); // return the unit norm vector a / ||a||
```

<a name="mat"></a>
### mat

Similarily to vectors, the library provide `mat2`, `mat3` and `mat4` structure (also following GLSL naming convention). 

```c++
// Direct initialization
mat3 A = { 1.1f, 2.5f, 2.0f,
          -2.1f, 4.1f, 1.5f,
            3.0f, 1.0f, 3.5f};

// Display matrix components
std::cout<< A <<std::endl;

// Matrix-vector product
vec3 x = {1,2,3};
vec3 y = A * x;   // = {12.1, 10.6, 15.5}


// Matrix component access
A(0,0) =  2.0f;
A(1,0) =  3.0f;
A(0,1) = -1.0f;
// A = { 2.0f, -1.0f, 2.0f,
//       3.0f,  4.1f, 1.5f,
//       3.0f,  1.0f, 3.5f};

mat3 B; // default initialization as matrix identity

// Matrix product
mat3 C = A * B;
```

As well as several helping function

```c++
// Helper function
mat3 At = transpose(A); // matrix transpose
mat3 iA = inverse(A);   // matrix inverse: A*iA = identity
float d = det(A);       // matrix determinant = 13.1

// Access to row and column
vec3 c0 = A.col(0); // first column = {2,3,3}
vec3 r1 = A.row(1); // second row   = {3, 4.1, 1.5}
// Indexing matrix as a contiguous vector
// A[0] == 2.0f
// A[1] == -1.0f
// ...
// A[8] == 3.5f
```

<a name="Transformations"></a>
### Transformations

`mat3` can be used to store linear transformation (such as rotation), while `mat4` can store general affine transform.

Rotations in 3D can be defined using the two helper functions

```c++
mat3 rotation_from_axis_angle_mat3(const vec3& axis, float angle);
mat3 rotation_between_vector_mat3(const vec3& a, const vec3& b);
```

Example of usage
```c++
// Rotation of pi/4 around the y-axis
mat3 R1 = rotation_from_axis_angle_mat3({0,1,0}, 3.14f/4);
// Rotation of pi/6 around some arbitrary axis (1,5,-2)/||(1,5,-2)||
mat3 R2 = rotation_from_axis_angle_mat3({1,5,-2}, 3.14f/6);

// Two arbitrary axis (scaled to have a unit norm)
vec3 a = normalize(vec3{1,5,-2});
vec3 b = normalize(vec3{-2,1.5,3});
// Rotation transforming a into b: R3 a = b
mat3 R3 = rotation_between_vector_mat3(a, b);
```

`mat4` provide helper tools to transform linear and translation component into a 4x4 matrix corresponding to the affine transform

```c++
// A 3x3 matrix
mat3 R = rotation_from_axis_angle_mat3({0,1,0}, 3.14f/4);
// A translation vector
vec3 t = {1,-2,3};
// Generate the 4x4 matrix T such that
// T = (         |    )
//     (    R    | t  )
//     ( ________|____)
//     ( 0  0  0 | 1  )
mat4 T = mat4::from_mat3_vec3(R, t);
```

Other helper functions are provided to easy generate special matrix corresponding to scaling, translation, etc.

```c++
/** Generate identity matrix */
static mat4 identity();
/** Matrix filled with zeros */
static mat4 zero();
/** Generate standard OpenGL-type perspective matrix */
static mat4 perspective(float angle_of_view, float image_aspect, float z_near, float z_far);

// Matrices related to transformations:
static mat4 from_scaling(float s);
static mat4 from_scaling(const vcl::vec3& s);
static mat4 from_mat3(const vcl::mat3& m);
static mat4 from_translation(const vcl::vec3& t);
```

<a name="Buffers"></a>
### Buffers

#### Dynamic 1D buffer

The structure `buffer<type>` is proposed at your convenience to store arbitrary number and numerical data contiguously in memory. `buffer` is only a special case of `std::vector` associated to an extra set of convenient functionalities for numerical vectors such as operators +-*/, std::cout, as well as strong bound checking.

```c++
// Buffer initialization
buffer<float> a = {1.1f, 2.5f, 8.0f, 4.0f};

// default initialization as an empty vector
buffer<float> b;

// Buffer can be resized at any time
b.resize(45); // b.size()==45
b.fill(5.0f); // fill all 45 elements with the number 5

// Add an element at the back
a.push_back(2.4f);

// Check equality
assert( is_equal(a, {1.1f, 2.5f, 8.0f, 4.0f, 2.4f}) );

// Display all elements
std::cout<< a <<std::endl;

// Getter/Setter similar to std::vector
a[2] = a[0] + 1;
// Calling a[5] would generate a run time error

// Apply operators on the entire buffer
a = 2.0f * a;
a = a + 2.5f; // add 2.5f to all elements of a
float avg = average(a); // get averaged value of the buffer
```

`buffer` can be used with `vec3` to conveniently store coordinates. Note that all floating values of the buffer are guaranteed to be contiguous in memory.

```c++
buffer<vec3> a = { {1,0,0}, {2,1,3} };
a.push_back({0,1,2});

a[0] = {0,0,1}; // Accessing an element as a vec3
a[0].x = 1.1f;  // x coordinate of the first element

// Display all coordinates
std::cout<< a <<std::endl;

// numerical operations on the entire buffer
a = 2.0f*a;
a = a + vec3(1,1,0); // (1,1,0) is added to every element

buffer<vec3> b;
b.resize(3);
b.fill({0,1,0});

// componentwise sum: for all i, a[i] = a[i] + b[i]
a = a + b;
// componentwise product: for all i, a[i] = a[i] + b[i]
a = a * b;
```

#### 2D/3D buffers

Buffers organized as 2D / 3D grid are also provided for convenience as `buffer2D` and `buffer3D`.

These buffer are internally similar to 1D one, and ensure contiguity of elements, but provide accessor using (x,y) notations and handle bounds checking.

Example of a buffer2D containing vec3 - example of application: grid of positions.

```c++
// Initialize a grid of size 2x3
// Each element of the grid contains a vec3
buffer2D<vec3> grid(2,3);

// Fill the entire grid with (1,1,0)
grid.fill({1,1,0});

// Set grid each grid element using (x,y) indexed notation
grid(0,0)={0,0,0}; grid(1,0)={0,0,1};
grid(0,1)={0,1,0}; grid(1,1)={0,1,1};
grid(0,2)={1,0,0}; grid(1,2)={1,0,1};

// Grid element can also be indexed using its offset in the buffer
grid[0] = {0,0,0};
grid[1] = {0,0,1};
grid[2] = {0,1,0};
grid[3] = {0,1,1};
grid[4] = {1,0,0};
grid[5] = {1,0,1};

// Display all elements
std::cout<< grid <<std::endl;


// Compatible with operators
buffer2D<vec3> grid2 = 0.5f * grid;

grid = 2.0f * grid;
buffer2D<vec3> g0 = grid + grid2; // g0(i,j) = grid(i,j) + grid2(i,j)
buffer2D<vec3> g1 = grid * grid2; // g1(i,j) = grid(i,j) * grid2(i,j)
buffer2D<vec3> g2 = grid + vec3(0,1,0); // g2(i,j) = grid(i,j) + (0,1,0)
```

_Note: You cannot `push_back` one element in a buffer2D or buffer3D as it would not be compatible with grid-like structure._


#### Fixed size buffer

The structure `buffer_stack` is similar the `buffer` (contiguous data) but for fixed size container (elements stored on stack memory).
While `buffer` are only convenient wrap around `std::vector`, `buffer_stack` is a convenient wrap around `std::array` for numerical data.

You will probably not have to create yourself directly `buffer_stack` object, but be aware that `vec` (2/3/4) are specific cases of the more general `buffer_stack`.





<!-- ### Doxygen Documentation -->




<!-- 
### Note on Compilation / Execution 


When editing the source code (without adding/removing files), you don't need to run CMake every time, but only call Makefile. The following command can be used from the vcl/ directory:


$ make -C build/


$ build/pgm



## Using QTCreator with CMake (Linux/MacOs)


Call qtcreator from vcl/ directory


$ qtcreator CMakeLists.txt &


Then follow the configuration steps from the GUI.


By default, a temporary directory build-cmake-Desktop-Default is created (in the parent directory of CMakeLists.txt file), as well as a file CMakeLists.txt.user (same directory than CMakeLists.txt file). Both can be removed safely.






## On Windows system with Visual Studio 


- Use CMakeLists.txt with Visual Studio
- Precompiled version of GLFW3 is provided (precompiled/glfw3_win)
- You need to copy data/ and shaders/ directories in the executable directory -->
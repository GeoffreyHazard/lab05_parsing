# Lab 5: Parsing

## 1.1. Introduction

In this lab, you will learn how to work with **scene files** to load all the information you need to render a 3d scene. In particular, you will learn how lights and cameras are represented, as well as how to read **transformation graphs** so that every object has the correct size and position in the scene. You will use the code you’ve written in this lab for the upcoming assignments, including the Ray and Realtime projects!

## 2. Understanding Scene Files

To visualize a compelling 3d scene, we need data about the lights, camera, textures, surfaces and geometry. 
Take a look at ```utils/CS123SceneData.h```. In this file, you will find all the data structures you need to build a scene. This section will go over the conceptual aspects of the scene elements.

### 2.1. Global Data
First, take a look at ```lines 16 - 22``` in ```CS123SceneData.h```:
```cpp
struct CS123SceneGlobalData  {
   // Scene global color coefficients
   float ka;  // global ambient coefficient
   float kd;  // global diffuse coefficient
   float ks;  // global specular coefficient
   float kt;  // global transparency coefficient
};
```
This struct contains all the data relating to the entirety of the scene. In our case, it contains color coefficients that will be applied to the shading of every object in the scene. If you don't know exactly what these coefficents are, don't worry. For the purposes of this lab, you just need to know that each scene will contain global data which we will need to have when rendering it.

### 2.2. Lights
Next, look at ```lines 26 - 28``` in ```CS123SceneData.h```:

```cpp
enum class LightType {
    LIGHT_POINT, LIGHT_DIRECTIONAL, LIGHT_SPOT, LIGHT_AREA
};
```

To simulate different light sources in the real world, we use four types of virtual lights: Point, Directional, Spot, and Area. 
You will know which one of these types of lights it is by the value of the enum ```LightType```.

<details>
  <summary>What are Enums?</summary>
Enums, or enumerated types, are user-defined data types that can take on a finite set of values. The values are defined during declaration, as shown above. Since they are enumerated, each value of the enum has an associated integer value, which by default starts with 0 for the first value. You probably have encountered enums already in Python or Java, where the convention is also to write the values in uppercase.
</details>

Now, look at ```lines 31 - 46``` in ```CS123SceneData.h```:

```cpp
struct CS123SceneLightData {
   int id;
   LightType type;

   CS123SceneColor color;
   glm::vec3 function;  // Attenuation function

   glm::vec4 pos;       // Not applicable to directional lights
   glm::vec4 dir;       // Not applicable to point lights

   float radius;        // Only applicable to spot lights
   float penumbra;      // Only applicable to spot lights
   float angle;         // Only applicable to spot lights

   float width, height; // Only applicable to area lights
};
```

Each light in our scene is represented with a struct that includes its ```LightType```, its color, and other relevant information depending on the ```LightType ```.

<details>
  <summary>What is CS123SceneColor?</summary>

```cpp
// Struct to store a RGBA color in floats [0,1]
using CS123SceneColor = glm::vec4;
```

You can look at lines 112 - 113 to see how color is represented. You should have already worked with with RGBA color in Brush and Filter.
</details>

**Point lights** are an single point in space that emits light (~lightbulb). They don't need a direction vector.

**Directional lights** simulate light from a source that is extremely far away (e.g. the sun). Because of this, they don't have an attenuation function.

**Spot lights** emit light in a cone shape (~a flashlight). They need an angle and radius to describe the dimentions of the cone, as well as a direction and position. The penumbra is how much the light fades away along the edges of the cone.

**Area lights** are squares that emit light in all directions uniformly accross their surface.

_Parts taken from [this article](https://docs.unity3d.com/Manual/Lighting.html) where you can read more about types of light used in 3d scenes._

### 2.3. Camera
For the camera, take a look at ```lines 50 - 60``` in ```CS123SceneData.h```:

```cpp
struct CS123SceneCameraData {
   glm::vec4 pos;
   glm::vec4 look;
   glm::vec4 up;

   float heightAngle;
   float aspectRatio;
};
```
Look familiar? The camera is also just a struct with the relevant fields needed to describe its attributes! The position, look and up vectors of the camera are all described in world space (recall the discussion of world space/camera space from last week's transform's lab).

### 2.4. Primitives
Moving on to ```lines 64 - 71```, we have multiple different types of primitives:
```cpp
enum class PrimitiveType {
    PRIMITIVE_CUBE,
    PRIMITIVE_CONE,
    PRIMITIVE_CYLINDER,
    PRIMITIVE_TORUS,
    PRIMITIVE_SPHERE,
    PRIMITIVE_MESH
};
```
Similarly to ```LightType```, we have ```PrimitiveType``` to keep track of different kinds of primitives. Again like the lights, the primitive types are used in a struct to describe a specific primitive in the scene: 
```cpp
struct CS123ScenePrimitive {
   PrimitiveType type;
   std::string meshfile;     // Only applicable to meshes
   CS123SceneMaterial material;
};
```
All primitives will have a ```PrimitiveType``` and ```CS123SceneMaterial```.
For meshes, the meshfile is a path to an .obj or equivalent file that has mesh geometry.
The material field describes all the important information about what the primitive looks like (its color, how shiny it is, texture, etc). You can look at the struct ```CS123SceneMaterial``` for details, but don't worry if you don't understand any of the terms if they haven't yet been covered in Lecture.

Notice that unlike the lights or camera structs, there aren't any fields in ```CS123ScenePrimitive``` that describe position or orientation. This is because it is more practical to use **transformation graphs** to manage this, especially when we have a lot of primitives!

### 2.5. Transformation Graphs

Since the geometry in our scene is made up of tens or hundrends of primitives, keeping track of the transformations for each one can be particularly inefficient. When creating a scene, it also means that we would have to know the exact final position and orientation of every primitive!

<details>
  <summary>Example of a cityscape</summary>
If our scene is a city, it would be senseless to describe the positions of all the windows by their distance from the center of the city. It would be much more sensible to describe each window's position relative to the building it is part of, place the building on the street it is on, and then describe the  street's position relative to the center of the city.
Moving a house to a different position would be as simple as applying a transformation matrix to the house, and consequently having that translation applied to every window and primitive it contains.
</details>

Consequently, when creating a scene, it is helpful to work with rotation, scaling and translation matricies applied to groupings of objects. However, when rendering a scene, we will need a single transformation matrix for each primitive.
Remember how in the transforms lab you learned to compose a series of rotation, scaling and translation matricies into a single transformation matrix? In the next two subsections, you will learn how to use **transformation graphs** to build final transformation matrices for each primitive.

<details>
  <summary>Example of a cityscape continued</summary>
In our city, we can define a first grouping as the streets, which can themselves be made up of sub-groupings consisting of buildings, which can be made of further sub-groupings of windows, doors and roofs, until we get to the primitives like cubes, pyramids, and cylinders. 
</details>

### 2.5.1. A Simple Transformation Graph

Consider the graph below that represents four primitives in a scene (two spheres, one cube, one cone and a cylinder):

![Scene Graph Image](img/Parsing_Lab_Simple_Graph.jpg)

Primitives are always leaf nodes. Transformation matrices (denoted ```M1```, ```M2```, etc.) are represented on the branches of the graph, and are applied to every child node. All nodes that are not leaf nodes are also called **transblocks** and can be thought of as groupings of objects as mentioned above.


**Task 1. Final Transformation Matrices**
Write the final transformation matrix for each of the primitives in terms of matrices M1, M2, etc. Your answer should be a product of matrices. Keep in mind that order matters when multiplying matricies!

_Aside: A final transformation matrix is also called a **cumulative transformation matrix**._

### 2.5.2. A More Complex Transformation Graph
Now, consider the graph below:
 
![Scene Graph Image](img/Parsing_Lab_City_Graph_v4.jpg)
Instead of transformation matricies ```M1```, ```M2```, etc. we have individual transformations such as Scaling, Translation and Rotation. These are written in the form ```S```, ```T``` and ```R``` followed by the parameters needed for each transformation. For example ```S(15,.1,1)``` scales by ```15``` in the x direction, by ```.1``` in the y direction and by ```1``` in the z direction. Note that Rotations have four parameters, the first three describing the axis of rotation in terms of x, y and z axes, and the last parameter describing the angle of rotation in degrees.

Like before, only the leaves of this tree contain any real geometry in the form of primitives (cube, sphere, etc). 

<details>
  <summary>What scene does this graph represent?</summary>
It represents a simplified version of our city example. We have divided our model into two streets (Willow Street and Main Street). Each of these has a road which is made of one cube that is stretched in the x and squished in the y, as well as a building. Each building is made up of a cube and a door, which is also made up of two cubes! In total we have 8 primitives, all of which are cubes. 
</details>

**Task 2. a. Order of Multiplication of Individual Transformations**
Write a matrix ```M1``` that is all the transformations applied only to Cube 2. You matrix ```M1``` should be in the form of a product of scaling, rotation and translation matricies. You can write each transformation matrix in the form ```S(x, y, z)```.

**Task 2. b. Building the Final Transformation Matrices**
For cubes 1, 3 and 8, write the final transformation matrix in terms of a product of matrices ```S(x, y, z)```, ```R(x, y, z, angle)```, ```T(x, y, z)```, as needed.

**Task 3. Navigating the Scene Graph Efficiently**

In the previous exercise, you might have seen how some matrix multiplications repeat throughout different objects.
Explain why traversing a scene graph from the root node every time each object is rendered is inefficient. Now consider how you might build the final transformation matricies using depth first search.
Explain how your approach is better in terms of time complexity, and write pseudocode for it.


## 3. Implementing a Scene Parser

By now, you should know all about how we represent global, light, camera, primitive and transformation data.

You will also implement a depth first search to generate the final transformation matrices for each primitive. To do this, you will be filling in out a struct called CS123SceneMetaData in `CS123SceneLoader::load`, based off of data found in the scene provided by a SceneData struct (SceneData.h).

### 3.1. Understanding SceneData.h

Throughout this course, we will use a CSCI1230-specific scene file format to describe a scene. Our scene file contains necessary information of a scene, namely global data, camera data, light data, and object data. You should refer to this document to understand scene files and scene graphs in detail.

### 3.2. Implementing CS123SceneLoader::load()
  **Explain the sencil structure, what files do the parsing, what functions to fill in.**
  Behind the scenes, we have parsed out all this data nicely and you can find it in SceneData.h
  
### 3.2.1 Global data Camera Data
  In load(), you will need 
  These are found in the parser member variables.
  
### 3.2.2 Lights
  Array of structs

### 3.2.3 Read the matrix of the root
  Call fsParseSceneNode() in load().
  Check that you are correctly printing out the transformation matrix. **Print out the matrix & provide it so they can check**
  In order to make a matrix from a vec::3, you will need to call GLM::rotate, translate, scale. For rotate in particular, look at SceneData.h angle, and rotate.
  
### 3.2.3 Depth-first search recusively 
  
Whenever we would like to load a scene, we can call `CS123SceneLoader::load`. Since this is a class method, there is no need to create any new `CS123SceneLoader` instance. 

The stencil code already covers the parsing for the XML scene file and what you should do is to create the scene meta-data based on the information inside the parser. In the load function, a new parser instance will be created and try to parse the given file. Upon success, all information will be available via the `CS123ISceneParser` interface.

In `CS123SceneLoader::load`, you should obtain the global data for the scene, the camera data and the lighting. You should also traverse through the primitive tree and calculate the cumulative transformation matrix for each of them. All data described above can be packed into a `CS123SceneMetaData` structure as a unified interface for you to create your own scene for your future assignments.

### 3.3 Testing scenes
  You should have a completed parser that can load all of the scene files. Be prepared to load the things in increasing complexing.
  You can run
  It will print to console but also show you a visual
  
  
## 4. Checkoff
To be checked off, you shoudl have your answers to conceptual questions ready (Tasks 1 through 3).
You should also be able to print out the correct matrices and show a visual of the scene.
  
## 6. (Optional) Creating your own 3-d scenes

You might want to work with the scene files directly for a final project, in order to create your own 3-d scenes. You can use this lab as a starting point and reference on how to do that. Below we will describe how the .xml format works, so you can understand how all the data mentioned above is actually written in the scene files.
Description of the .xml format
  

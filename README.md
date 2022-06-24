# Lab 5: Parsing

## 1.1. Introduction

In this lab, you will learn how to work with **scene files** to load all the information you need to render a 3d scene. In particular, you will learn how lights and cameras are represented, as well as how to read **transformation graphs** so that every object has the correct size and position in the scene. You will use the code you’ve written in this lab for the upcoming assignments, including the Ray and Realtime projects!

## 2. Understanding Scene Files

To visualize a compelling 3d scene, we need data about the lights, camera, textures, surfaces and geometry. 
Take a look at ```utils/CS123SceneData.h```. In this file, you will find all the data structures you need to build a scene. This section will go over the conceptual aspects of the scene elements.

### 2.1. Lights
First, look at ```lines 15 - 36``` in ```CS123SceneData.h```:

```cpp
enum class LightType {
    LIGHT_POINT, LIGHT_DIRECTIONAL, LIGHT_SPOT, LIGHT_AREA
};
```

Lights in the scene are of four possible different types: Point, Directional, Spot, and Area.

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


Each light is represented with a struct that includes what type of light it is, its color, and other relevant information.

**Point lights** are an single point in space that emits light (~lightbulb). They don't need a direction vector.

**Directional lights** simulate light from a source that is extremely far away (e.g. the sun). Because of this, they don't have an attenuation function.

**Spot lights** emit light in a cone shape (~a flashlight). They need an angle and radius to describe the dimentions of the cone, as well as a direction and position. The penumbra is how much the light fades away along the edges of the cone.

**Area lights** are squares that emit light in all directions uniformly accross their surface.

_Parts taken from [this article](https://docs.unity3d.com/Manual/Lighting.html) where you can read more about types of light used in 3d scenes._

### 2.2. Camera
Now, take a look at ```lines 40 - 50``` in ```CS123SceneData.h```:

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

### 2.3. Primitives
On ```line 42```, we also have multiple different types of primitives:
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
Like the lights, the primitive types are used in a struct to describe a specific object in the scene: 
```cpp
struct CS123ScenePrimitive {
   PrimitiveType type;
   std::string meshfile;     // Only applicable to meshes
   CS123SceneMaterial material;
};
```
The meshfile is a path to an .obj or equivalent file that has a mesh's geometry. The material field describes all the important information about what the primitive looks like (its color, how shiny it is, texture, etc). You can look at the struct ```CS123SceneMaterial``` for details, but don't worry if you don't understand any of the terms if they haven't yet been covered in Lecture.

Notice that unlike the lights or camera structs, there aren't any fields in ```CS123ScenePrimitive``` that describe position or orientation. This is because it is more practical to use **transformation graphs** to manage this, especially when we have a lot of primitives!

### 2.4. Transformation Graphs

Since all the geometry in our scene is made up of hundreds or thousands of the primitives seen just above, keeping track of the transformations for each one can be particularly inefficient. This is especially true when we notice that many (but not all) of the same transformations are applied to many primitives in the scene. When creating a scene, it also means that we would have to calculate the exact position and orientation of every primitive!

<details>
  <summary>Example of a cityscape</summary>
If our scene is a city, it would be senseless to describe the positions of all the windows by their distance from the center of the city. It would be much more sensible to describe each window's position relative to the building it is part of, and describe the building's position relative to its neighborhood, and finally the neighborhood's position relative to the center of the city.
Since everything in our scene is made up of primitives, moving a house to a different position would be as simple as applying a transformation matrix to the house.
</details>

We can consequently define nested groupings of geometry that compose our scene. This is particularly helpful for managing the transformation matrices of objects in a compact way.

<details>
  <summary>Example of a cityscape continued</summary>
In our city, we can define a first grouping as the neighborhoods, which can themselves be made up of sub-groupings consisting of buildings, which can be made of further sub-groupings of windows, doors and roofs, until we get to the primitives like cubes, pyramids, and cylinders. 
</details>

We can represent the objects in a scene and their transformations as a directed acyclic graph, which we will call a scene graph or **transformation graphs**.

Consider the graph below that represents four primitives in a scene (two spheres, one cube, one cone and a cylinder found on the leaf nodes). Transformation matrices (denoted M1, M2, etc.) are represented on the branches of the graph. All nodes that are not leaf nodes are also called **transblocks** and can be thought of as groupings of objects as mentioned above.

![Scene Graph Image](img/Parsing_Lab_Simple_Graph.jpg)


**Task 1. a. Order of Multiplication**
Write the cumulative matrix for each of the primitives (leaf nodes) in terms of matrices M1, M2, etc. Your answer should be a product of matrices.


Now consider the graph below. Instead of transformation matricies M1, M2, etc. we now have individual transformations such as Scaling, Translation and Rotation. These are written in the form S, T and R followed by the parameters needed for each transformation. For example S(15,.1,1) scales by 15 in the x direction, by .1 in the y direction and by 1 in the z direction. Note that Rotations have four parameters, the first three describing the axis of rotation in terms of x,y and z axes, and the last parameter describing the angle of rotation in degrees.

Like before, only the leaves of this tree contain any real geometry in the form of primitives (cube, sphere, etc). 
<details>
  <summary>What scene does this graph actually represent?</summary>
It represents a simplified version of our city example. We have divided our model into two districts (Willow Street and Main Street). Each of these districts has a road which is made of one cube that is stretched in the x and squished in the y, as well as a building. Each building is made up of a cube and a door, which is also made up of two cubes! In total we have 8 primitives, all of which are cubes. 
</details>

The letters S, T and R describe transformations applied to the children nodes.
```S(15, .1,1)```
<details>
  <summary>What does the notation like </summary>
We have divided our model into two districts (Willow Street and Main Street). Each of these districts has a road which is made of one cube that is stretched in the x and squished in the y, as well as a building. Each building is made up of a cube and a door, which is also made up of two cubes! In total we have 8 primitives, all of which are cubes. 
</details>
S stands for Scale, T for Transformation, R for Rotation, and the numbers in parentheses. 
 
![Scene Graph Image](img/Parsing_Lab_City_Graph_v4.jpg)


Remember how in the transforms lab you learned how to compose a series of rotation, scaling and translation matricies into a single transformation matrix? In order to transform each primitive to its final place, you will need one of these transformation matrices for each of the primitives in your scene.
Keep in mind that order matters when multiplying matricies!


**Task 2. a. Order of Multiplication of Individual Transformations**
For two Cubes of your choice, write down the order in which the three matricies should be applied. You can write each matrix in the form S(0, 6, .1) where S is the type of transformation (S for Scale, T for Translation, R for Rotation) and the parameters

**Task 2. b. Building the Final Transformation Matrices**
For cubes 1, 3 and 8, write the final transformation matrix in terms of a product of matrices T, R, S, etc., as needed. You can use the same notation as 1. a.

**Task 3. Navigating the Scene Graph Efficiently**

In the previous exercise, you might have seen how some matrix multiplications repeat throughout different objects.
Explain why traversing a scene graph from the root node every time each object is rendered is inefficient. Now consider how you might build the final transformation matricies using depth first search.
Explain how your approach is better in terms of time complexity, and write pseudocode for it.


## 3. Implementing a Scene Parser

In this section, you will load in the lights, camera and scene data. You will also implement a depth first search to generate the final transformation matrices for each matrix. To do this, you will be filling in out a struct called CS123SceneMetaData in `CS123SceneLoader::load`, based off of data found in the scene provided by a SceneData struct (SceneData.h).

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
  
  
### 4. Checkoff
To be checked off, you shoudl have your answers to conceptual questions ready (Tasks 1 through 3).
You should also be able to print out the correct matrices and show a visual of the scene.
  
## 6. (Optional) Creating your own 3-d scenes

You might want to work with the scene files directly for a final project, in order to create your own 3-d scenes. You can use this lab as a starting point and reference on how to do that. Below we will describe how the .xml format works, so you can understand how all the data mentioned above is actually written in the scene files.
Description of the .xml format
  

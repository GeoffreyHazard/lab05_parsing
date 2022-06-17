# Lab 5: Parsing

## 1. Introduction

In this lab, you will learn how to work with **scene files** to load all the information you need to render a 3d scene. In particular, you will learn how to read **transformation graphs** so that every object has the correct size and position in the scene. You will need the code you’ve written in this lab for the upcoming assignments!

## 2. Problem Statement

To visualize a compelling 3d scene, we need data about the camera, lights, textures, surfaces and geometry. The final placement of the geometry (representing objects, people, or just about anything else) can be particularly overwhelming, because we need to manage many different transformations like scaling, rotation or translation. Often, many (but not all) of the same transformations are applied to many objects in the scene. We need a good data structure to manage this!

<details>
  <summary>Example of a cityscape</summary>
If our scene is a city, it would be senseless to describe the positions of all the windows by their distance from the center of the city. It would be much more sensible to describe each window's position relative to the building it is part of, and describe the building's position relative to its neighborhood, and finally the neighborhood's position relative to the center of the city.

</details>

## 3. Understanding Transfromation Graphs

To handle the objects and their transformations, we can define nested groupings of geometry that compose our scene. This is particularly helpful for managing the transformaton matricies of objects in a compact way.

<details>
  <summary>Example of a cityscape continued</summary>
In our city, we can define a first grouping as the neighborhoods, which can themselves be made up of sub-groupings consisting of buildings, which can be made of further sub-groupings of windows, doors and roofs, until we get to the primitives like cubes, pyramids, and cylinders. 
</details>

We can represent the objects in a scene and their transformations as a directed acyclic graph, which we will call a scene graph or **transformation graphs**.

Consider the graph below that represents four primitives in a scene (two spheres, one cube, one cone and a cylinder found on the leaf nodes). Transformation matrices (denoted M1, M2, etc.) are represented on the branches of the graph. All nodes that are not leaf nodes are also called **transblocks** and can be thought of as groupings of objects as mentioned above.

![Scene Graph Image](img/Parsing_Lab_Simple_Graph.jpg)


**Task 1. a. Order of Multiplication**
Write the cumulative matrix for each of the primitives (leaf nodes) in terms of matricies M1, M2, etc. Your answer should be a product of matricies.


Now consider the graph below, which represents a simplified version of our city example. The nodes separate objects that have different transformations applied to them. On the branches, we showed the exact transformations to be appled to all of the geometry downstream. These are written in the form S, T or R along with the  Only the leaves of this tree contain any real geometry in the form of primitives (cube, sphere, etc).
<details>
  <summary>How does the graph represent a model of a city?</summary>
We have devided our model into two districts (Willow Street and Main Street). Each of these districts has a road which is made of one cube that is stretched in the x and squished in the y, as well as a building. Each building is made up of a cube and a door, which is also made up of two cubes! In total we have 8 primitives, all of which are cubes. 
</details>

The letters T, R and S describe transformations applied to the children nodes.
 
![Scene Graph Image](img/Parsing_Lab_City_Graph_v4.jpg)


Remember how in the transforms lab you learned how to compose a series of rotation, scaling and translation matricies into a single transformation matrix? In order to transform each primitive to its final place, you will need one of these transformation matrices for each of the primitives in your scene.
Keep in mind that order matters when multiplying matricies!


**Task 2. a. Order of Multiplication of Individual Transformations**
For the Cube 2, write down the order in which the three matricies should be applied. You can write each matrix in the form S(0, 6, .1)

**Task 2. b. Building the Final Transformation Matrices**
For cubes 1, 3 and 8, write the final transformation matrix in terms of a product of matrices T, R, S, etc., as needed. You can use the same notation as 1. a.

**Task 3. Navigating the Scene Graph Efficiently**

In the previous exercise, you might have seen how some matrix multiplications repeat throughout different objects.
Explain why traversing a scene graph from the root node every time each object is rendered is inefficient. Now consider how you might build the final transformation matricies using Depth First Search.
Explain how your approach is better in terms of time complexity, and write pseudocode for it.


## 4. Implementing a Scene Parser

In this section you will be filling in CS123SceneMetaData struct using depth first search. It has everything you need in this lab
You may have noticed that omatadata has an &, explain references here.
**Dropdown on references**
Now, you are ready to implement the static `CS123SceneLoader::load` method in the provided stencil.

### 4.1. Understanding the Header Files in SceneData.h
**In detail**

Throughout this course, we will use a CSCI1230-specific scene file format to describe a scene. Our scene file contains necessary information of a scene, namely global data, camera data, light data, and object data. You should refer to this document to understand scene files and scene graphs in detail.

### 4.2. Implementing CS123SceneLoader::load()
  **Explain the sencil structure, what files do the parsing, what functions to fill in.**
  Behind the scenes, we have parsed out all this data nicely and you can find it in SceneData.h
  
### 4.2.1 Global data Camera Data
  In load(), you will need 
  These are found in the parser member variables.
  
### 4.2.2 Lights
  Array of structs

### 4.2.3 Read the matrix of the root
  Call fsParseSceneNode() in load().
  Check that you are correctly printing out the transformation matrix. **Print out the matrix & provide it so they can check**
  In order to make a matrix from a vec::3, you will need to call GLM::rotate, translate, scale. For rotate in particular, look at SceneData.h angle, and rotate.
  
### 4.2.3 Depth-first search recusively 
  
Whenever we would like to load a scene, we can call `CS123SceneLoader::load`. Since this is a class method, there is no need to create any new `CS123SceneLoader` instance. 

The stencil code already covers the parsing for the XML scene file and what you should do is to create the scene meta-data based on the information inside the parser. In the load function, a new parser instance will be created and try to parse the given file. Upon success, all information will be available via the `CS123ISceneParser` interface.

In `CS123SceneLoader::load`, you should obtain the global data for the scene, the camera data and the lighting. You should also traverse through the primitive tree and calculate the cumulative transformation matrix for each of them. All data described above can be packed into a `CS123SceneMetaData` structure as a unified interface for you to create your own scene for your future assignments.

### 4.3 Testing scenes
  You should have a completed parser that can load all of the scene files. Be prepared to load the things in increasing complexing.
  You can run
  It will print to console but also show you a visual
  
  
### 5. Checkoff
To be checked off, you shoudl have your answers to conceptual questions ready (Tasks 1 through 3).
You should also be able to print out the correct matrices and show a visual of the scene.
  
## 6. (Optional) Creating your own 3-d scenes

You might want to work with the scene files directly for a final project, in order to create your own 3-d scenes. You can use this lab as a starting point and reference on how to do that. Below we will describe how the .xml format works, so you can understand how all the data mentioned above is actually written in the scene files.
Description of the .xml format
  

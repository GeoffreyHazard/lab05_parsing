# Lab 5: Parsing

## 1 Introduction

In this lab, you will learn how scene graphs work and how to load them. You are expected to implement a scene file loader that will load 3-d geometry in a way that you can use it effectively. You will need the code you’ve written in this lab for the upcoming assignments!

## 2 Understanding Scene Graphs

In order to visualize a compelling 3-dimensional scene, we need to manage tons of complex geometries that represent objects, people, or just about anything else. If we are using real-time graphics, it would be senseless to require the manual placement of the thousands of tiny triangles that make up our 3-d scene. When working on Raytraced geometry, it would be equally senseless to place every implicitly-defined shape to its final place in the scene. The placement of all of these geometries becomes particularly hard to manage.

To solve this, we can define groupings of geometry that compose our scene, which can themselves be made up of sub-groupings, etc, until we get to the primitives. Since such groupings can be nested, our representation of objects in a scene will form a directed acyclic graph, which we will call scene graph.

Probably include some handwritten tasks like last year, but will we set up a check-off session for the parser lab since we are planning on setting up ‘auto-grader’ for this lab?


## 3 Implementing the Scene Loader

Now, you are ready to implement the static `CS123SceneLoader::load` method in the provided stencil.

### 3.1 CSCI1230-specific Format

Throughout this course, we will use a CSCI1230-specific scene file format to describe a scene. Our scene file contains necessary information of a scene, namely global data, camera data, light data, and object data. You should refer to this document to understand scene files and scene graphs in detail.

### 3.2 What CS123SceneLoader::load should do?

Whenever we would like to load a scene, we can call `CS123SceneLoader::load`. Since this is a class method, there is no need to create any new `CS123SceneLoader` instance. 

The stencil code already covers the parsing for the XML scene file and what you should do is to create the scene meta-data based on the information inside the parser. In the load function, a new parser instance will be created and try to parse the given file. Upon success, all information will be available via the `CS123ISceneParser` interface.

In `CS123SceneLoader::load`, you should obtain the global data for the scene, the camera data and the lighting. You should also traverse through the primitive tree and calculate the cumulative transformation matrix for each of them. All data described above can be packed into a `CS123SceneMetaData` structure as a unified interface for you to create your own scene for your future assignments.

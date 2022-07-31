#include "SceneParser.h"
#include "ScenefileReader.h"
#include "glm/gtx/transform.hpp"

#include <chrono>
#include <memory>
#include <iostream>

using namespace std;

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    shared_ptr<ScenefileReader> fileReader = make_shared<ScenefileReader>(filepath);
    bool success = fileReader->readXML();
    if (!success) {
        return false;
    }

    //TODO: Use the fileReader instance to get the scene data and turn it into RenderData

    return true;
}

/* TA SOLUTION BEGIN */

void SceneParser::dfsParseSceneNode(RenderData &renderData, SceneNode *node, glm::mat4 matrix) {
    //TODO: Navigate the scenegraph and load RenderShapes into RenderData
    return;
}

/* TA SOLUTION BEGIN */

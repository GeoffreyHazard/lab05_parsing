#include "SceneParser.h"
#include "ScenefileReader.h"
#include "glm/gtx/transform.hpp"

#include <chrono>
#include <memory>
#include <iostream>

using namespace std;

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readXML();
    if (!success) {
        return false;
    }

    /* TA SOLUTION BEGIN */

    // load the scene with the data in the parser
    renderData.lights.clear();
    renderData.shapes.clear();

    renderData.cameraData = fileReader.getCameraData();
    renderData.globalData = fileReader.getGlobalData();
    renderData.lights = fileReader.getLights();

    SceneNode *root = fileReader.getRootNode();
    glm::mat4 matrix(1.0f);

    auto startTS = std::chrono::system_clock::now();
    std::cout << std::endl << "Begin parsing scene " << filepath << std::endl;

    dfsParseSceneNode(renderData, root, matrix);

    auto endTS = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTS - startTS;
    std::cout << "Complete parsing scene. Time elapse: " << elapsed_seconds.count() * 1000.f << "ms" << std::endl;

    /* TA SOLUTION END */

    return true;
}

/* TA SOLUTION BEGIN */

void SceneParser::dfsParseSceneNode(RenderData &renderData, SceneNode * node, glm::mat4 matrix) {
    if (node == nullptr) {
        return;
    }

    for (auto transform : node->transformations) {
        glm::mat4 transMatrix;
        switch (transform->type) {
        case TRANSFORMATION_TRANSLATE:
        {
            transMatrix = glm::translate(transform->translate);
            break;
        }
        case TRANSFORMATION_SCALE:
        {
            transMatrix = glm::scale(transform->scale);
            break;
        }
        case TRANSFORMATION_ROTATE:
        {
            transMatrix = glm::rotate(transform->angle, transform->rotate);
            break;
        }
        case TRANSFORMATION_MATRIX:
        {
            transMatrix = transform->matrix;
            break;
        }
        default:
            ;
        }
        matrix = matrix * transMatrix;
    }

    for (auto primitive : node->primitives) {
        RenderShapeData shape = {.primitive = *primitive, .ctm = matrix};
        renderData.shapes.emplace_back(shape);
    }

    for (SceneNode  * child : node->children) {
        dfsParseSceneNode(renderData, child, matrix);
    }
    return;
}

/* TA SOLUTION BEGIN */

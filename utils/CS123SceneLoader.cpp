#include "CS123SceneLoader.h"
#include "CS123XmlSceneParser.h"
#include "glm/gtx/transform.hpp"

#include <chrono>
#include <memory>
#include <iostream>

using namespace CS123;
using namespace std;

bool CS123SceneLoader::load(std::string filepath, CS123SceneMetaData &oMetaData) {
    shared_ptr<CS123XmlSceneParser> parser = make_shared<CS123XmlSceneParser>(filepath);
    bool success = parser->parse();
    if (!success) {
        return false;
    }

    // load the scene with the data in the parser
    oMetaData.lights.clear();
    oMetaData.shapes.clear();

    parser->getCameraData(oMetaData.cameraData);
    parser->getGlobalData(oMetaData.globalData);

    int numLights = parser->getNumLights();
    oMetaData.lights.reserve(numLights);

    for (int i = 0; i < numLights; i++) {
        CS123SceneLightData lightData;
        parser->getLightData(i, lightData);
        oMetaData.lights.emplace_back(lightData);
    }

    CS123SceneNode *root = parser->getRootNode();
    glm::mat4 matrix(1.0f);

    auto startTS = std::chrono::system_clock::now();
    std::cout << std::endl << "Begin loading scene " << filepath << std::endl;

    dfsParseSceneNode(oMetaData, root, matrix);

    auto endTS = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTS - startTS;
    std::cout << "Complete loading scene. Time elapse: " << elapsed_seconds.count() * 1000.f << "ms" << std::endl;

    return true;
}

void CS123SceneLoader::dfsParseSceneNode(CS123SceneMetaData &oMetaData, CS123SceneNode *node, glm::mat4 matrix) {
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
        CS123SceneShapeData shape = {.primitive = *primitive, .ctm = matrix};
        oMetaData.shapes.emplace_back(shape);
    }

    for (CS123SceneNode * child : node->children) {
        dfsParseSceneNode(oMetaData, child, matrix);
    }
    return;
}

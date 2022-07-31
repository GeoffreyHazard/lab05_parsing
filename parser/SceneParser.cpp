#include "SceneParser.h"
#include "ScenefileReader.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/string_cast.hpp"

#include <chrono>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace std;

auto DFSTraversal(auto& Shapes, auto&& ParentTransformation, auto Node)->void {
    auto FusedTransformation = ParentTransformation;
    for (auto x : Node->transformations)
        FusedTransformation *= [&] {
        if (x->type == TRANSFORMATION_TRANSLATE)
            return glm::translate(x->translate);
        else if (x->type == TRANSFORMATION_SCALE)
            return glm::scale(x->scale);
        else if (x->type == TRANSFORMATION_ROTATE)
            return glm::rotate(x->angle, x->rotate);
        else if (x->type == TRANSFORMATION_MATRIX)
            return x->matrix;
        throw std::runtime_error{ "?????"};
    }();

    for (auto x : Node->primitives)
        Shapes.push_back({ .primitive = *x, .ctm = FusedTransformation });

    for (auto x : Node->children)
        DFSTraversal(Shapes, FusedTransformation, x);
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    shared_ptr<ScenefileReader> fileReader = make_shared<ScenefileReader>(filepath);
    bool success = fileReader->readXML();
    if (!success) {
        return false;
    }

    fileReader->getGlobalData(renderData.globalData);
    fileReader->getCameraData(renderData.cameraData);

    renderData.lights.resize(fileReader->getNumLights());

    for (auto x = 0; x < fileReader->getNumLights(); ++x)
        fileReader->getLightData(x, renderData.lights[x]);

    DFSTraversal(renderData.shapes, glm::mat4{}, fileReader->getRootNode());

    auto num_to_str = [](auto x) {
        if (x == PrimitiveType::PRIMITIVE_CUBE)
            return "cube";
        else if (x == PrimitiveType::PRIMITIVE_CONE)
            return "cone";
        else if (x == PrimitiveType::PRIMITIVE_CYLINDER)
            return "cylinder";
        else if (x == PrimitiveType::PRIMITIVE_MESH)
            return "mesh";
        else if (x == PrimitiveType::PRIMITIVE_SPHERE)
            return "sphere";
        else if (x == PrimitiveType::PRIMITIVE_TORUS)
            return "torus";
        throw std::runtime_error{ "wtf???"};
    };

    for (auto& x : renderData.shapes)
        std::cout << num_to_str(x.primitive.type) << ": " << glm::to_string(x.ctm) << std::endl;


    return true;
}



void SceneParser::dfsParseSceneNode(RenderData &renderData, SceneNode *node, glm::mat4 matrix) {
    //TODO: Navigate the scenegraph and load RenderShapes into RenderData
    return;
}

/* TA SOLUTION BEGIN */

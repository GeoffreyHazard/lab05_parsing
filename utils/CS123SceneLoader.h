#ifndef CS123SCENELOADER_H
#define CS123SCENELOADER_H

#include "CS123SceneData.h"
#include <vector>
#include <string>

namespace CS123 {

struct CS123SceneShapeData {
    // The primitive description structure
    CS123ScenePrimitive primitive;
    // The cumulative transformation matrix
    glm::mat4 ctm;
};

struct CS123SceneMetaData {
    CS123SceneGlobalData globalData;
    CS123SceneCameraData cameraData;

    std::vector<CS123SceneLightData> lights;
    std::vector<CS123SceneShapeData> shapes;
};

class CS123SceneLoader
{
public:
    // Load the scene and store the results in oMetaData.
    // @param filepath The path of the scene file to load.
    // @param oMetaData On return, this will contain the metadata of the loaded scene.
    // @return A boolean value indicating whether the load is successful.
    static bool load(std::string filepath, CS123SceneMetaData &oMetaData);

private:
    static void dfsParseSceneNode(CS123SceneMetaData &oMetaData, CS123SceneNode *node, glm::mat4 matrix);
};

}

#endif // CS123SCENELOADER_H

#include "Renderer.hpp"

#include "MeshComponent.hpp"
#include "CubeMesh.hpp"
#include "WaterMesh.hpp"
#include "TextureComponent.hpp"

Renderer::Renderer(const ObjectManager &objectManager, const Camera &camera) : mObjectManager(objectManager), mCamera(camera)
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderSolidObjects()
{
    // // collect ptrs to each object type
    // std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>> objectMap;

    // for (auto &object : mObjectManager.GetObjects())
    // {
    //     MeshComponent *meshComponent = object.second->GetComponent<MeshComponent>();

    //     if (meshComponent->GetMeshType() == MeshType::CUBE)
    //     {
    //         objectMap[object.second->GetID()].push_back(object.second);
    //     }
    //     object.second->Render();
    // }

    // // find the texture information for each object type
    // std::unordered_map<std::string, std::map<std::string, GLuint *>> textureMap;

    // for (auto &obj : objectMap)
    // {
    //     if (textureMap.find(obj.first) != textureMap.end())
    //     {
    //         continue;
    //     }

    //     TextureComponent *textureComponent = obj.second[0]->GetComponent<TextureComponent>();
    //     textureMap[obj.first] = textureComponent->GetTextureGroup();
    // }

    // 
}

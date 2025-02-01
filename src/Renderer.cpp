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
}

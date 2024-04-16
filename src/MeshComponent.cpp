#include "MeshComponent.hpp"
#include "CubeMesh.hpp"
#include "WaterMesh.hpp"

void MeshComponent::Input()
{
}

void MeshComponent::Update()
{
    std::shared_ptr<Mesh> mesh = mMesh;
    if (mesh != nullptr)
    {
        mesh->Update();
    }
}

void MeshComponent::Render()
{
    std::shared_ptr<Mesh> mesh = mMesh;
    if (mesh != nullptr)
    {
        mesh->Render();
    }
}

// std::shared_ptr<Cube> MeshComponent::AddCube()
// {
//     std::shared_ptr<Cube> cube = std::make_shared<Cube>();
//     cube->SetParentComponent(this);
//     mCubes.push_back(cube);
//     return cube;
// }

std::shared_ptr<Mesh> MeshComponent::AddMesh(MeshType type)
{
    std::shared_ptr<Mesh> cube;
    if (type == MeshType::CUBE)
    {
        cube = std::make_shared<CubeMesh>();
    }
    else if (type == MeshType::WATER)
    {
        cube = std::make_shared<WaterMesh>();
    } else {
        return nullptr;
    }

    mMesh = cube;
    cube->SetParentComponent(this);
    mMeshType = type;
    return cube;
}

void MeshComponent::SetVisibility(std::string side, bool visible)
{
    mVisibilityMap[side] = visible;
}
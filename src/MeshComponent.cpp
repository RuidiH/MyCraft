#include "MeshComponent.hpp"
#include "Mesh.hpp"
#include "CubeMesh.hpp"
#include "WaterMesh.hpp"

MeshComponent::MeshComponent(MeshType type)
{
    std::shared_ptr<Mesh> cube;
    if (type == MeshType::CUBE)
    {
        cube = std::make_shared<CubeMesh>();
    }
    else if (type == MeshType::WATER)
    {
        cube = std::make_shared<WaterMesh>();
    } 
    mMesh = cube;
    cube->SetParentComponent(this);
    mMeshType = type;

    mVisibleSides = std::make_shared<std::unordered_set<std::string>>(); 
    mVisibleSides->insert({"front", "back", "left", "right", "top", "bottom"});
}

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

void MeshComponent::Init(std::shared_ptr<std::unordered_map<std::string, std::vector<float>>> vertices)
{
    std::shared_ptr<Mesh> mesh = mMesh;
    if (mesh != nullptr)
    {
        mesh->Init(vertices);
    }
}

// std::shared_ptr<Cube> MeshComponent::AddCube()
// {
//     std::shared_ptr<Cube> cube = std::make_shared<Cube>();
//     cube->SetParentComponent(this);
//     mCubes.push_back(cube);
//     return cube;
// }

// std::shared_ptr<Mesh> MeshComponent::AddMesh(MeshType type)
// {
//     std::shared_ptr<Mesh> cube;
//     if (type == MeshType::CUBE)
//     {
//         cube = std::make_shared<CubeMesh>();
//     }
//     else if (type == MeshType::WATER)
//     {
//         cube = std::make_shared<WaterMesh>();
//     } else {
//         return nullptr;
//     }

//     mMesh = cube;
//     cube->SetParentComponent(this);
//     mMeshType = type;
//     return cube;
// }

void MeshComponent::SetVisibility(std::string side, bool visible)
{
    mVisibilityMap[side] = visible;
}
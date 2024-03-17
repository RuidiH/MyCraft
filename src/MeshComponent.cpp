#include "MeshComponent.hpp"

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
    if (type == MeshType::CUBE)
    {
        std::shared_ptr<CubeMesh> cube = std::make_shared<CubeMesh>();
        mMesh = cube;
        cube->SetParentComponent(this);
        mMeshType = type; 
        return cube;
    } else if (type == MeshType::WATER)
    {
        // TODO: Add water mesh
    }
    // handle error if type is not recognized

    return nullptr;
}

// void MeshComponent::setCube(Cube *cube)
// {
//     if (cube != nullptr)
//     {
//         this->cube = cube;
//     }
// }

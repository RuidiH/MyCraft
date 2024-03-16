#include "MeshComponent.hpp"

void MeshComponent::Input()
{
}

void MeshComponent::Update()
{
    for (auto cube : mCubes)
    {
        cube->Update();
    }
}

void MeshComponent::Render()
{
    for (auto cube : mCubes)
    {
        cube->Render();
    }
}

void MeshComponent::AddCube(std::shared_ptr<Cube> cube)
{
    if (cube != nullptr)
    {
        mCubes.push_back(cube);
        cube->SetParentComponent(this);
    }
}

std::shared_ptr<Cube> MeshComponent::AddCube() {
    std::shared_ptr<Cube> cube = std::make_shared<Cube>();
    cube->SetParentComponent(this);
    mCubes.push_back(cube); 
    return cube;
}

// void MeshComponent::setCube(Cube *cube)
// {
//     if (cube != nullptr)
//     {
//         this->cube = cube;
//     }
// }

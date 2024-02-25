#include "ShapeComponent.hpp"

void ShapeComponent::Input()
{
}

void ShapeComponent::Update()
{
    for (auto cube : mCubes)
    {
        cube->Update();
    }
}

void ShapeComponent::Render()
{
    for (auto cube : mCubes)
    {
        cube->Render();
    }
}

void ShapeComponent::AddCube(std::shared_ptr<Cube> cube)
{
    if (cube != nullptr)
    {
        mCubes.push_back(cube);
        cube->SetParentComponent(this);
    }
}

std::shared_ptr<Cube> ShapeComponent::AddCube() {
    std::shared_ptr<Cube> cube = std::make_shared<Cube>();
    cube->SetParentComponent(this);
    mCubes.push_back(cube); 
    return cube;
}

// void ShapeComponent::setCube(Cube *cube)
// {
//     if (cube != nullptr)
//     {
//         this->cube = cube;
//     }
// }

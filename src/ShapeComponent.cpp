#include "ShapeComponent.hpp"

void ShapeComponent::Input()
{
}

void ShapeComponent::Update()
{
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

// void ShapeComponent::setCube(Cube *cube)
// {
//     if (cube != nullptr)
//     {
//         this->cube = cube;
//     }
// }

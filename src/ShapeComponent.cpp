#include "ShapeComponent.hpp"

void ShapeComponent::Input()
{
}

void ShapeComponent::Update()
{
}

void ShapeComponent::Render()
{
    cube->Render();
}

void ShapeComponent::setCube(Cube *cube)
{
    if (cube != nullptr)
    {
        this->cube = cube;
    }
}

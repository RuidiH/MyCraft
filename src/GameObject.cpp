#include "GameObject.hpp"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
    // for (auto component : mComponents)
    // {
    //     delete component;
    // }
    mComponents.clear();
}

// TODO: complete game loop functions
void GameObject::Input()
{
}

void GameObject::Update()
{
    for (auto component : mComponents)
    {
        component->Update();
    }
}

void GameObject::Render()
{
    for (auto component : mComponents)
    {
        component->Render();
    }
}

// void GameObject::AddComponent(std::shared_ptr<Component> component)
// {
//     if (component != nullptr)
//     {
//         mComponents.push_back(component);
//         component->SetParent(std::make_unique<GameObject>(*this));
//     }
// }

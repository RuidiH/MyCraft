#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <vector>
#include <memory>

#include "Component.hpp"
class Component;

class GameObject
{
public:
    GameObject(std::string id) : mID(id) {};
    ~GameObject();

    // Game loop functions
    void Input();
    void Update();
    void Render();

    // void AddComponent(std::shared_ptr<Component> component);
    template <typename T, typename... Args>
    T *AddComponent(Args &&...args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component type");

        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        component->SetParent(this);
        mComponents.push_back(component);
        return component.get();
    }

    // Template method to get a component of a specific type
    template <typename T>
    T *GetComponent() const
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component type");

        for (const auto &component : mComponents)
        {
            T *casted = dynamic_cast<T *>(component.get());
            if (casted != nullptr)
            {
                return casted;
            }
        }
        return nullptr; // Return nullptr if the component of type T is not found
    }

    // Getters and setters
    std::string GetID() const { return mID; }

private:
    std::string mID;
    std::vector<std::shared_ptr<Component>> mComponents;
};

#endif // GAME_OBJECT_HPP
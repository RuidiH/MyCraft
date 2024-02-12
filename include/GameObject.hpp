#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <vector>
#include <memory>

#include "Component.hpp"

class GameObject
{
public:
    GameObject();
    ~GameObject();

    // Game loop functions
    void Input();
    void Update();
    void Render();

    void AddComponent(std::shared_ptr<Component> component);

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

private:
    std::vector<std::shared_ptr<Component>> mComponents;
};

#endif // GAME_OBJECT_HPP
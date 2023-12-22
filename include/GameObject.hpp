#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <vector>

#include "Component.hpp"

class GameObject {
    public:
        GameObject();
        ~GameObject();

        // Game loop functions
        void Input();
        void Update();
        void Render();

        void AddComponent(Component* component);
    private:
        std::vector<Component*> mComponents;
};

#endif // GAME_OBJECT_HPP
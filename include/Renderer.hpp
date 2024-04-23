#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <unordered_map>
#include <memory>
#include <string>

#include "GameObject.hpp"
#include "ObjectManager.hpp"
#include "Camera.hpp"

class Renderer
{
    public:
        Renderer(const ObjectManager &objectManager, const Camera &camera);
        ~Renderer();

        void RenderSolidObjects();
        void RenderTransparentObjects();

    private:
        const ObjectManager &mObjectManager;
        const Camera &mCamera;

};

#endif
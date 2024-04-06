#ifndef OBJECT_MANAGER_HPP
#define OBJECT_MANAGER_HPP

#include <memory>
#include <vector>

#include "GameObject.hpp"
#include "Camera.hpp"

class ObjectManager
{
public:
    ObjectManager(std::shared_ptr<Camera> camera);
    ~ObjectManager();

    void AddSolidObject(std::shared_ptr<GameObject> object);
    void AddTransparentObject(std::shared_ptr<GameObject> object);
    void AddObject(std::shared_ptr<GameObject> object);

    void RemoveSolidObject(std::shared_ptr<GameObject> object);
    void RemoveTransparentObject(std::shared_ptr<GameObject> object);
    void RemoveObject(std::shared_ptr<GameObject> object);

    std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> GetSolidObjects();
    std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> GetTransparentObjects();
    std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> GetObjects();

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> mSolidObjects;
    std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> mTransparentObjects;
    std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> mObjects;
};

#endif
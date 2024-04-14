#ifndef OBJECT_MANAGER_HPP
#define OBJECT_MANAGER_HPP

#include <memory>
#include <vector>
#include <set>

#include "GameObject.hpp"
#include "Camera.hpp"

// Custom comparator that uses the camera to sort GameObjects by distance
class TransparentObjectComparator {
    const Camera& camera;

public:
    TransparentObjectComparator(const Camera& cam) : camera(cam) {}

    bool operator()(const std::shared_ptr<GameObject>& lhs, const std::shared_ptr<GameObject>& rhs) const {
        return camera.DistanceTo(*lhs) > camera.DistanceTo(*rhs);
    }
};

class ObjectManager
{
public:
    ObjectManager(const Camera& camera);
    ~ObjectManager();

    void AddObject(const std::shared_ptr<GameObject>& object);
    void RemoveObject(const std::shared_ptr<GameObject>& object);
    void UpdateSortedTransparentObjects();


    // void AddSolidObject(std::shared_ptr<GameObject> object);
    // void AddTransparentObject(std::shared_ptr<GameObject> object);
    // void AddObject(std::shared_ptr<GameObject> object);

    // void RemoveSolidObject(std::shared_ptr<GameObject> object);
    // void RemoveTransparentObject(std::shared_ptr<GameObject> object);
    // void RemoveObject(std::shared_ptr<GameObject> object);

    std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> GetSolidObjects();
    const std::set<std::shared_ptr<GameObject>, TransparentObjectComparator>& GetTransparentObjects();
    const std::vector<std::shared_ptr<GameObject>>& GetObjects();

private:
    // std::shared_ptr<Camera> mCamera;
    const Camera& mCamera;
    std::set<std::shared_ptr<GameObject>, TransparentObjectComparator> mSortedTransparentObjects;
    std::vector<std::shared_ptr<GameObject>> mObjects;

    // std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> mSolidObjects;
    // std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> mTransparentObjects;
    // std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> mObjects;
};

#endif
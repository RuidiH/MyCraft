#ifndef OBJECT_MANAGER_HPP
#define OBJECT_MANAGER_HPP

#include <memory>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <string>

#include "GameObject.hpp"
#include "Camera.hpp"

// Custom comparator that uses the camera to sort GameObjects by distance
class TransparentObjectComparator
{
    const Camera &camera;

public:
    TransparentObjectComparator(const Camera &cam) : camera(cam) {}

    bool operator()(const std::shared_ptr<GameObject> &lhs, const std::shared_ptr<GameObject> &rhs) const
    {
        return camera.DistanceTo(*lhs) > camera.DistanceTo(*rhs);
    }
};

class ObjectManager
{
public:
    ObjectManager(const Camera &camera, const bool &isRunning);
    ~ObjectManager();

    void AddObject(const std::shared_ptr<GameObject> &object);
    void RemoveObject(const std::shared_ptr<GameObject> &object);
    void UpdateSortedTransparentObjects();

    const std::set<std::shared_ptr<GameObject>, TransparentObjectComparator> &GetTransparentObjects();
    const std::unordered_map<std::string, std::shared_ptr<GameObject>> &GetObjects();
    
private:
    const Camera &mCamera;
    const bool &mIsRunning; 
    std::set<std::shared_ptr<GameObject>, TransparentObjectComparator> mSortedTransparentObjects;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> mObjects;

    const std::vector<std::pair<std::string, glm::vec3>> mNeighbors = {
        {"front", glm::vec3(0, 0, 1)},
        {"back", glm::vec3(0, 0, -1)},
        {"left", glm::vec3(-1, 0, 0)},
        {"right", glm::vec3(1, 0, 0)},
        {"top", glm::vec3(0, 1, 0)},
        {"bottom", glm::vec3(0, -1, 0)}};

    const std::map<std::string, std::string> mOppositeSides = {
        {"front", "back"},
        {"back", "front"},
        {"left", "right"},
        {"right", "left"},
        {"top", "bottom"},
        {"bottom", "top"}};
};

#endif
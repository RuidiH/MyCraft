#include "ObjectManager.hpp"
#include "MeshComponent.hpp"
#include "TransformComponent.hpp"

#include <algorithm>
#include <iostream>

ObjectManager::ObjectManager(const Camera &camera) : mCamera(camera), mSortedTransparentObjects(TransparentObjectComparator(camera))
{
    // mSolidObjects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>();
    // mTransparentObjects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>();
    // mObjects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>();
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::AddObject(const std::shared_ptr<GameObject> &object)
{
    MeshComponent *meshComponent = object->GetComponent<MeshComponent>();

    if (meshComponent == nullptr)
    {
        std::cout << "Object does not have a mesh component\n";
        return;
    }

    TransformComponent *transformComponent = object->GetComponent<TransformComponent>();

    // check if neighbor exists
    for (auto neighbor : mNeighbors)
    {
        glm::vec3 neighborPosition = transformComponent->GetPosition() + neighbor.second;
        std::string neighborPositionString = std::to_string(neighborPosition.x) + ", " + std::to_string(neighborPosition.y) + ", " + std::to_string(neighborPosition.z);
        if (mObjects.find(neighborPositionString) != mObjects.end())
        {
            if (meshComponent->GetMeshType() != mObjects[neighborPositionString]->GetComponent<MeshComponent>()->GetMeshType())
            {
                if (meshComponent->GetMeshType() == MeshType::WATER)
                {
                    // meshComponent->SetVisibility(neighbor.first, false);
                    // mObjects[neighborPositionString]->GetComponent<MeshComponent>()->SetVisibility(mOppositeSides.at(neighbor.first), true);
                    meshComponent->RemoveVisibleSide(neighbor.first);
                    mObjects[neighborPositionString]->GetComponent<MeshComponent>()->AddVisibleSide(mOppositeSides.at(neighbor.first));
                }
                else if (meshComponent->GetMeshType() == MeshType::CUBE)
                {
                    meshComponent->AddVisibleSide(neighbor.first);
                    mObjects[neighborPositionString]->GetComponent<MeshComponent>()->RemoveVisibleSide(mOppositeSides.at(neighbor.first));
                    // meshComponent->SetVisibility(neighbor.first, true);
                    // mObjects[neighborPositionString]->GetComponent<MeshComponent>()->SetVisibility(mOppositeSides.at(neighbor.first), false);
                }
            }
            else
            {
                meshComponent->RemoveVisibleSide(neighbor.first);
                mObjects[neighborPositionString]->GetComponent<MeshComponent>()->RemoveVisibleSide(mOppositeSides.at(neighbor.first));
                // mObjects[neighborPositionString]->GetComponent<MeshComponent>()->SetVisibility(mOppositeSides.at(neighbor.first), false);
                // meshComponent->SetVisibility(neighbor.first, false);
            }
        }
        else
        {
            // meshComponent->SetVisibility(neighbor.first, true);
            meshComponent->AddVisibleSide(neighbor.first);
        }
    }

    // insert object into map
    std::string positionString = transformComponent->GetPositionString();
    mObjects[positionString] = object;

    if (meshComponent->GetMeshType() == MeshType::WATER)
    {
        mSortedTransparentObjects.insert(object);
    }
}

void ObjectManager::RemoveObject(const std::shared_ptr<GameObject> &object)
{
    // mObjects.erase(std::remove(mObjects.begin(), mObjects.end(), object), mObjects.end());
    for (auto it = mObjects.begin(); it != mObjects.end();)
    {
        if (it->second == object)
        {
            // adjust neighbor visibility
            TransformComponent *transformComponent = object->GetComponent<TransformComponent>();
            for (auto neighbor : mNeighbors)
            {
                glm::vec3 neighborPosition = transformComponent->GetPosition() + neighbor.second;
                std::string neighborPositionString = std::to_string(neighborPosition.x) + ", " + std::to_string(neighborPosition.y) + ", " + std::to_string(neighborPosition.z);
                if (mObjects.find(neighborPositionString) != mObjects.end())
                {
                    mObjects[neighborPositionString]->GetComponent<MeshComponent>()->SetVisibility(mOppositeSides.at(neighbor.first), true);
                }
            }

            it = mObjects.erase(it);
        }
        else
        {
            ++it;
        }
    }

    MeshComponent *meshComponent = object->GetComponent<MeshComponent>();

    if (meshComponent->GetMeshType() == MeshType::WATER)
    {
        mSortedTransparentObjects.erase(object);
    }
}

void ObjectManager::UpdateSortedTransparentObjects()
{
    mSortedTransparentObjects.clear();
    for (auto &pair : mObjects)
    {
        MeshComponent *meshComponent = pair.second->GetComponent<MeshComponent>();
        if (meshComponent->GetMeshType() == MeshType::WATER)
        {
            mSortedTransparentObjects.insert(pair.second);
        }
    }
}


const std::set<std::shared_ptr<GameObject>, TransparentObjectComparator> &ObjectManager::GetTransparentObjects()
{
    return mSortedTransparentObjects;
}

const std::unordered_map<std::string, std::shared_ptr<GameObject>> &ObjectManager::GetObjects()
{
    return mObjects;
}
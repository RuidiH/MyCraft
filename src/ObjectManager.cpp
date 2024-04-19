#include "ObjectManager.hpp"
#include "MeshComponent.hpp"
#include "TransformComponent.hpp"

#include <algorithm>
#include <iostream>

ObjectManager::ObjectManager(const Camera &camera, const bool &isRunning) : mCamera(camera), mSortedTransparentObjects(TransparentObjectComparator(camera)), mIsRunning(isRunning)
{
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

    meshComponent->Init();

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
                    meshComponent->RemoveVisibleSide(neighbor.first);
                    mObjects[neighborPositionString]->GetComponent<MeshComponent>()->AddVisibleSide(mOppositeSides.at(neighbor.first));
                }
                else if (meshComponent->GetMeshType() == MeshType::CUBE)
                {
                    meshComponent->AddVisibleSide(neighbor.first);
                    mObjects[neighborPositionString]->GetComponent<MeshComponent>()->RemoveVisibleSide(mOppositeSides.at(neighbor.first));
                }
            }
            else
            {
                meshComponent->RemoveVisibleSide(neighbor.first);
                mObjects[neighborPositionString]->GetComponent<MeshComponent>()->RemoveVisibleSide(mOppositeSides.at(neighbor.first));
            }
        }
        else
        {
            meshComponent->AddVisibleSide(neighbor.first);
        }
    }

    // insert object into map
    std::string positionString = transformComponent->GetPositionString();

    if (object->GetID() == "grass_block" && mObjects.find(positionString) != mObjects.end())
    {
        std::cout << "adding grass block thats already in object manager\n";
        mObjects.erase(positionString);
        mObjects.insert({positionString, object});
    }
    else
    {
        mObjects.insert({positionString, object});
    }

    // mObjects.insert({positionString, object});

    if (meshComponent->GetMeshType() == MeshType::WATER)
    {
        mSortedTransparentObjects.insert(object);
    }
}

void ObjectManager::RemoveObject(const std::shared_ptr<GameObject> &object)
{
    if (mObjects.find(object->GetComponent<TransformComponent>()->GetPositionString()) == mObjects.end())
    {
        return;
    }

    // adjust neighbor visibility
    TransformComponent *transformComponent = object->GetComponent<TransformComponent>();
    for (auto neighbor : mNeighbors)
    {
        glm::vec3 neighborPosition = transformComponent->GetPosition() + neighbor.second;
        std::string neighborPositionString = std::to_string(neighborPosition.x) + ", " + std::to_string(neighborPosition.y) + ", " + std::to_string(neighborPosition.z);
        if (mObjects.find(neighborPositionString) != mObjects.end())
        {
            mObjects[neighborPositionString]->GetComponent<MeshComponent>()->AddVisibleSide(mOppositeSides.at(neighbor.first));
        }
    }

    mObjects.erase(transformComponent->GetPositionString());
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
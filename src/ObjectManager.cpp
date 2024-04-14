#include "ObjectManager.hpp"
#include "MeshComponent.hpp"

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

    mObjects.push_back(object);
    if (meshComponent->GetMeshType() == MeshType::WATER)
    {
        mSortedTransparentObjects.insert(object);
    }
}

void ObjectManager::RemoveObject(const std::shared_ptr<GameObject> &object)
{
    mObjects.erase(std::remove(mObjects.begin(), mObjects.end(), object), mObjects.end());

    MeshComponent *meshComponent = object->GetComponent<MeshComponent>();

    if (meshComponent->GetMeshType() == MeshType::WATER)
    {
        mSortedTransparentObjects.erase(object);
    }
}

void ObjectManager::UpdateSortedTransparentObjects()
{
    mSortedTransparentObjects.clear();
    for (auto object : mObjects)
    {
        MeshComponent *meshComponent = object->GetComponent<MeshComponent>();
        if (meshComponent->GetMeshType() == MeshType::WATER)
        {
            mSortedTransparentObjects.insert(object);
        }
    }
}

// void ObjectManager::AddSolidObject(std::shared_ptr<GameObject> object)
// {
//     mSolidObjects->push_back(object);
//     mObjects->push_back(object);
// }

// void ObjectManager::AddTransparentObject(std::shared_ptr<GameObject> object)
// {
//     mTransparentObjects->push_back(object);
//     mObjects->push_back(object);
// }

// void ObjectManager::AddObject(std::shared_ptr<GameObject> object)
// {
//     MeshComponent *meshComponent = object->GetComponent<MeshComponent>();

//     if (meshComponent == nullptr) {
//         std::cout << "Object does not have a mesh component\n";
//         return;
//     }

//     if (meshComponent->GetMeshType() == MeshType::CUBE)
//     {
//         mSolidObjects->push_back(object);
//     }
//     else if (meshComponent->GetMeshType() == MeshType::WATER)
//     {
//         mTransparentObjects->push_back(object);
//     } else {
//         std::cout << "Mesh type not supported\n";
//     }
//     mObjects->push_back(object);
// }

// void ObjectManager::RemoveSolidObject(std::shared_ptr<GameObject> object)
// {
//     auto it = std::find(mSolidObjects->begin(), mSolidObjects->end(), object);
//     if (it != mSolidObjects->end())
//     {
//         mSolidObjects->erase(it);
//     }
//     it = std::find(mObjects->begin(), mObjects->end(), object);
//     if (it != mObjects->end())
//     {
//         mObjects->erase(it);
//     }
// }

// void ObjectManager::RemoveTransparentObject(std::shared_ptr<GameObject> object)
// {
//     auto it = std::find(mTransparentObjects->begin(), mTransparentObjects->end(), object);
//     if (it != mTransparentObjects->end())
//     {
//         mTransparentObjects->erase(it);
//     }
//     it = std::find(mObjects->begin(), mObjects->end(), object);
//     if (it != mObjects->end())
//     {
//         mObjects->erase(it);
//     }
// }

// void ObjectManager::RemoveObject(std::shared_ptr<GameObject> object)
// {
//     auto it = std::find(mSolidObjects->begin(), mSolidObjects->end(), object);
//     if (it != mSolidObjects->end())
//     {
//         mSolidObjects->erase(it);
//     }

//     it = std::find(mTransparentObjects->begin(), mTransparentObjects->end(), object);
//     if (it != mTransparentObjects->end())
//     {
//         mTransparentObjects->erase(it);
//     }

//     it = std::find(mObjects->begin(), mObjects->end(), object);
//     if (it != mObjects->end())
//     {
//         mObjects->erase(it);
//     }
// }

const std::set<std::shared_ptr<GameObject>, TransparentObjectComparator>& ObjectManager::GetTransparentObjects()
{
    return mSortedTransparentObjects;
}

const std::vector<std::shared_ptr<GameObject>>& ObjectManager::GetObjects()
{
    return mObjects;
}
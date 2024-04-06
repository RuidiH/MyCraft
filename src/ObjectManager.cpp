#include "ObjectManager.hpp"
#include "MeshComponent.hpp"

#include <algorithm>
#include <iostream>

ObjectManager::ObjectManager(std::shared_ptr<Camera> camera) : mCamera(camera)
{
    mSolidObjects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>();
    mTransparentObjects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>();
    mObjects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>();
}

ObjectManager::~ObjectManager()
{
    mSolidObjects->clear();
    mTransparentObjects->clear();
}

void ObjectManager::AddSolidObject(std::shared_ptr<GameObject> object)
{
    mSolidObjects->push_back(object);
    mObjects->push_back(object);
}

void ObjectManager::AddTransparentObject(std::shared_ptr<GameObject> object)
{
    mTransparentObjects->push_back(object);
    mObjects->push_back(object);
}

void ObjectManager::AddObject(std::shared_ptr<GameObject> object)
{
    MeshComponent *meshComponent = object->GetComponent<MeshComponent>(); 

    if (meshComponent == nullptr) {
        std::cout << "Object does not have a mesh component\n";
        return;
    }

    if (meshComponent->GetMeshType() == MeshType::CUBE)
    {
        mSolidObjects->push_back(object);
    }
    else if (meshComponent->GetMeshType() == MeshType::WATER)
    {
        mTransparentObjects->push_back(object);
    } else {
        std::cout << "Mesh type not supported\n";
    }  
    mObjects->push_back(object);
}

void ObjectManager::RemoveSolidObject(std::shared_ptr<GameObject> object)
{
    auto it = std::find(mSolidObjects->begin(), mSolidObjects->end(), object);
    if (it != mSolidObjects->end())
    {
        mSolidObjects->erase(it);
    }
    it = std::find(mObjects->begin(), mObjects->end(), object);
    if (it != mObjects->end())
    {
        mObjects->erase(it);
    }
}

void ObjectManager::RemoveTransparentObject(std::shared_ptr<GameObject> object)
{
    auto it = std::find(mTransparentObjects->begin(), mTransparentObjects->end(), object);
    if (it != mTransparentObjects->end())
    {
        mTransparentObjects->erase(it);
    }
    it = std::find(mObjects->begin(), mObjects->end(), object);
    if (it != mObjects->end())
    {
        mObjects->erase(it);
    }
}

void ObjectManager::RemoveObject(std::shared_ptr<GameObject> object)
{
    auto it = std::find(mSolidObjects->begin(), mSolidObjects->end(), object);
    if (it != mSolidObjects->end())
    {
        mSolidObjects->erase(it);
    }

    it = std::find(mTransparentObjects->begin(), mTransparentObjects->end(), object);
    if (it != mTransparentObjects->end())
    {
        mTransparentObjects->erase(it);
    }

    it = std::find(mObjects->begin(), mObjects->end(), object);
    if (it != mObjects->end())
    {
        mObjects->erase(it);
    }
}

std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> ObjectManager::GetSolidObjects()
{
    return mSolidObjects;
}

std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> ObjectManager::GetTransparentObjects()
{
    return mTransparentObjects;
}

std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> ObjectManager::GetObjects()
{
    return mObjects;
}
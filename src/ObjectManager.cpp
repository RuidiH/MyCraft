#include "ObjectManager.hpp"
#include "MeshComponent.hpp"
#include "TransformComponent.hpp"

#include <algorithm>
#include <iostream>

ObjectManager::ObjectManager(const Camera &camera, const bool &isRunning) : mCamera(camera), mSortedTransparentObjects(TransparentObjectComparator(camera)), mIsRunning(isRunning)
{
    mSolidObjectVertices = std::make_shared<std::unordered_map<std::string, std::vector<float>>>();
    mTransparentObjectVertices = std::make_shared<std::unordered_map<std::string, std::vector<float>>>();

    float radius = 1.0 / 2.0;

    // solid object data
    // vertex data structure:
    // x, y, z, u, v, nx, ny, nz

    // initialize vertex data map
    std::vector<float> sTop{

        // Top
        -radius, radius, -radius, 0.0f, 0.0f, 0.f, 1.f, 0.f, // - + -
        -radius, radius, radius, 0.0f, 1.0f, 0.f, 1.f, 0.f,  // - + +
        radius, radius, radius, 1.0f, 1.0f, 0.f, 1.f, 0.f,   // + + +
        radius, radius, -radius, 1.0f, 0.0f, 0.f, 1.f, 0.f   // + + -
    };

    std::vector<float> sBottom{
        // Bottom
        -radius, -radius, radius, 0.0f, 1.0f, 0.f, -1.f, 0.f,  // - - +
        -radius, -radius, -radius, 0.0f, 0.0f, 0.f, -1.f, 0.f, // - - -
        radius, -radius, -radius, 1.0f, 0.0f, 0.f, -1.f, 0.f,  // + - -
        radius, -radius, radius, 1.0f, 1.0f, 0.f, -1.f, 0.f    // + - +
    };

    std::vector<float> sLeft{
        // Left
        -radius, radius, -radius, 0.0f, 0.0f, -1.f, 0.f, 0.f,  // - + -
        -radius, -radius, -radius, 0.0f, 1.0f, -1.f, 0.f, 0.f, // - - -
        -radius, -radius, radius, 1.0f, 1.0f, -1.f, 0.f, 0.f,  // - - +
        -radius, radius, radius, 1.0f, 0.0f, -1.f, 0.f, 0.f    // - + +
    };

    std::vector<float> sRight{
        // Right
        radius, radius, radius, 0.0f, 0.0f, 1.f, 0.f, 0.f,   // + + +
        radius, -radius, radius, 0.0f, 1.0f, 1.f, 0.f, 0.f,  // + - +
        radius, -radius, -radius, 1.0f, 1.0f, 1.f, 0.f, 0.f, // + - -
        radius, radius, -radius, 1.0f, 0.0f, 1.f, 0.f, 0.f   // + + -
    };

    std::vector<float> sFront{
        // Front
        -radius, radius, radius, 0.0f, 0.0f, 0.f, 0.f, 1.f,  // - + +
        -radius, -radius, radius, 0.0f, 1.0f, 0.f, 0.f, 1.f, // - - +
        radius, -radius, radius, 1.0f, 1.0f, 0.f, 0.f, 1.f,  // + - +
        radius, radius, radius, 1.0f, 0.0f, 0.f, 0.f, 1.f    // + + +
    };

    std::vector<float> sBack{
        // Back
        -radius, -radius, -radius, 1.0f, 1.0f, 0.f, 0.f, -1.f, // - - -
        -radius, radius, -radius, 1.0f, 0.0f, 0.f, 0.f, -1.f,  // - + -
        radius, radius, -radius, 0.0f, 0.0f, 0.f, 0.f, -1.f,   // + + -
        radius, -radius, -radius, 0.0f, 1.0f, 0.f, 0.f, -1.f   // + - -
    };

    mSolidObjectVertices->insert({"top", sTop});
    mSolidObjectVertices->insert({"bottom", sBottom});
    mSolidObjectVertices->insert({"left", sLeft});
    mSolidObjectVertices->insert({"right", sRight});
    mSolidObjectVertices->insert({"front", sFront});
    mSolidObjectVertices->insert({"back", sBack});

    // water data
    // x, y, z, r, g, b, nx, ny, nz
    float r = 0.0f, g = 0.5f, b = 1.0f;

    // initialize vertex data map
    std::vector<float> tTop{
        // Top
        -radius, radius, -radius, r, g, b, 0.f, 1.f, 0.f, // - + -
        -radius, radius, radius, r, g, b, 0.f, 1.f, 0.f,  // - + +
        radius, radius, radius, r, g, b, 0.f, 1.f, 0.f,   // + + +
        radius, radius, -radius, r, g, b, 0.f, 1.f, 0.f   // + + -
    };

    std::vector<float> tBottom{
        // Bottom
        -radius, -radius, radius, r, g, b, 0.f, -1.f, 0.f,  // - - +
        -radius, -radius, -radius, r, g, b, 0.f, -1.f, 0.f, // - - -
        radius, -radius, -radius, r, g, b, 0.f, -1.f, 0.f,  // + - -
        radius, -radius, radius, r, g, b, 0.f, -1.f, 0.f    // + - +
    };

    std::vector<float> tLeft{
        // Left
        -radius, radius, -radius, r, g, b, -1.f, 0.f, 0.f,  // - + -
        -radius, -radius, -radius, r, g, b, -1.f, 0.f, 0.f, // - - -
        -radius, -radius, radius, r, g, b, -1.f, 0.f, 0.f,  // - - +
        -radius, radius, radius, r, g, b, -1.f, 0.f, 0.f    // - + +
    };

    std::vector<float> tRight{
        // Right
        radius, radius, radius, r, g, b, 1.f, 0.f, 0.f,   // + + +
        radius, -radius, radius, r, g, b, 1.f, 0.f, 0.f,  // + - +
        radius, -radius, -radius, r, g, b, 1.f, 0.f, 0.f, // + - -
        radius, radius, -radius, r, g, b, 1.f, 0.f, 0.f   // + + -
    };

    std::vector<float> tFront{
        // Front
        -radius, radius, radius, r, g, b, 0.f, 0.f, 1.f,  // - + +
        -radius, -radius, radius, r, g, b, 0.f, 0.f, 1.f, // - - +
        radius, -radius, radius, r, g, b, 0.f, 0.f, 1.f,  // + - +
        radius, radius, radius, r, g, b, 0.f, 0.f, 1.f    // + + +
    };

    std::vector<float> tBack{
        // Back
        -radius, -radius, -radius, r, g, b, 0.f, 0.f, -1.f, // - - -
        -radius, radius, -radius, r, g, b, 0.f, 0.f, -1.f,  // - + -
        radius, radius, -radius, r, g, b, 0.f, 0.f, -1.f,   // + + -
        radius, -radius, -radius, r, g, b, 0.f, 0.f, -1.f   // + - -
    };

    mTransparentObjectVertices->insert({"top", tTop});
    mTransparentObjectVertices->insert({"bottom", tBottom});
    mTransparentObjectVertices->insert({"left", tLeft});
    mTransparentObjectVertices->insert({"right", tRight});
    mTransparentObjectVertices->insert({"front", tFront});
    mTransparentObjectVertices->insert({"back", tBack});
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

    if (meshComponent->GetMeshType() == MeshType::CUBE)
    {
        meshComponent->Init(mSolidObjectVertices);
    }
    else
    {
        meshComponent->Init(mTransparentObjectVertices);
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
            // if (mIsRunning)
            // {
            //     UpdateObjectFaces(object);
            //     UpdateObjectFaces(mObjects[neighborPositionString]);
            // }
        }
        else
        {
            meshComponent->AddVisibleSide(neighbor.first);
            // if (mIsRunning)
            // {
            //     UpdateObjectFaces(object);
            // }
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
                    mObjects[neighborPositionString]->GetComponent<MeshComponent>()->AddVisibleSide(mOppositeSides.at(neighbor.first)); 
                    // UpdateObjectFaces(mObjects[neighborPositionString]);
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

void ObjectManager::LoadObjectFaces()
{
    for (auto &pair : mObjects)
    {
        for (const auto &face : *pair.second->GetComponent<MeshComponent>()->GetVisibleSides())
        {
            pair.second->GetComponent<MeshComponent>()->GetMesh()->LoadFace(face);
        }
    }
}

void ObjectManager::UpdateObjectFaces(std::shared_ptr<GameObject> object)
{
    for (auto &pair : mObjects)
    {
        if (pair.second == object)
        {
            for (const auto &face : *pair.second->GetComponent<MeshComponent>()->GetVisibleSides())
            {
                pair.second->GetComponent<MeshComponent>()->GetMesh()->LoadFace(face);
            }
        }
    }
}
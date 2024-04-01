#include "Engine.hpp"
#include "MeshComponent.hpp"
#include "TransformComponent.hpp"
#include "TextureComponent.hpp"
#include "Mesh.hpp"
#include "CubeMesh.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>
// #include <glm/gtx/rotate_vector.hpp>

#include <array>
#include <algorithm>

Uint32 Engine::deltaTime;

Engine::Engine(int width, int height) : mScreenWidth(width), mScreenHeight(height)
{
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<< Initializing Engine >>>>>>>>>>>>>>>>>>>>>>>>>\n";
    InitializeGraphicsProgram();

    CreateGraphicsPipeline();

    InitializeShadowMap();

    mGameObjects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>();

    // hard-coded ortho light
    glm::mat4 orthoProjection = glm::ortho(-15.f, 15.f, -15.f, 15.f, 1.f, 35.f);
    glm::mat4 lightView = glm::lookAt(glm::vec3(3.f, 3.f, 3.f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    mLightProjection = orthoProjection * lightView;

    // hard-coded crosshair
    float aspectRatio = (float)mScreenWidth / (float)mScreenHeight;
    mCrosshairVertices = {
        -0.03f, 0.0f * aspectRatio, 0.0f, // Left point
        0.03f, 0.0f * aspectRatio, 0.0f,  // Right point
        0.0f, -0.03f * aspectRatio, 0.0f, // Bottom point
        0.0f, 0.03f * aspectRatio, 0.0f   // Top point}
    };
    glGenVertexArrays(1, &mCrosshairVAO);
    glBindVertexArray(mCrosshairVAO);
    glGenBuffers(1, &mCrosshairVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mCrosshairVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mCrosshairVertices.size(), mCrosshairVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<< Engine initialized >>>>>>>>>>>>>>>>>>>>>>>>>\n\n\n";
}

Engine::~Engine()
{
    // TODO: clear all pointer reference
}

void Engine::SetupObject()
{
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<< Setting up objects >>>>>>>>>>>>>>>>>>>>>>>>>\n";
    mTextureManager = std::make_shared<TextureManager>();
    mWorldSerializer = std::make_shared<WorldSerializer>(mTextureManager, mGameObjects);
    mWorldSerializer->ReadBlockTypes("./config/blockTypes.json");
    mWorldSerializer->LoadTextureConfig("./config/textureConfig.json");
    mWorldSerializer->CreateBlocks("./config/worldData.json");
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<< Objects setup complete >>>>>>>>>>>>>>>>>>>>>>>>>\n\n\n";
}

void Engine::MainLoop()
{
    while (!mQuit)
    {
        FrameCapping();
        Input();

        // Update game here
        Update();

        Render();

        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
    }
}

void Engine::Input()
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        {
            std::cout << "Goodbye!" << std::endl;
            mQuit = true;
            break;
        }
        if (e.type == SDL_MOUSEMOTION)
        {
            mCamera.RotateCamera(e.motion.xrel, e.motion.yrel);
        }

        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            if (e.key.keysym.sym == SDLK_s)
            {
                const Uint8 *state = SDL_GetKeyboardState(NULL);
                // Check if either CTRL key is pressed
                if (state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL])
                {
                    // Save world
                    mWorldSerializer->SaveWorldData("./config/worldData.json");
                }
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                // Right mouse button was clicked
                AddObject();
            }
            else if (e.button.button == SDL_BUTTON_LEFT)
            {
                RemoveObject();
            }
        }
    }

    // Retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // camera controls
    if (state[SDL_SCANCODE_W])
    {
        mCamera.MoveForward(0.1f);
    }
    else if (state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_LCTRL])
    {
        mCamera.MoveBackward(0.1f);
    }

    if (state[SDL_SCANCODE_A])
    {
        mCamera.MoveLeft(0.1f);
    }
    else if (state[SDL_SCANCODE_D])
    {
        mCamera.MoveRight(0.1f);
    }

    // ray casting test
    FindSelectedObject();

    // switch type of object to make
    if (state[SDL_SCANCODE_1])
    {
        mNewObjectTextureGroup = "dirt";
        mNewObjectID = "dirt_block";
    }
    else if (state[SDL_SCANCODE_2])
    {
        mNewObjectTextureGroup = "grass";
        mNewObjectID = "grass_block";
    }
    else if (state[SDL_SCANCODE_3])
    {
        mNewObjectTextureGroup = "snow";
        mNewObjectID = "snow_block";
    }
}

void Engine::RemoveObject()
{
    // Left mouse button was clicked
    if (mSelected != nullptr)
    {

        mGameObjects->erase(std::remove(mGameObjects->begin(), mGameObjects->end(), mSelected), mGameObjects->end());
        mSelected = nullptr;
        mSelectedFace = "none";
    }
}

void Engine::AddObject()
{
    if (mSelected != nullptr && mSelectedFace != "none")
    {
        MeshComponent *mesh = mSelected->GetComponent<MeshComponent>();
        glm::vec3 normal = mesh->GetMesh()->GetSideNormal(mSelectedFace);
        glm::vec3 placementPos = mSelected->GetComponent<TransformComponent>()->GetPosition() + normal;

        // make sure the placement position is not occupied
        bool isOccupied = false;
        for (auto gameObject : *mGameObjects)
        {
            if (gameObject != mSelected)
            {
                MeshComponent *mesh = gameObject->GetComponent<MeshComponent>();
                if (mesh->GetMeshType() == MeshType::CUBE)
                {
                    CubeMesh *cube = static_cast<CubeMesh *>(mesh->GetMesh().get());
                    if (glm::distance(placementPos, cube->GetMinCorner()) < 0.1f)
                    {
                        isOccupied = true;
                        break;
                    }
                }
                // TODO: handle other types of mesh
            }
        }
        if (!isOccupied)
        {
            std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(mNewObjectID);
            obj->AddComponent<TransformComponent>()->SetPosition(placementPos);
            // newCube->AddComponent<MeshComponent>()->AddCube();
            obj->AddComponent<MeshComponent>()->AddMesh(MeshType::CUBE);
            obj->AddComponent<TextureComponent>()->SetTextureGroupName(mNewObjectTextureGroup);
            obj->GetComponent<TextureComponent>()->SetTextureGroup(mTextureManager->GetTextureGroup(mNewObjectTextureGroup));
            mGameObjects->push_back(obj);
            std::cout << "New cube placed at " << placementPos.x << " " << placementPos.y << " " << placementPos.z << std::endl;
        }
        else
        {
            std::cout << "Placement position is occupied" << std::endl;
        }
    }
}

void Engine::FindSelectedObject()
{
    glm::vec3 rayDir = mCamera.GetRayDirection(mScreenWidth, mScreenHeight, glm::vec2(mScreenWidth / 2, mScreenHeight / 2));
    // std::cout << "Ray direction: " << rayDir.x << " " << rayDir.y << " " << rayDir.z << std::endl;
    float smallestTNear = -1.f;
    std::string clostestHitSide;
    std::shared_ptr<GameObject> hitObject;
    for (std::shared_ptr<GameObject> gameObject : *mGameObjects)
    {
        std::shared_ptr<CubeMesh> mesh;
        if (gameObject->GetComponent<MeshComponent>()->GetMeshType() == MeshType::CUBE)
        {
            mesh = std::static_pointer_cast<CubeMesh>(gameObject->GetComponent<MeshComponent>()->GetMesh());
        } else {
            std::cout << "Mesh type not recognized by ray caster!" << std::endl;
            continue;
        }

        float tNear, tFar = 0.f;
        std::string hitSide;
        bool result = RayCastTest(mCamera.GetPosition(), rayDir, mesh->GetCorners(), tNear, tFar, hitSide);

        // std::cout << "tNear: " << tNear << " tFar: " << tFar << std::endl;

        // keep track of closest colliding object
        if (result)
        {
            // base case for ray cast test
            if (smallestTNear == -1.f)
            {
                smallestTNear = tNear;
                hitObject = gameObject;
                clostestHitSide = hitSide;
                continue;
            }

            if (tNear < smallestTNear)
            {
                smallestTNear = tNear;
                hitObject = gameObject;
                clostestHitSide = hitSide;
            }
        }
    }
    if (hitObject != nullptr)
    {
        // std::cout << "Hit side: " << clostestHitSide << std::endl;
        mSelectedFace = clostestHitSide;
        mSelected = hitObject;
    }
    else
    {
        mSelected = nullptr;
        mSelectedFace = "none";
    }
}

void Engine::Update()
{
    for (auto gameObject : *mGameObjects)
    {
        gameObject->Update();
    }
}

void Engine::Render()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_STENCIL_TEST);

    ShadowPass();
    LightPass();
    HighlightPass();
    CrosshairPass();
    glUseProgram(0);
}

void Engine::ShadowPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFBO);
    glViewport(0, 0, mShadowResolution.x, mShadowResolution.y);
    glClear(GL_DEPTH_BUFFER_BIT);

    mShadowShader.Use();

    mShadowShader.SetUniform("lightProjection", mLightProjection);

    // render
    for (auto gameObject : *mGameObjects)
    {
        glm::mat4 model = gameObject->GetComponent<TransformComponent>()->GetModelMatrix();
        mShadowShader.SetUniform("model", model);
        gameObject->Render();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::LightPass()
{
    glViewport(0, 0, mScreenWidth, mScreenHeight);

    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    mMainShader.Use();

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, mShadowMapTexture);

    mMainShader.SetUniform("shadowMap", 1);

    mMainShader.SetUniform("u_Projection", mCamera.GetProjectionMatrix());

    // set view matrix
    mCamera.UpdateViewMatrix();
    mMainShader.SetUniform("u_View", mCamera.GetViewMatrix());

    // set lightings
    mMainShader.SetUniform("lightPos", glm::vec3(3.0f, 3.0f, 3.0f));
    mMainShader.SetUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    mMainShader.SetUniform("viewPos", mCamera.GetPosition());
    mMainShader.SetUniform("u_LightProjection", mLightProjection);

    for (auto &gameObject : *mGameObjects)
    {
        glm::mat4 model = gameObject->GetComponent<TransformComponent>()->GetModelMatrix();
        mMainShader.SetUniform("u_ModelMatrix", model);

        if (gameObject == mSelected)
        {
            glEnable(GL_STENCIL_TEST);
            glStencilMask(0xFF);                       // Enable writing to the stencil buffer.
            glStencilFunc(GL_ALWAYS, 1, 0xFF);         // Always pass stencil test, write 1 to stencil buffer.
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Replace stencil buffer value on depth pass.
        }
        else
        {
            glStencilMask(0x00); // Disable writing to the stencil buffer.
        }

        gameObject->Render();
    }
}

void Engine::HighlightPass()
{
    for (auto &gameObject : *mGameObjects)
    {
        if (mSelected != nullptr && gameObject == mSelected)
        {
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00); // Disable writing to the stencil buffer.
            glDisable(GL_DEPTH_TEST);

            mHighlightShader.Use();
            mHighlightShader.SetUniform("projection", mCamera.GetProjectionMatrix());
            mCamera.UpdateViewMatrix();
            mHighlightShader.SetUniform("view", mCamera.GetViewMatrix());
            glm::mat4 model = gameObject->GetComponent<TransformComponent>()->GetModelMatrix();
            model = glm::scale(model, glm::vec3(1.05f, 1.05f, 1.05f));
            mHighlightShader.SetUniform("model", model);
            gameObject->Render();

            glEnable(GL_DEPTH_TEST);
            glStencilMask(0xFF);        // Enable writing to the stencil buffer.
            glDisable(GL_STENCIL_TEST); // Disable stencil test.
        }
    }
}

void Engine::CrosshairPass()
{
    glDisable(GL_DEPTH_TEST); // Disable depth testing for HUD elements

    // Draw crosshair
    mCrosshairShader.Use();
    glBindVertexArray(mCrosshairVAO);
    glDrawArrays(GL_LINES, 0, 4);

    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST); // Re-enable depth testing if there are subsequent rendering operations
}

void Engine::Shutdown()
{
    SDL_StopTextInput();
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

void Engine::FrameCapping()
{
    auto current = SDL_GetTicks();
    deltaTime = current - prevTime;
    float expectation = 1000.0 / 60.0;
    if (deltaTime < expectation)
    {
        SDL_Delay(expectation - (deltaTime));
        deltaTime += (expectation - (deltaTime));
    }
    prevTime = current;
}

void Engine::SetTime()
{
    prevTime = SDL_GetTicks();
}

void Engine::InitializeGraphicsProgram()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL2 cout not initialize video subsystem" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    gGraphicsApplicationWindow = SDL_CreateWindow("Window", 400, 300, mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);
    if (gGraphicsApplicationWindow == nullptr)
    {
        std::cout << "SDL_Window not created" << std::endl;
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
    if (gOpenGLContext == nullptr)
    {
        std::cout << "OpenGL context not available\n";
        exit(1);
    }

    // initialize GLAD Library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "glad was not initialized" << std::endl;
        exit(1);
    }

    GetOpenGLVersionInfo();

    mCamera.SetPosition(glm::vec3(0.f, 5.f, 5.f));
    mCamera.SetUpVector(glm::vec3(0.f, 1.f, 0.f));
    mCamera.SetDirection(glm::normalize(glm::vec3(0.f, 0.f, 0.f) - mCamera.GetPosition()));
    mCamera.SetProjectionMatrix(45.0f, (float)mScreenWidth / (float)mScreenHeight, 0.1f, 50.0f);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    // enable depth testing and culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glViewport(0, 0, mScreenWidth, mScreenHeight);
}

void Engine::InitializeShadowMap()
{

    glGenFramebuffers(1, &mShadowMapFBO);

    // Create a depth texture for the shadow map
    glGenTextures(1, &mShadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, mShadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mShadowResolution.x, mShadowResolution.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attach the depth texture to the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::CreateGraphicsPipeline()
{

    mMainShader.Init("./shaders/vert.glsl", "./shaders/frag.glsl");
    mShadowShader.Init("./shaders/shadow_v.glsl", "./shaders/shadow_f.glsl");
    mCrosshairShader.Init("./shaders/crosshair_v.glsl", "./shaders/crosshair_f.glsl");
    mHighlightShader.Init("./shaders/highlight_v.glsl", "./shaders/highlight_f.glsl");
    // mQuadShader.Init("./shaders/quad_v.glsl", "./shaders/quad_f.glsl");
}

void Engine::GetOpenGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

bool Engine::RayCastTest(const glm::vec3 origin, const glm::vec3 direction, const std::array<glm::vec3, 2> &box, float &tNear, float &tFar, std::string &hitSide)
{
    glm::vec3 minCorner = box[0];
    glm::vec3 maxCorner = box[1];

    glm::vec3 invDir = 1.0f / direction;
    glm::vec3 tMin = (minCorner - origin) * invDir;
    glm::vec3 tMax = (maxCorner - origin) * invDir;

    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);

    tNear = std::max(std::max(t1.x, t1.y), t1.z);
    tFar = std::min(std::min(t2.x, t2.y), t2.z);

    float tHit = tNear; // The time at which the ray hits the cube

    if (tNear > tFar)
    {
        return false;
    }

    // Check which axis had the intersection and determine the direction
    if (tHit == t1.x)
    {
        hitSide = direction.x < 0 ? "right" : "left";
    }
    else if (tHit == t1.y)
    {
        hitSide = direction.y < 0 ? "top" : "bottom";
    }
    else if (tHit == t1.z)
    {
        hitSide = direction.z < 0 ? "front" : "back";
    }

    // std::cout << "Hit side: " << hitSide << std::endl;

    return tNear <= tFar;
}

std::string Engine::LoadShaderAsString(const std::string &filename)
{
    std::string result = "";

    std::string line = "";
    std::ifstream myFile(filename.c_str());

    if (myFile.is_open())
    {
        while (std::getline(myFile, line))
        {
            result += line + '\n';
        }
        myFile.close();
    }
    return result;
}

/**
 * @brief returns camera angles for pointing camera to the lookAtPos
 *
 * @param cameraPos camera position
 * @param lookAtPos where the camera should look at
 * @return glm::vec3 camera angles
 */

glm::vec3 Engine::CalculateCameraAngles(const glm::vec3 &cameraPos, const glm::vec3 &lookAtPos)
{
    glm::vec3 dir = glm::normalize(lookAtPos - cameraPos);

    float radius = glm::distance(cameraPos, lookAtPos);

    float theta = std::atan2(dir.z, dir.x);
    float phi = std::acos(dir.y / radius);

    // Adjust theta to be in the range [0, 2PI]
    if (theta < 0.0f)
    {
        theta += glm::two_pi<float>();
    }

    return glm::vec3(theta, phi, radius);
}

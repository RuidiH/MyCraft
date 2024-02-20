#include "Engine.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>
// #include <glm/gtx/rotate_vector.hpp>

Uint32 Engine::deltaTime;

Engine::Engine(int width, int height) : mScreenWidth(width), mScreenHeight(height)
{
    InitializeGraphicsProgram();

    CreateGraphicsPipeline();

    InitializeShadowMap();

    // hard-coded ortho light
    glm::mat4 orthoProjection = glm::ortho(-15.f, 15.f, -15.f, 15.f, 1.f, 35.f);
    glm::mat4 lightView = glm::lookAt(glm::vec3(3.f, 3.f, 3.f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    mLightProjection = orthoProjection * lightView;
}

Engine::~Engine()
{
    // TODO: clear all pointer reference 
}

void Engine::SetupObject()
{
    mTextureManager.LoadTexture("dirt", "./assets/texture/dirt.png");
    mTextureManager.LoadTexture("grass_carried", "./assets/texture/grass_carried.png");
    mTextureManager.LoadTexture("grass_side_carried", "./assets/texture/grass_side_carried.png");
    mTextureManager.LoadTexture("grass_side_snowed", "./assets/texture/grass_side_snowed.png");
    mTextureManager.LoadTexture("snow", "./assets/texture/snow.png");

    std::shared_ptr<GameObject> grass = std::make_shared<GameObject>();
    std::shared_ptr<Cube> cube1 = std::make_shared<Cube>();
    cube1->SetFaceTexture("top", mTextureManager.GetTexture("grass_carried"));
    cube1->SetFaceTexture("bottom", mTextureManager.GetTexture("dirt"));
    cube1->SetFaceTexture("left", mTextureManager.GetTexture("grass_side_carried"));
    cube1->SetFaceTexture("right", mTextureManager.GetTexture("grass_side_carried"));
    cube1->SetFaceTexture("front", mTextureManager.GetTexture("grass_side_carried"));
    cube1->SetFaceTexture("back", mTextureManager.GetTexture("grass_side_carried"));
    grass->AddComponent<ShapeComponent>();
    grass->GetComponent<ShapeComponent>()->AddCube(cube1);
    grass->AddComponent<TransformComponent>();
    grass->GetComponent<TransformComponent>()->SetPosition(glm::vec3(-2.0f, 1.0f, -1.f));
    mGameObjects.push_back(grass);

    std::shared_ptr<GameObject> snow = std::make_shared<GameObject>();
    std::shared_ptr<Cube> cube2 = std::make_shared<Cube>();
    cube2->SetFaceTexture("top", mTextureManager.GetTexture("snow"));
    cube2->SetFaceTexture("bottom", mTextureManager.GetTexture("dirt"));
    cube2->SetFaceTexture("left", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->SetFaceTexture("right", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->SetFaceTexture("front", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->SetFaceTexture("back", mTextureManager.GetTexture("grass_side_snowed"));
    snow->AddComponent<ShapeComponent>();
    snow->GetComponent<ShapeComponent>()->AddCube(cube2);
    snow->AddComponent<TransformComponent>();
    snow->GetComponent<TransformComponent>()->SetPosition(glm::vec3(0.f, 1.f, 0.f));
    mGameObjects.push_back(snow);

    for (int i = -2; i < 3; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
            std::shared_ptr<Cube> cube = std::make_shared<Cube>();
            cube->SetFaceTexture("top", mTextureManager.GetTexture("dirt"));
            cube->SetFaceTexture("bottom", mTextureManager.GetTexture("dirt"));
            cube->SetFaceTexture("left", mTextureManager.GetTexture("dirt"));
            cube->SetFaceTexture("right", mTextureManager.GetTexture("dirt"));
            cube->SetFaceTexture("front", mTextureManager.GetTexture("dirt"));
            cube->SetFaceTexture("back", mTextureManager.GetTexture("dirt"));
            object->AddComponent<ShapeComponent>();
            object->GetComponent<ShapeComponent>()->AddCube(cube);
            object->AddComponent<TransformComponent>();
            object->GetComponent<TransformComponent>()->SetPosition(glm::vec3(i, 0.0f, j));
            mGameObjects.push_back(object);
        }
    }
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
            mWorldSerializer.SaveWorld("./world.json", mGameObjects, mTextureManager);
        }
        if (e.type == SDL_MOUSEMOTION)
        {
            mCamera.RotateCamera(e.motion.xrel, e.motion.yrel);
        }
    }

    // Retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // camera controls
    if (state[SDL_SCANCODE_W])
    {
        mCamera.MoveForward(0.1f);
    }
    else if (state[SDL_SCANCODE_S])
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
}

void Engine::FindSelectedObject()
{
    glm::vec3 rayDir = mCamera.GetRayDirection(mScreenWidth, mScreenHeight, glm::vec2(mScreenWidth / 2, mScreenHeight / 2));
    // std::cout << "Ray direction: " << rayDir.x << " " << rayDir.y << " " << rayDir.z << std::endl;
    float smallestTNear = -1.f;
    std::shared_ptr<GameObject> hitObject;
    for (std::shared_ptr<GameObject> gameObject : mGameObjects)
    {
        ShapeComponent *shape = gameObject->GetComponent<ShapeComponent>();
        for (const auto &cube : shape->GetCubes())
        {
            float tNear, tFar = 0.f;
            bool result = RayCastTest(mCamera.GetPosition(), rayDir, cube->GetMinCorner(), cube->GetMaxCorner(), tNear, tFar);

            // std::cout << "tNear: " << tNear << " tFar: " << tFar << std::endl;

            // base case for ray cast test
            if (smallestTNear == -1.f)
            {
                smallestTNear = tNear;
                hitObject = gameObject;
                continue;
            }

            // keep track of closest colliding object
            if (result)
            {
                if (tNear < smallestTNear)
                {
                    smallestTNear = tNear;
                    hitObject = gameObject;
                }
            }
        }
    }
    mSelected = hitObject;
}

void Engine::Update()
{
    for (auto gameObject : mGameObjects)
    {
        gameObject->Update();
    }
}

void Engine::Render()
{ 
    ShadowPass();
    LightPass();
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
    for (auto gameObject : mGameObjects)
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

    for (auto &gameObject : mGameObjects)
    {
        glm::mat4 model = gameObject->GetComponent<TransformComponent>()->GetModelMatrix();
        mMainShader.SetUniform("u_ModelMatrix", model);
        if (gameObject == mSelected)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        gameObject->Render();
    }
}

void Engine::Shutdown()
{
    // clear all game objects
    // for (auto gameObject : mGameObjects)
    // {
    //     delete gameObject;
    // }
    mGameObjects.clear();

    

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
    mQuadShader.Init("./shaders/quad_v.glsl", "./shaders/quad_f.glsl");
}

void Engine::GetOpenGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

bool Engine::RayCastTest(const glm::vec3 origin, const glm::vec3 direction, glm::vec3 minCorner, glm::vec3 maxCorner, float &tNear, float &tFar)
{
    glm::vec3 invDir = 1.0f / direction;
    glm::vec3 tMin = (minCorner - origin) * invDir;
    glm::vec3 tMax = (maxCorner - origin) * invDir;

    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);

    tNear = std::max(std::max(t1.x, t1.y), t1.z);
    tFar = std::min(std::min(t2.x, t2.y), t2.z);

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

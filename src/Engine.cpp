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
    std::shared_ptr<ShapeComponent> shape1 = std::make_shared<ShapeComponent>();
    std::shared_ptr<TransformComponent> transform1 = std::make_shared<TransformComponent>();
    std::shared_ptr<Cube> cube1 = std::make_shared<Cube>(glm::vec3(-1.f, 1.0f, 0.f), 1.0f);
    cube1->setFaceTexture("top", mTextureManager.GetTexture("grass_carried"));
    cube1->setFaceTexture("bottom", mTextureManager.GetTexture("dirt"));
    cube1->setFaceTexture("left", mTextureManager.GetTexture("grass_side_carried"));
    cube1->setFaceTexture("right", mTextureManager.GetTexture("grass_side_carried"));
    cube1->setFaceTexture("front", mTextureManager.GetTexture("grass_side_carried"));
    cube1->setFaceTexture("back", mTextureManager.GetTexture("grass_side_carried"));
    // shape1->setCube(cube1);
    // shape1->AddCube(cube1);
    grass->AddComponent<ShapeComponent>();
    grass->GetComponent<ShapeComponent>()->AddCube(cube1);
    mGameObjects.push_back(grass);

    std::shared_ptr<GameObject> snow = std::make_shared<GameObject>();
    std::shared_ptr<ShapeComponent> shape2 = std::make_shared<ShapeComponent>();
    std::shared_ptr<Cube> cube2 = std::make_shared<Cube>(glm::vec3(0.f, 1.f, 0.f), 1.0f);
    cube2->setFaceTexture("top", mTextureManager.GetTexture("snow"));
    cube2->setFaceTexture("bottom", mTextureManager.GetTexture("dirt"));
    cube2->setFaceTexture("left", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->setFaceTexture("right", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->setFaceTexture("front", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->setFaceTexture("back", mTextureManager.GetTexture("grass_side_snowed"));
    shape2->AddCube(cube2);
    // snow->AddComponent<ShapeComponent>(shape2);
    snow->AddComponent<ShapeComponent>()->AddCube(cube2);
    mGameObjects.push_back(snow);

    for (int i = -2; i < 3; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
            std::shared_ptr<ShapeComponent> shape = std::make_shared<ShapeComponent>();
            std::shared_ptr<Cube> cube = std::make_shared<Cube>(glm::vec3(i, 0.0f, j), 1.0f);
            cube->setFaceTexture("top", mTextureManager.GetTexture("dirt"));
            cube->setFaceTexture("bottom", mTextureManager.GetTexture("dirt"));
            cube->setFaceTexture("left", mTextureManager.GetTexture("dirt"));
            cube->setFaceTexture("right", mTextureManager.GetTexture("dirt"));
            cube->setFaceTexture("front", mTextureManager.GetTexture("dirt"));
            cube->setFaceTexture("back", mTextureManager.GetTexture("dirt"));
            shape->AddCube(cube);
            object->AddComponent<ShapeComponent>()->AddCube(cube);
            mGameObjects.push_back(object);
        }
    }
}

void Engine::MainLoop()
{
    while (!mQuit)
    {
        // std::cout << "camera values: " << mCamera.angles.x << " " << mCamera.angles.y << " " << mCamera.angles.z << std::endl;
        // std::cout << "direction: " << mCamera.direction.x << " " << mCamera.direction.y << " " << mCamera.direction.z << std::endl;
        FrameCapping();
        Input();

        // Update game here
        // TODO: add update function

        Render();

        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
    }
}

void Engine::Input()
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            std::cout << "Goodbye!" << std::endl;
            mQuit = true;
        }
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
        {
            std::cout << "Goodbye!" << std::endl;
            mQuit = true;
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
    glm::vec3 rayDir = mCamera.GetRayDirection(mScreenWidth, mScreenHeight, glm::vec2(mScreenWidth / 2, mScreenHeight / 2));
    // std::cout << "ray direction: " << rayDir.x << " " << rayDir.y << " " << rayDir.z << std::endl;
    // std::cout << "position: " << mCamera.GetPosition().x << " " << mCamera.GetPosition().y << " " << mCamera.GetPosition().z << std::endl;
    for (std::shared_ptr<GameObject> gameObject : mGameObjects) {
        ShapeComponent* shape = gameObject->GetComponent<ShapeComponent>();
        for (auto cube : shape->GetCubes()) {
            float tNear, tFar;
            bool result = RayCastTest(mCamera.GetPosition(), rayDir, cube->getMinCorner(), cube->getMaxCorner(), tNear, tFar);
            if (result) {
                // std::cout << "hit," << "tNear: " << tNear << std::endl;
            }
        }
    }
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
    // for (auto gameObject : mGameObjects)
    // {
    //     glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    //     model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //     mShadowShader.SetUniform("model", model);
    //     ShadowPass();
    //     gameObject->Render();
    // }


    ShadowPass();
    LightPass();

    glUseProgram(0);
}

void Engine::ShadowPass()
{
    // glCullFace(GL_FRONT);
    glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFBO);
    glViewport(0, 0, mShadowResolution.x, mShadowResolution.y);

    glClear(GL_DEPTH_BUFFER_BIT);

    mShadowShader.Use();

    // Shadow pass
    glm::mat4 orthoProjection = glm::ortho(-35.f, 35.f, -35.f, 35.f, 0.1f, 75.f);
    glm::mat4 lightView = glm::lookAt(glm::vec3(30.f, 30.f, 30.f), glm::vec3(0.0f), glm::vec3(0, 1, 0));
    glm::mat4 lightProjection = orthoProjection * lightView;

    mShadowShader.SetUniform("lightProjection", lightProjection);

    // render
    for (auto gameObject : mGameObjects)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        mShadowShader.SetUniform("model", model);
        gameObject->Render();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::LightPass()
{
    // glCullFace(GL_BACK);
    glViewport(0, 0, mScreenWidth, mScreenHeight);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // glUseProgram(mGraphicsPipelineShaderProgram);
    mMainShader.Use();

    glClearColor(0.1f, 0.1f, 0.1f, 1.f);

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

    glm::mat4 orthoProjection = glm::ortho(-35.f, 35.f, -35.f, 35.f, 0.1f, 75.f);
    glm::mat4 lightView = glm::lookAt(glm::vec3(30.f, 30.f, 30.f), glm::vec3(0.0f), glm::vec3(0, 1, 0));
    glm::mat4 lightProjection = orthoProjection * lightView;

    mMainShader.SetUniform("u_LightProjection", lightProjection);

    for (auto &gameObject : mGameObjects)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(g_uScale, g_uScale, g_uScale));

        mMainShader.SetUniform("u_ModelMatrix", model);

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

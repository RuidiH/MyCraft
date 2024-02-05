#include "Engine.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

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

    GameObject *grass = new GameObject();
    ShapeComponent *shape1 = new ShapeComponent();
    Cube *cube1 = new Cube(glm::vec3(-1.f, 1.0f, 0.f), 1.0f);
    cube1->setFaceTexture("top", mTextureManager.GetTexture("grass_carried"));
    cube1->setFaceTexture("bottom", mTextureManager.GetTexture("dirt"));
    cube1->setFaceTexture("left", mTextureManager.GetTexture("grass_side_carried"));
    cube1->setFaceTexture("right", mTextureManager.GetTexture("grass_side_carried"));
    cube1->setFaceTexture("front", mTextureManager.GetTexture("grass_side_carried"));
    cube1->setFaceTexture("back", mTextureManager.GetTexture("grass_side_carried"));
    shape1->setCube(cube1);
    grass->AddComponent(shape1);
    mGameObjects.push_back(grass);

    GameObject *snow = new GameObject();
    ShapeComponent *shape2 = new ShapeComponent();
    Cube *cube2 = new Cube(glm::vec3(0.f, 1.f, 0.f), 1.0f);
    cube2->setFaceTexture("top", mTextureManager.GetTexture("snow"));
    cube2->setFaceTexture("bottom", mTextureManager.GetTexture("dirt"));
    cube2->setFaceTexture("left", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->setFaceTexture("right", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->setFaceTexture("front", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->setFaceTexture("back", mTextureManager.GetTexture("grass_side_snowed"));
    shape2->setCube(cube2);
    snow->AddComponent(shape2);
    mGameObjects.push_back(snow);

    for (int i = -2; i < 3; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            GameObject *object = new GameObject();
            ShapeComponent *shape = new ShapeComponent();
            Cube *cube = new Cube(glm::vec3(i, 0.0f, j), 1.0f);
            // cube->setPosition(glm::vec3(i, 0.0f, j));
            cube->setFaceTexture("top", mTextureManager.GetTexture("dirt"));
            cube->setFaceTexture("bottom", mTextureManager.GetTexture("dirt"));
            cube->setFaceTexture("left", mTextureManager.GetTexture("dirt"));
            cube->setFaceTexture("right", mTextureManager.GetTexture("dirt"));
            cube->setFaceTexture("front", mTextureManager.GetTexture("dirt"));
            cube->setFaceTexture("back", mTextureManager.GetTexture("dirt"));
            shape->setCube(cube);
            object->AddComponent(shape);
            mGameObjects.push_back(object);
        }
    }
}

void Engine::MainLoop()
{
    while (!mQuit)
    {
        std::cout << "camera values: " << mCamera.angles.x << " " << mCamera.angles.y << " " << mCamera.angles.z << std::endl;
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
        if (e.type == SDL_MOUSEMOTION)
        {
            // Assuming mCamera.angles.y (phi) is pitch and mCamera.angles.x (theta) is yaw
            // Adjust pitch, ensuring it doesn't flip over the top or bottom
            mCamera.angles.y = std::max(std::min(mCamera.angles.y - e.motion.yrel * 0.005f, glm::radians(89.0f)), glm::radians(-89.0f));
            mCamera.angles.x -= e.motion.xrel * 0.005f; // Adjust yaw

            // Calculate the direction vector from spherical coordinates
            // Note: Assuming angles are stored in radians. If in degrees, convert them to radians.

            mCamera.direction = glm::normalize(
                glm::vec3(
                    cos(mCamera.angles.y) * sin(mCamera.angles.x), // X
                    sin(mCamera.angles.y),                         // Y
                    cos(mCamera.angles.y) * cos(mCamera.angles.x)  // Z
                    ));

        }
    }

    // Retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // camera controls
    if (state[SDL_SCANCODE_W])
    {
        mCamera.position += mCamera.direction * 0.1f;
    }
    if (state[SDL_SCANCODE_S])
    {
        mCamera.position -= mCamera.direction * 0.1f;
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
    glViewport(0, 0, mScreenWidth, mScreenHeight);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // glUseProgram(mGraphicsPipelineShaderProgram);
    mMainShader.Use();

    glClearColor(0.1f, 0.1f, 0.1f, 1.f);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, mShadowMapTexture);

    // glUniform1i(glGetUniformLocation(mGraphicsPipelineShaderProgram, "shadowMap"), 1);
    mMainShader.SetUniform("shadowMap", 1);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            (float)mScreenWidth / (float)mScreenHeight,
                                            1.0f,
                                            100.0f);

    mMainShader.SetUniform("u_Projection", projection);

    glm::mat4 viewMtx = glm::lookAt(mCamera.position,
                                    mCamera.direction + mCamera.position,
                                    glm::vec3(0.0f, 1.0f, 0.0f));

    // set view matrix
    mMainShader.SetUniform("u_View", viewMtx);

    // set lightings
    mMainShader.SetUniform("lightPos", glm::vec3(3.0f, 3.0f, 3.0f));

    mMainShader.SetUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    mMainShader.SetUniform("viewPos", mCamera.position);

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
    for (auto gameObject : mGameObjects)
    {
        delete gameObject;
    }
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

    gGraphicsApplicationWindow = SDL_CreateWindow("Window", 800, 300, mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);
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

    mCamera.position = glm::vec3(-3.f, 3.f, 3.f);
    // mCamera.angles = glm::vec3(-M_PI / 3.0f, M_PI / 2.8f, 1.0f);
    mCamera.angles = glm::vec3(M_PI * 3 / 4.f, M_PI / 6.f, 10.0f);
    // mCamera.angles = CalculateCameraAngles(mCamera.position, glm::vec3(0.f, 0.f, 0.f));
    mCamera.direction = glm::vec3(0.f, 0.f, 0.f) - mCamera.position;

    // recomputeOrientation();

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

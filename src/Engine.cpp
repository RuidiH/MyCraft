#include "Engine.hpp"

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

    // GameObject *grass = new GameObject();
    // ShapeComponent *shape1 = new ShapeComponent();
    // Cube *cube1 = new Cube(glm::vec3(-1.f, 1.0f, 0.f), 1.0f);
    // cube1->setFaceTexture("top", mTextureManager.GetTexture("grass_carried"));
    // cube1->setFaceTexture("bottom", mTextureManager.GetTexture("dirt"));
    // cube1->setFaceTexture("left", mTextureManager.GetTexture("grass_side_carried"));
    // cube1->setFaceTexture("right", mTextureManager.GetTexture("grass_side_carried"));
    // cube1->setFaceTexture("front", mTextureManager.GetTexture("grass_side_carried"));
    // cube1->setFaceTexture("back", mTextureManager.GetTexture("grass_side_carried"));
    // shape1->setCube(cube1);
    // grass->AddComponent(shape1);
    // mGameObjects.push_back(grass);

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
            mCamera.angles.y = std::max(std::min(static_cast<float>(mCamera.angles.y + e.motion.yrel * 0.005), 180.0f), 0.0f);
            mCamera.angles.x = mCamera.angles.x + e.motion.xrel * 0.005;

            mCamera.direction = glm::normalize(
                glm::vec3(
                    cos(mCamera.angles.y) * cos(mCamera.angles.x),
                    sin(mCamera.angles.y),
                    cos(mCamera.angles.y) * sin(mCamera.angles.x)));
            mCamera.direction *= mCamera.angles.z;
        }
    }

    // Retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // camera controls
    if (state[SDL_SCANCODE_W])
    {
        mCamera.position += mCamera.direction * 0.01f;
    }
    if (state[SDL_SCANCODE_S])
    {
        mCamera.position -= mCamera.direction * 0.01f;
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
    glViewport(0, 0, 1024, 1024);

    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(mFrameBufferProgram);

    // Shadow pass
    glm::mat4 orthoProjection = glm::ortho(-35.f, 35.f, -35.f, 35.f, 0.1f, 75.f);
    glm::mat4 lightView = glm::lookAt(glm::vec3(30.f, 30.f, 30.f), glm::vec3(0.0f), glm::vec3(0, 1, 0));
    glm::mat4 lightProjection = orthoProjection * lightView;

    GLint g_lightProjection = glGetUniformLocation(mFrameBufferProgram, "lightProjection");

    if (g_lightProjection >= 0)
    {
        glUniformMatrix4fv(g_lightProjection, 1, GL_FALSE, &lightProjection[0][0]);
    }
    else
    {
        std::cout << "Could not find g_LightProjection, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    // render
    for (auto gameObject : mGameObjects)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        GLint u_model = glGetUniformLocation(mFrameBufferProgram, "model");

        if (u_model >= 0)
        {
            glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);
        }
        else
        {
            std::cout << "Could not find model, maybe a mispelling?\n";
            exit(EXIT_FAILURE);
        }

        gameObject->Render();
    }
}

void Engine::LightPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, mScreenWidth, mScreenHeight);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(mGraphicsPipelineShaderProgram);

    glClearColor(0.1f, 0.1f, 0.1f, 1.f);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, mShadowMapTexture);
    glUniform1i(glGetUniformLocation(mGraphicsPipelineShaderProgram, "shadowMap"), 1);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            (float)mScreenWidth / (float)mScreenHeight,
                                            1.0f,
                                            100.0f);

    GLint u_ProjectionLocation = glGetUniformLocation(mGraphicsPipelineShaderProgram, "u_Projection");

    if (u_ProjectionLocation >= 0)
    {
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
    else
    {
        std::cout << "Could not find u_Projection, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    glm::mat4 viewMtx = glm::lookAt(mCamera.position,
                                    mCamera.direction + mCamera.position,
                                    glm::vec3(0.0f, 1.0f, 0.0f));

    // set view matrix
    GLint u_ViewLocation = glGetUniformLocation(mGraphicsPipelineShaderProgram, "u_View");
    if (u_ViewLocation >= 0)
    {
        glUniformMatrix4fv(u_ViewLocation, 1, GL_FALSE, &viewMtx[0][0]);
    }
    else
    {
        std::cout << "Could not find u_ViewLocation, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    // set lightings
    GLuint u_LightPosition = glGetUniformLocation(mGraphicsPipelineShaderProgram, "lightPos");
    if (u_LightPosition >= 0)
    {
        glUniform3f(u_LightPosition, 3.0f, 3.0f, 3.0f);
    }
    else
    {
        std::cout << "Could not find u_LightPosition, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    GLuint u_LightColor = glGetUniformLocation(mGraphicsPipelineShaderProgram, "lightColor");
    if (u_LightColor >= 0)
    {
        glUniform3f(u_LightColor, 1.0f, 1.0f, 1.0f);
    }
    else
    {
        std::cout << "Could not find u_LightColor, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    GLuint u_ViewPosition = glGetUniformLocation(mGraphicsPipelineShaderProgram, "viewPos");
    if (u_ViewPosition >= 0)
    {
        glUniform3f(u_ViewPosition, mCamera.position.x, mCamera.position.y, mCamera.position.z);
    }
    else
    {
        std::cout << "Could not find u_ViewPosition, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    // glUniform3f(u_LightColor, 1.0f, 1.0f, 1.0f);
    // glUniform3f(u_LightPosition, 2.f, 2.f, 2.f);
    // glUniform3f(u_ViewPosition, mCamera.position.x, mCamera.position.y, mCamera.position.z);

    glm::mat4 orthoProjection = glm::ortho(-35.f, 35.f, -35.f, 35.f, 0.1f, 75.f);
    glm::mat4 lightView = glm::lookAt(glm::vec3(30.f, 30.f, 30.f), glm::vec3(0.0f), glm::vec3(0, 1, 0));
    glm::mat4 lightProjection = orthoProjection * lightView;

    GLint u_lightProjection = glGetUniformLocation(mGraphicsPipelineShaderProgram, "u_LightProjection");

    if (u_lightProjection >= 0)
    {
        glUniformMatrix4fv(u_lightProjection, 1, GL_FALSE, &lightProjection[0][0]);
    }
    else
    {
        std::cout << "Could not find u_LightProjection, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }


    for (auto &gameObject : mGameObjects)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(g_uScale, g_uScale, g_uScale));

        GLint u_ModelMatrixLocation = glGetUniformLocation(mGraphicsPipelineShaderProgram, "u_ModelMatrix");

        if (u_ModelMatrixLocation >= 0)
        {
            glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
        }
        else
        {
            std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
            exit(EXIT_FAILURE);
        }

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

    gGraphicsApplicationWindow = SDL_CreateWindow("Window", 500, 500, mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);
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

    mCamera.position = glm::vec3(3.f, 3.f, 3.f);
    mCamera.angles = glm::vec3(-M_PI / 3.0f, M_PI / 2.8f, 20.0f);
    // mCamera.direction = glm::vec3(-1.f, -1.f, -1.f);

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

    const int shadowMapWidth = 1024;
    const int shadowMapHeight = 1024;

    glGenFramebuffers(1, &mShadowMapFBO);

    // Create a depth texture for the shadow map
    glGenTextures(1, &mShadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, mShadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
    std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");

    mGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    std::string vertFrameBuffer = LoadShaderAsString("./shaders/shadow_v.glsl");
    std::string fragFrameBuffer = LoadShaderAsString("./shaders/shadow_f.glsl");

    mFrameBufferProgram = CreateShaderProgram(vertFrameBuffer, fragFrameBuffer);
}

GLuint Engine::CreateShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{
    GLuint programObject = glCreateProgram();

    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    glValidateProgram(programObject);
    // detach and delete

    return programObject;
}

GLuint Engine::CompileShader(GLuint type, const std::string &source)
{
    GLuint shaderObject;
    if (type == GL_VERTEX_SHADER)
    {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char *src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    return shaderObject;
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

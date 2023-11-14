#include "Engine.hpp"

Uint32 Engine::deltaTime;

Engine::Engine(int width, int height) : mScreenWidth(width), mScreenHeight(height)
{
    InitializeGraphicsProgram();

    CreateGraphicsPipeline();
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
    Cube *cube2 = new Cube(glm::vec3(1.f, 1.0f, 0.f), 1.0f);
    cube2->setFaceTexture("top", mTextureManager.GetTexture("snow"));
    cube2->setFaceTexture("bottom", mTextureManager.GetTexture("dirt"));
    cube2->setFaceTexture("left", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->setFaceTexture("right", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->setFaceTexture("front", mTextureManager.GetTexture("grass_side_snowed"));
    cube2->setFaceTexture("back", mTextureManager.GetTexture("grass_side_snowed"));
    shape2->setCube(cube2);
    snow->AddComponent(shape2);
    mGameObjects.push_back(snow);

    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
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
                glm::vec3(sin(mCamera.angles.y) * sin(mCamera.angles.x), cos(mCamera.angles.y), -sin(mCamera.angles.y) * cos(mCamera.angles.x)));
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
    PreDraw();

    for (auto gameObject : mGameObjects)
    {
        gameObject->Render();
    }

    glUseProgram(0);
}

void Engine::PreDraw()
{

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);

    glUseProgram(gGraphicsPipelineShaderProgram);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(g_uScale, g_uScale, g_uScale));

    GLint u_ModelMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ModelMatrix");

    if (u_ModelMatrixLocation >= 0)
    {
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    }
    else
    {
        std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            (float)mScreenWidth / (float)mScreenHeight,
                                            1.0f,
                                            100.0f);

    GLint u_ProjectionLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Projection");

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
    GLint u_ViewLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_View");
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
    GLuint u_LightPosition = glGetUniformLocation(gGraphicsPipelineShaderProgram, "lightPos");
    if (u_LightPosition >= 0)
    {
        glUniform3f(u_LightPosition, 0.0f, 0.0f, 0.0f);
    }
    else
    {
        std::cout << "Could not find u_LightPosition, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    GLuint u_LightColor = glGetUniformLocation(gGraphicsPipelineShaderProgram, "lightColor");
    if (u_LightColor >= 0)
    {
        glUniform3f(u_LightColor, 1.0f, 1.0f, 1.0f);
    }
    else
    {
        std::cout << "Could not find u_LightColor, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    GLuint u_ViewPosition = glGetUniformLocation(gGraphicsPipelineShaderProgram, "viewPos");
    if (u_ViewPosition >= 0)
    {
        glUniform3f(u_ViewPosition, mCamera.position.x, mCamera.position.y, mCamera.position.z);
    }
    else
    {
        std::cout << "Could not find u_ViewPosition, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    // GLuint u_ObjectColorLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "objectColor");
    // if (u_ObjectColorLocation >= 0)
    // {
    //     glUniform3f(u_ObjectColorLocation, 1.0f, 1.0f, 1.0f);
    // }
    // else
    // {
    //     std::cout << "Could not find u_ObjectColor, maybe a mispelling?\n";
    //     exit(EXIT_FAILURE);
    // }

    // glUniform3f(u_ObjectColorLocation, 1.0f, 1.0f, 1.0f);

    glUniform3f(u_LightColor, 1.0f, 1.0f, 1.0f);
    glUniform3f(u_LightPosition, mCamera.position.x, mCamera.position.y, mCamera.position.z);
    glUniform3f(u_ViewPosition, mCamera.position.x, mCamera.position.y, mCamera.position.z);
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

void Engine::CreateGraphicsPipeline()
{
    std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");

    gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
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

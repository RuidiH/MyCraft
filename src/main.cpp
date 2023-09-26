// Third Party
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Standard
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct Camera
{
    glm::vec3 lookAt;
    glm::vec3 upVector;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 angles;
};

Camera camera;

// Error Handling

static void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR)
    {
    }
}

static bool GLCheckErrorStatus(const char *function, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL Error:" << error << "\tLine: " << line
                  << "\tFunction: " << function << std::endl;
        return true;
    }
    return false;
}

#define GLCheck(x)   \
    GLClearErrors(); \
    x;               \
    GLCheckErrorStatus(#x, __LINE__);

float g_uOffset = -2.0f;
float g_uRotate = 0.0f;
float g_uScale = 1.5f;

// Globals
int gScreenHeight = 640;
int gScreenWidth = 480;
SDL_Window *gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

bool gQuit = false;

// VAO
GLuint gVertexArrayObject = 0;
// VBO
GLuint gVertexBufferObject = 0;
// IBO
GLuint gIndexBufferObject = 0;

// Program Object for shaders
GLuint gGraphicsPipelineShaderProgram = 0;

glm::vec3 camPos;
GLdouble cameraTheta, cameraPhi;
glm::vec3 camDir;
float radius = 20.0f;

double mouseX = gScreenWidth / 2, mouseY = gScreenHeight / 2;

void recomputeOrientation()
{
    camDir = glm::normalize(
        glm::vec3(sin(cameraPhi) * sin(cameraTheta), cos(cameraPhi), -sin(cameraPhi) * cos(cameraTheta)));
    camDir *= radius;

    camera.direction = glm::normalize(
        glm::vec3(sin(camera.angles.y) * sin(camera.angles.x), cos(camera.angles.y), -sin(camera.angles.y) * cos(camera.angles.x)));
    camera.direction *= camera.angles.z;
}

std::string LoadShaderAsString(const std::string &filename)
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

GLuint CompileShader(GLuint type, const std::string &source)
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

GLuint CreateShaderProgram(const std::string &vertexShaderSource,
                           const std::string &fragmentShaderSource)
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
void CreateGraphicsPipeline()
{
    std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");

    gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource,
                                                         fragmentShaderSource);
}

void GetOpenGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void VertexSpecification()
{
    const std::vector<float> vertexData{
        -0.5f, -0.5f, 0.0f, // bottom left
        1.0f, 0.0f, 0.0f,   // color

        0.5f, -0.5f, 0.0f, // bottom right
        0.0f, 0.0f, 1.0f,  // color

        -0.5f, 0.5f, 0.0f, // top left
        0.0f, 1.0f, 0.0f,  // color

        0.5f, 0.5f, 0.0f, // top right
        1.0f, 0.0f, 0.0f  // color
    };

    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 vertexData.size() * sizeof(GL_FLOAT),
                 vertexData.data(),
                 GL_STATIC_DRAW);

    const std::vector<GLuint> indexBufferData{2, 0, 1, 3, 2, 1};

    // Set up Index Buffer Object
    glGenBuffers(1, &gIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indexBufferData.size() * sizeof(GLuint),
                 indexBufferData.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 6,
                          (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 6,
                          (GLvoid *)(sizeof(GL_FLOAT) * 3));

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Initialize()
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

    gGraphicsApplicationWindow = SDL_CreateWindow("Window", 300, 300, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);
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

    // glfwInit();

    // glfwTerminate();

    // initializing camera parameters
    camPos.x = 0;
    camPos.y = 0;
    camPos.z = 0;
    cameraTheta = -M_PI / 3.0f;
    cameraPhi = M_PI / 2.8f;

    camera.position = glm::vec3(0.f, 0.f, -1.f);
    camera.angles = glm::vec3(-M_PI / 3.0f, M_PI / 2.8f, 20.0f);

    recomputeOrientation();

    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Input()
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            std::cout << "Goodbye!" << std::endl;
            gQuit = true;
        }

        if (e.type == SDL_MOUSEMOTION)
        {
            cameraPhi = std::max(std::min(static_cast<float>(cameraPhi + e.motion.yrel * 0.005), 180.0f), 0.0f);
            // std::cout << "x: " << e.motion.xrel << " y: " << e.motion.yrel << std::endl;
            // if (cameraPhi + e.motion.xrel * 0.005 > 0 && cameraPhi + e.motion.yrel * 0.005 < M_PI)
            // {
            //     cameraPhi = cameraPhi + e.motion.yrel * 0.005;
            // }
            cameraTheta = cameraTheta + e.motion.xrel * 0.005;

            camera.angles.y = std::max(std::min(static_cast<float>(cameraPhi + e.motion.yrel * 0.005), 180.0f), 0.0f);
            camera.angles.x = camera.angles.x + e.motion.xrel * 0.005;

            recomputeOrientation();
        }

        // update camera with mouse state
        // switch (e.button.button)
        // {
        // case SDL_BUTTON_LEFT:
        //     int x = e.motion.xrel + gScreenWidth / 2;
        //     int y = e.motion.yrel + gScreenHeight / 2; //     std::cout << "mouse event triggered" << std::endl; //     if (mouseX == -99999)
        //     {
        //         mouseX = x;
        //         mouseY = y;
        //     }
        //     if (e.type == SDL_MOUSEMOTION)
        //     {
        //         std::cout << "x: " << x << ", y: " << y << std::endl;
        //         if (cameraPhi + ((mouseY - y) * 0.005) > 0 && cameraPhi + ((mouseY - y) * 0.005) < M_PI)
        //         {
        //             cameraPhi = cameraPhi + ((mouseY - y) * 0.005);
        //         }
        //         cameraTheta = cameraTheta + ((x - mouseX) * 0.005);
        //     }
        //     recomputeOrientation();
        //     break;
        // }
    }

    // Retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP])
    {
        g_uOffset += 0.05f;
        std::cout << "g_uOffset: " << g_uOffset << std::endl;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        g_uOffset -= 0.05f;
        std::cout << "g_uOffset: " << g_uOffset << std::endl;
    }
    if (state[SDL_SCANCODE_LEFT])
    {
        g_uRotate -= 1.0f;
        std::cout << "g_uRotate: " << g_uRotate << std::endl;
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        g_uRotate += 1.0f;
        std::cout << "g_uRotate: " << g_uRotate << std::endl;
    }

    // camera controls
    if (state[SDL_SCANCODE_W])
    {
        camera.position += camera.direction * 0.01f;
    }
    if (state[SDL_SCANCODE_S])
    {
        camera.position -= camera.direction * 0.01f;
    }
}

void PreDraw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.f, 1.f, 0.1f, 1.f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, g_uOffset));
    model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(g_uScale, g_uScale, g_uScale));

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

    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
                                             (float)gScreenWidth / (float)gScreenHeight,
                                             0.1f,
                                             10.0f);

    GLint u_ProjectionLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Projection");

    if (u_ProjectionLocation >= 0)
    {
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &perspective[0][0]);
    }
    else
    {
        std::cout << "Could not find u_Perspective, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    // glm::vec3 forward{
    //     glm::sin(glm::radians(cameraPhi)) * glm::cos(glm::radians(cameraTheta)),
    //     glm::sin(glm::radians(cameraPhi)) * glm::sin(glm::radians(cameraTheta)),
    //     glm::cos(glm::radians(cameraPhi))};

    // glm::mat4 viewMtx = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f),
    //                                 camDir,
    //                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 viewMtx = glm::lookAt(camera.position,
                                    camera.direction + camera.position,
                                    glm::vec3(0.0f, 1.0f, 0.0f));
    // glm::mat4 viewMtx = glm::lookAt((camDir + glm::vec3(0, 0, 0)),
    //                                 glm::vec3(0, 0, 0),
    //                                 glm::vec3(0, 1, 0));

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

    // set normal matrix
    // glm::mat4 modelView = viewMtx * model;
    // glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelView)));
    // glUniformMatrix3fv(normalMtxLocation, 1, GL_FALSE, &normalMatrix[0][0]);
}

// draw a cube
void DrawCube(glm::vec3 position, glm::vec3 color, float size)
{
    // initialize cube vertices
    const std::vector<float> vertexData{
        position.x, position.y, position.z, // 0
        color.x, color.y, color.z, // color

        position.x + size, position.y, position.z, // 1
        color.x, color.y, color.z, // color

        position.x + size, position.y + size, position.z, // 2
        color.x, color.y, color.z, // color

        position.x + size, position.y + size, position.z + size, // 3
        color.x, color.y, color.z, // color

        position.x, position.y + size, position.z, // 4
        color.x, color.y, color.z, // color

        position.x, position.y, position.z + size, // 5
        color.x, color.y, color.z, // color

        position.x, position.y + size, position.z + size, // 6
        color.x, color.y, color.z, // color

        position.x + size, position.y, position.z + size, // 7
        color.x, color.y, color.z, // color
    };

    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 vertexData.size() * sizeof(GL_FLOAT),
                 vertexData.data(),
                 GL_STATIC_DRAW);

    // const std::vector<GLuint> indexBufferData{2, 0, 1, 3, 2, 1};
    const std::vector<GLuint> indexBufferData{0, 4, 2, 1, 0, 2,
                                              7, 3, 2, 1, 7, 2,
                                              0, 5, 7, 1, 0, 7,
                                              5, 6, 3, 7, 5, 3,
                                              4, 6, 3, 2, 4, 3,
                                              5, 6, 4, 0, 5, 4};

    // Set up Index Buffer Object
    glGenBuffers(1, &gIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,   
                 indexBufferData.size() * sizeof(GLuint),
                 indexBufferData.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3, // x, y, z
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 6,
                          (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3, // r, g, b
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 6,
                          (GLvoid *)(sizeof(GL_FLOAT) * 3));

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Draw()
{
    glBindVertexArray(gVertexArrayObject);
    GLCheck(glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);)
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        GLCheck(glDrawElements(GL_TRIANGLES,
                               36,
                               GL_UNSIGNED_INT,
                               //    GL_INT,
                               0);)

        // not required if there's only a single graphics pipeline
        glUseProgram(0);
}

void MainLoop()
{
    while (!gQuit)
    {
        Input();

        PreDraw();

        Draw();

        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
    }
}

void CleanUp()
{
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    // Set up graphics program
    Initialize();

    // Setup geometry
    // VertexSpecification();
    DrawCube(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 1.f), 1.0f);

    // Create graphics pipeline with at least vertex and fragment shader
    CreateGraphicsPipeline();

    MainLoop();

    CleanUp();

    return 0;
}
#ifndef Engine_HPP
#define Engine_HPP

// Third Party
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Standard
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "GameObject.hpp"
#include "Cube.hpp"
#include "TextureManager.hpp"
#include "Shader.hpp"
#include "Camera.hpp"


class Engine
{
public:
    static Uint32 deltaTime;

    Engine(int width, int height);
    ~Engine();

    void SetupObject();

    void MainLoop();

    void Shutdown();

private:
    int mScreenWidth;
    int mScreenHeight;
    TextureManager mTextureManager;
    std::vector<std::shared_ptr<GameObject>> mGameObjects;
    Camera mCamera;
    glm::vec2 mShadowResolution = glm::vec2(4096, 4096);
    // game state
    bool mQuit = false;

    SDL_Window *gGraphicsApplicationWindow = nullptr;
    SDL_GLContext gOpenGLContext = nullptr;
    GLuint mGraphicsPipelineShaderProgram;
    GLuint mFrameBufferProgram;
    GLuint mShadowMapTexture;

    // for framecap
    uint32_t prevTime = 0;

    // game loop functions
    void Input();
    void Update();
    void Render();

    // shaders
    Shader mMainShader;
    Shader mShadowShader;
    GLuint mShadowMapFBO;

    // light pass and shadowPass
    // void ShadowPass(std::shared_ptr<GameObject> object);
    void ShadowPass();
    void LightPass();

    // opengl settup
    void InitializeGraphicsProgram();
    void InitializeShadowMap();
    void CreateGraphicsPipeline();

    // framecap related functions
    void FrameCapping();
    void SetTime();

    // helper functions
    void GetOpenGLVersionInfo();
    std::string LoadShaderAsString(const std::string &filename);
    glm::vec3 CalculateCameraAngles(const glm::vec3& cameraPos, const glm::vec3& lookAtPos);
    bool RayCastTest(const glm::vec3 origin, const glm::vec3 direction, const glm::vec3 minCorner, const glm::vec3 maxCorner, float &tNear, float &tFar);
};

#endif
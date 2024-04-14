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
#include <unordered_map>
#include <array>

// #ifndef M_PI
//     #define M_PI 3.14159265358979323846
// #endif

#include "GameObject.hpp"
#include "TextureManager.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "WorldSerializer.hpp"
#include "ObjectManager.hpp"

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
    std::shared_ptr<TextureManager> mTextureManager;
    std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> mGameObjects;

    std::shared_ptr<Camera> mCamera;
    glm::vec2 mShadowResolution = glm::vec2(4096, 4096);

    std::shared_ptr<ObjectManager> mObjectManager;

    // game state
    bool mQuit = false;

    SDL_Window *gGraphicsApplicationWindow = nullptr;
    SDL_GLContext gOpenGLContext = nullptr;
    GLuint mGraphicsPipelineShaderProgram;
    GLuint mFrameBufferProgram;
    GLuint mShadowMapTexture;

    // World Serializer
    std::shared_ptr<WorldSerializer> mWorldSerializer;

    // for framecap
    uint32_t prevTime = 0;

    // game loop functions
    void Input();
    void RemoveObject();
    void AddObject();
    void Update();
    void Render();
    void FindSelectedObject();

    // shaders
    Shader mMainShader;
    Shader mShadowShader;
    Shader mQuadShader;
    Shader mCrosshairShader;
    Shader mHighlightShader;
    Shader mWaterShader;

    GLuint mShadowMapFBO;
    glm::mat4 mLightProjection; 

    // light pass and shadowPass
    // void ShadowPass(std::shared_ptr<GameObject> object);
    void ShadowPass();
    void LightPass();
    void CrosshairPass();
    void HighlightPass();

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

    // Ray Cast
    std::shared_ptr<GameObject> mSelected;
    std::string mSelectedFace = "none";
    std::string mNewObjectTextureGroup = "dirt";
    std::string mNewObjectID = "dirt_block";
    bool RayCastTest(const glm::vec3 origin, const glm::vec3 direction, const std::array<glm::vec3, 2> &box, float &tNear, float &tFar, std::string &hitSide);

    // Crosshair logic
    std::array<float, 12> mCrosshairVertices;
    GLuint mCrosshairVAO;
    GLuint mCrosshairVBO;
};

#endif
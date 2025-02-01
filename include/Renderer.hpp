#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <unordered_map>
#include <memory>
#include <string>

#include "GameObject.hpp"
#include "ObjectManager.hpp"
#include "Camera.hpp"

class Renderer
{
public:
    Renderer(int width, int height, std::shared_ptr<Camera> camera)
        : mScreenWidth(width), mScreenHeight(height), mCamera(camera);
    ~Renderer();

    void Initialize();

    void RenderScene(const std::vector<std::shared_ptr<GameObject>> &gameObjects,
                     const std::shared_ptr<GameObject> &selectedObject,
                     const glm::vec3 &lightPos);

    GLuint GetShadowMapTexture() const;

    const glm::mat4 &GetLightProjection() const;

private:
    // --- Render Passes ---
    void RenderShadowPass(const std::vector<std::shared_ptr<GameObject>> &gameObjects,
                          const glm::mat4 &lightProjection);
    void RenderLightPass(const std::vector<std::shared_ptr<GameObject>> &gameObjects,
                         const std::shared_ptr<GameObject> &selectedObject,
                         const glm::vec3 &lightPos,
                         const glm::mat4 &lightProjection);
    void RenderHighlightPass(const std::vector<std::shared_ptr<GameObject>> &gameObjects,
                             const std::shared_ptr<GameObject> &selectedObject);
    void RenderCrosshair();

private:
    int mScreenWidth;
    int mScreenHeight;
    std::shared_ptr<Camera> mCamera;

    // --- Shader Programs ---
    Shader mMainShader;
    Shader mShadowShader;
    Shader mCrosshairShader;
    Shader mHighlightShader;
    // Shader mWaterShader;
    // Shader mWaterShadowShader;

    // --- Shadow Map Resources ---
    GLuint mShadowMapFBO;
    GLuint mShadowMapTexture;
    glm::ivec2 mShadowResolution;
    glm::mat4 mLightProjection;  // Typically orthographic * lookAt matrix

    // --- Crosshair Resources ---
    GLuint mCrosshairVAO;
    GLuint mCrosshairVBO;
    std::vector<float> mCrosshairVertices;
};

#endif
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
    public:
        Camera();
        Camera(glm::vec3 upVector, glm::vec3 position, glm::vec3 direction);
        ~Camera();

        void MoveForward(float speed);
        void MoveBackward(float speed);
        void MoveLeft(float speed);
        void MoveRight(float speed);
        void RotateCamera(float x, float y);

        void UpdateViewMatrix();

        // void SetLookAt(glm::vec3 lookAt);
        void SetUpVector(glm::vec3 upVector);
        void SetPosition(glm::vec3 position);
        void SetDirection(glm::vec3 direction);
        void SetProjectionMatrix(float fov, float aspect, float near, float far);

        // glm::vec3 GetLookAt();
        glm::vec3 GetUpVector();
        glm::vec3 GetPosition();
        glm::vec3 GetDirection();
        glm::mat4 GetViewMatrix();
        glm::mat4 GetProjectionMatrix();

    private:
        // glm::vec3 lookAt;
        glm::vec3 upVector;
        glm::vec3 position;
        glm::vec3 direction;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
};

#endif
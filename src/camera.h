#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glad/gl.h>

#define CAMERA_SPEED 1.5f // Units per second
#define CAMERA_SENSITIVITY 0.1f

class Camera {
    public:
        Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
            this->position = position;
            this->direction = direction;
            this->up = up;
        };

        void HandleKeyboard(GLFWwindow* window, float deltaTime) {
            // Forward / backward
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                this->position += CAMERA_SPEED * deltaTime * this->direction;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                this->position -= CAMERA_SPEED * deltaTime * this->direction;

            // Left / right
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                this->position -= glm::normalize(glm::cross(this->direction, this->up)) * CAMERA_SPEED * deltaTime;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                this->position += glm::normalize(glm::cross(this->direction, this->up)) * CAMERA_SPEED * deltaTime;
        };

        void HandleMouse(GLFWwindow* window, double x, double y) {
            float offsetX = (x - this->lastX) * CAMERA_SENSITIVITY;
            float offsetY = (y - this->lastY) * CAMERA_SENSITIVITY;

            this->lastX = x;
            this->lastY = y;

            this->yaw += offsetX;
            this->pitch -= offsetY;

            this->pitch = std::clamp(this->pitch, -89.0f, 89.0f);

            Camera::UpdateVectors();
        };

        glm::mat4 GetViewMatrix() {
            return glm::lookAt(this->position, this->position + this->direction, this->up);
        };

        glm::vec3 GetPosition() {
            return this->position;
        }

        glm::vec3 GetForward() {
            return this->direction;
        };
        
    private:
        void UpdateVectors() {
            this->direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            this->direction.y = sin(glm::radians(this->pitch));
            this->direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            this->direction = glm::normalize(this->direction);
        };

        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up;

        float pitch;
        float yaw;

        float lastX, lastY = 0.0f;
};

#pragma once
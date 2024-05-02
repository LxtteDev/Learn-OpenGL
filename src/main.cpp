#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cstdlib>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"
#include "camera.h"

#define ASSERT(x, msg) if (!(x)) { printf("%s\n", msg); exit(1); }
#define GLASSERT(x) if (!(x)) { const char* message; int error = glfwGetError(&message); printf("GLFW Error %i: %s\n", error, message); glfwTerminate(); exit(1); }

#define SCROLL_SENSITIVITY 2.0f

float fov = 45.0f;
void HandleScroll(GLFWwindow* window, double x, double y) {
    fov -= y * SCROLL_SENSITIVITY;
    fov = std::clamp(fov, 1.0f, 45.0f);
}

int main(int argc, char** argv) {
    GLASSERT(glfwInit());


    // Setup window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Breaking GL", NULL, NULL);
    GLASSERT(window != NULL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    
    // Load window context
    glfwMakeContextCurrent(window);
    GLASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));


    // Render information
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);


    // Setup square
    float pos = 0.5f;
    float vertices[] = {
        -pos, -pos, -pos,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         pos, -pos, -pos,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         pos,  pos, -pos,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         pos,  pos, -pos,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -pos,  pos, -pos,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -pos, -pos, -pos,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

        -pos, -pos,  pos,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         pos, -pos,  pos,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         pos,  pos,  pos,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
         pos,  pos,  pos,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -pos,  pos,  pos,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -pos, -pos,  pos,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

        -pos,  pos,  pos,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -pos,  pos, -pos,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -pos, -pos, -pos,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -pos, -pos, -pos,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -pos, -pos,  pos,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -pos,  pos,  pos,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         pos,  pos,  pos,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         pos,  pos, -pos,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         pos, -pos, -pos,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         pos, -pos, -pos,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         pos, -pos,  pos,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         pos,  pos,  pos,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

        -pos, -pos, -pos,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         pos, -pos, -pos,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         pos, -pos,  pos,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         pos, -pos,  pos,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -pos, -pos,  pos,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -pos, -pos, -pos,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

        -pos,  pos, -pos,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         pos,  pos, -pos,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         pos,  pos,  pos,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
         pos,  pos,  pos,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -pos,  pos,  pos,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -pos,  pos, -pos,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
    };

    unsigned int vertexArray;
    glGenBuffers(1, &vertexArray);
    glBindVertexArray(vertexArray);

    unsigned int arrayBuffer;
    glGenBuffers(1, &arrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Layout Location, Size, Type, Normalised, Stride, Offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Setup camera
    Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glfwSetWindowUserPointer(window, camera);
    glfwSetScrollCallback(window, HandleScroll);  
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
        static_cast<Camera*>(glfwGetWindowUserPointer(window))->HandleMouse(window, x, y);
    });


    // Setup shaders
    glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f,2.0f);
    glm::vec3 objectColour = glm::vec3(1.0f, 0.5f, 0.31f);

    Shader* cubeShader = new Shader("res/shaders/vertex.glsl", "res/shaders/material.glsl");
    Shader* lightShader = new Shader("res/shaders/vertex.glsl", "res/shaders/lighting/light.glsl");


    // Start render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        float time = (float)glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;

        glm::vec3 lightColour;
        lightColour.x = sin(glfwGetTime() * 2.0f);
        lightColour.y = sin(glfwGetTime() * 0.7f);
        lightColour.z = sin(glfwGetTime() * 1.3f);


        camera->HandleKeyboard(window, deltaTime);
        glm::mat4 projection = glm::perspective(glm::radians(fov), 8.0f / 6.0f, 0.1f, 100.0f);  // FOV, Aspect ration, near plane, far plane
        glm::mat4 view = camera->GetViewMatrix();

        // Draw Cube
        {
            cubeShader->Bind();

            glm::mat4 model = glm::mat4(1.0f);

            cubeShader->SetUniformMat4f("uModel", model);
            cubeShader->SetUniformMat4f("uProjection", projection);
            cubeShader->SetUniformMat4f("uView", view);

            cubeShader->SetUniformVec3f("uViewPosition", camera->GetPosition());

            cubeShader->SetUniformVec3f("light.position", lightPosition);
            cubeShader->SetUniformVec3f("light.specular", 1.0f);
            cubeShader->SetUniformVec3f("light.ambient", lightColour * glm::vec3(0.2f));
            cubeShader->SetUniformVec3f("light.diffuse", lightColour * glm::vec3(0.5f));

            cubeShader->SetUniform1f("material.shininess", 32.0f);
            cubeShader->SetUniformVec3f("material.specular", 0.5f);
            cubeShader->SetUniformVec3f("material.ambient", objectColour);
            cubeShader->SetUniformVec3f("material.diffuse", objectColour);

            glBindVertexArray(vertexArray);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Draw Light
        {
            lightShader->Bind();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, lightPosition);
            model = glm::scale(model, glm::vec3(0.2f)); 

            lightShader->SetUniformMat4f("uModel", model);
            lightShader->SetUniformMat4f("uProjection", projection);
            lightShader->SetUniformMat4f("uView", view);

            lightShader->SetUniformVec3f("uLightingColour", lightColour);

            glBindVertexArray(vertexArray);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

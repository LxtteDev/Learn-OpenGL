#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cstdlib>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"

#define ASSERT(x, msg) if (!(x)) { printf("%s\n", msg); exit(1); }
#define GLASSERT(x) if (!(x)) { const char* message; int error = glfwGetError(&message); printf("GLFW Error %i: %s\n", error, message); glfwTerminate(); exit(1); }

int main(int argc, char** argv) {
    GLASSERT(glfwInit());


    // Setup window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Breaking GL", NULL, NULL);
    GLASSERT(window != NULL);
    
    // Load window context
    glfwMakeContextCurrent(window);
    GLASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));


    // Render information
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);


    // Setup square
    float pos = 0.5f;
    float vertices[] = {
        -pos, -pos, -pos,  0.0f, 0.0f,
         pos, -pos, -pos,  1.0f, 0.0f,
         pos,  pos, -pos,  1.0f, 1.0f,
         pos,  pos, -pos,  1.0f, 1.0f,
        -pos,  pos, -pos,  0.0f, 1.0f,
        -pos, -pos, -pos,  0.0f, 0.0f,

        -pos, -pos,  pos,  0.0f, 0.0f,
         pos, -pos,  pos,  1.0f, 0.0f,
         pos,  pos,  pos,  1.0f, 1.0f,
         pos,  pos,  pos,  1.0f, 1.0f,
        -pos,  pos,  pos,  0.0f, 1.0f,
        -pos, -pos,  pos,  0.0f, 0.0f,

        -pos,  pos,  pos,  1.0f, 0.0f,
        -pos,  pos, -pos,  1.0f, 1.0f,
        -pos, -pos, -pos,  0.0f, 1.0f,
        -pos, -pos, -pos,  0.0f, 1.0f,
        -pos, -pos,  pos,  0.0f, 0.0f,
        -pos,  pos,  pos,  1.0f, 0.0f,

         pos,  pos,  pos,  1.0f, 0.0f,
         pos,  pos, -pos,  1.0f, 1.0f,
         pos, -pos, -pos,  0.0f, 1.0f,
         pos, -pos, -pos,  0.0f, 1.0f,
         pos, -pos,  pos,  0.0f, 0.0f,
         pos,  pos,  pos,  1.0f, 0.0f,

        -pos, -pos, -pos,  0.0f, 1.0f,
         pos, -pos, -pos,  1.0f, 1.0f,
         pos, -pos,  pos,  1.0f, 0.0f,
         pos, -pos,  pos,  1.0f, 0.0f,
        -pos, -pos,  pos,  0.0f, 0.0f,
        -pos, -pos, -pos,  0.0f, 1.0f,

        -pos,  pos, -pos,  0.0f, 1.0f,
         pos,  pos, -pos,  1.0f, 1.0f,
         pos,  pos,  pos,  1.0f, 0.0f,
         pos,  pos,  pos,  1.0f, 0.0f,
        -pos,  pos,  pos,  0.0f, 0.0f,
        -pos,  pos, -pos,  0.0f, 1.0f
    };

    glm::vec3 positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    unsigned int vertexArray;
    glGenBuffers(1, &vertexArray);
    glBindVertexArray(vertexArray);

    unsigned int arrayBuffer;
    glGenBuffers(1, &arrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Layout Location, Size, Type, Normalised, Stride, Offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Setup shaders
    Shader* shader = new Shader("res/vertex.glsl", "res/fragment.glsl");


    // Setup textures
    Texture* texture = new Texture("res/texture-border.png");
    shader->SetUniform1i("uTexture", texture->GetTextureID());

    
    // Setup projections
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 8.0f / 6.0f, 0.1f, 100.0f);  // FOV, Aspect ration, near plane, far plane


    // Start render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = glfwGetTime();
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
        view = glm::rotate(view, time * glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        shader->SetUniformMat4f("uProjection", projection);
        shader->SetUniformMat4f("uView", view);
        shader->Bind();

        texture->Bind();
        glBindVertexArray(vertexArray);

        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i] + glm::vec3(0.0f, 0.0f, 5.0f));
            model = glm::rotate(model, time * glm::radians(20.0f * (i + 1)), glm::vec3(1.0f, 0.3f, 0.5f));

            shader->SetUniformMat4f("uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

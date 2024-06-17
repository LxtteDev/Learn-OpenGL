#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cstdlib>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "lightShader.h"
#include "model/model.h"
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

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    printf("%stype = %d; severity = %d; message = %s;\n", (type == GL_DEBUG_TYPE_ERROR ? "OpenGL encounter and error! " : ""), (int)type, (int)severity, message);
}

int main(int argc, char** argv) {
    GLASSERT(glfwInit());
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(MessageCallback, 0);


    // Setup window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    std::vector<Vertex> vertices = {
        { glm::vec3(-0.5f, -0.5f, -0.5f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f) },
        { glm::vec3( 0.5f,  0.5f, -0.5f) },
        { glm::vec3( 0.5f,  0.5f, -0.5f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f) },

        { glm::vec3(-0.5f, -0.5f,  0.5f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f) },
        { glm::vec3( 0.5f,  0.5f,  0.5f) },
        { glm::vec3( 0.5f,  0.5f,  0.5f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f) },

        { glm::vec3(-0.5f,  0.5f,  0.5f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f) },

        { glm::vec3( 0.5f,  0.5f,  0.5f) },
        { glm::vec3( 0.5f,  0.5f, -0.5f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f) },
        { glm::vec3( 0.5f,  0.5f,  0.5f) },

        { glm::vec3(-0.5f, -0.5f, -0.5f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f) },

        { glm::vec3(-0.5f,  0.5f, -0.5f) },
        { glm::vec3( 0.5f,  0.5f, -0.5f) },
        { glm::vec3( 0.5f,  0.5f,  0.5f) },
        { glm::vec3( 0.5f,  0.5f,  0.5f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f) }
    };

    std::vector<unsigned int> indices(vertices.size());
    for (unsigned int i = 0; i < vertices.size(); i++) indices[i] = i;

    // Setup camera
    Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glfwSetWindowUserPointer(window, camera);
    glfwSetScrollCallback(window, HandleScroll);  
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
        static_cast<Camera*>(glfwGetWindowUserPointer(window))->HandleMouse(window, x, y);
    });


    // Setup shaders
    // glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);
    // glm::vec3 lightPosition(0.0f, 1.0f, 0.0f);

    // LightShader* cubeShader = new LightShader("res/shaders/vertex.glsl");
    // cubeShader->AddLightCaster(lightDirection, glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3( 0.5f));
    // cubeShader->AddPointLight(lightPosition, glm::vec3(1.0f), glm::vec3(0.2f), glm::vec3( 0.5f), 1.0f, 0.09f, 0.032f);

    // Shader* cubeShader = new Shader("res/shaders/vertex.glsl", "res/shaders/lighting/diffuse.glsl");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Shader* cubeShader = new Shader("res/shaders/vertex.glsl", "res/shaders/lighting/diffuse.glsl");

    // Setup Textures
    // Texture* crateSpecular = new Texture("res/textures/crateSpecular.png");
    // Texture* crateTexture = new Texture("res/textures/crate.png");


    // Setup model
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    Model* backpack = new Model("res/models/backpack/backpack.obj");


    // Start render loop
    while (!glfwWindowShouldClose(window)) {
        GL_CHECK(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        float time = (float)glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;

        camera->HandleKeyboard(window, deltaTime);
        glm::mat4 projection = glm::perspective(glm::radians(fov), 8.0f / 6.0f, 0.1f, 100.0f);  // FOV, Aspect ration, near plane, far plane
        glm::mat4 view = camera->GetViewMatrix();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.0f));
        model = glm::scale(model, glm::vec3(1.0f));

        backpack->Draw(camera->GetPosition(), projection, model, view);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete backpack;

    glfwTerminate();
    return 0;
}

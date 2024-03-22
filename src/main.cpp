#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cstdlib>

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

    // Start render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

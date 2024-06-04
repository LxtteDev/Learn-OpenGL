#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <signal.h>
#include <stdio.h>

#define GL_CHECK(stmt) do { \
    stmt; \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) { \
        printf("OpenGL error %08x, at %s:%i - for %s\n", err, __FILE__, __LINE__, #stmt); \
        exit(1); \
    } \
} while (0)

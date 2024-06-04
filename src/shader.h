#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glad/gl.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>

#include "shared.h"

class Shader {
    public:
        Shader(const char* vertexPath, const char* fragmentPath) {
            std::string vertexCode = Shader::ReadFile(vertexPath);
            std::string fragmentCode = Shader::ReadFile(fragmentPath);

            unsigned int vertexShader = Shader::CompileShader(GL_VERTEX_SHADER, vertexCode);
            unsigned int fragmentShader = Shader::CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

            this->id = glCreateProgram();
            glAttachShader(this->id, vertexShader);
            glAttachShader(this->id, fragmentShader);
            glLinkProgram(this->id);

            int success;
            char message[256];

            glGetProgramiv(this->id, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(this->id, 256, NULL, message);
                printf("Failed to link shader programme: %s\n", message);
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        };

        Shader() {};

        virtual void Bind() {
            glUseProgram(this->id);
        };

        void SetUniform1i(const char* name, int value) {
            int location = glGetUniformLocation(this->id, name);
            GL_CHECK(glUniform1i(location, value));
        }

        void SetUniform1f(const char* name, float value) {
            int location = glGetUniformLocation(this->id, name);
            GL_CHECK(glUniform1f(location, value));
        }

        void SetUniformMat4f(const char* name, glm::mat4 matrix) {
            int location = glGetUniformLocation(this->id, name);
            GL_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
        }

        void SetUniformVec3f(const char* name, float a, float b, float c) {
            int location = glGetUniformLocation(this->id, name);
            GL_CHECK(glUniform3f(location, a, b, c));
        }

        void SetUniformVec3f(const char* name, float a) {
            Shader::SetUniformVec3f(name, a, a, a);
        }

        void SetUniformVec3f(const char* name, glm::vec3 vector) {
            Shader::SetUniformVec3f(name, vector.x, vector.y, vector.z);
        }

    protected:
        std::string ReadFile(const char* file) {
            std::string contents;

            std::ifstream fileStream;
            fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fileStream.open(file);

            std::stringstream stream;
            stream << fileStream.rdbuf();

            fileStream.close();
            contents = stream.str();

            return contents;
        };

        virtual unsigned int CompileShader(unsigned int type, std::string source) {
            int success;
            char message[256];
            const char* sourceStr = source.c_str();

            unsigned int shader = glCreateShader(type);
            glShaderSource(shader, 1, &sourceStr, NULL);
            glCompileShader(shader);

            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 256, NULL, message);
                printf("Failed to compile %s shader: %s\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
            }

            return shader;
        };

        unsigned int id;
};

#pragma once
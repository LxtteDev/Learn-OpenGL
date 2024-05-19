#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <vector>

#include "../shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

enum TextureType {
    DIFFUSE = 0,
    SPECULAR = 1
};

/* struct Texture {
    unsigned int id;
    TextureType type;
}; */

class Mesh {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices /*, std::vector<Texture> textures*/) {
            this->vertices = vertices;
            this->indices = indices;
            // this->textures = textures;

            Mesh::SetupMesh();
        };

        ~Mesh() {
            glDeleteVertexArrays(1, &(this->vertexArray));
            glDeleteBuffers(1, &(this->vertexBuffer));
            glDeleteBuffers(1, &(this->elementBuffer));
        };

        void Draw() {
            unsigned int i = 0;
            unsigned int j = 0;

            /* for (unsigned int k = 0; k < this->textures.size(); k++) {
                glActiveTexture(GL_TEXTURE0 + k);

                switch (this->textures[k].type) {
                    case TextureType::DIFFUSE:
                        shader->SetUniform1i(("Mesh_Diffuse" + std::to_string(i++)).c_str(), this->textures[k].id);
                        break;
                    case TextureType::SPECULAR:
                        shader->SetUniform1i(("Mesh_Specular" + std::to_string(j++)).c_str(), this->textures[k].id);
                        break;
                }

                glBindTexture(GL_TEXTURE_2D, this->textures[k].id);
            } */

            // glActiveTexture(GL_TEXTURE0);

            glBindVertexArray(this->vertexArray);
            glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        };

    private:
        void SetupMesh() {
            // Generate buffers
            glGenVertexArrays(1, &(this->vertexArray));
            glGenBuffers(1, &(this->vertexBuffer));
            glGenBuffers(1, &(this->elementBuffer));

            // Bind data
            glBindVertexArray(this->vertexArray);

            glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &(this->vertices[0]), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &(this->indices[0]), GL_STATIC_DRAW);

            // Set vertex attributes
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

            // Clean up
            glBindVertexArray(0);
        };

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        // std::vector<Texture> textures;

        unsigned int vertexArray, vertexBuffer, elementBuffer;
};

#pragma once
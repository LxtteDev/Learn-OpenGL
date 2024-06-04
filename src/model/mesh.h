#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <vector>

#include "../shader.h"
#include "../shared.h"


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
            this->vertices.resize(vertices.size());
            this->indices.resize(indices.size());
            // this->textures = textures;

            std::copy(vertices.begin(), vertices.end(), this->vertices.begin());
            std::copy(indices.begin(), indices.end(), this->indices.begin());

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

            glGetError();
            GL_CHECK(glBindVertexArray(this->vertexArray));
            GL_CHECK(glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0));
            GL_CHECK(glBindVertexArray(0));
        };

        void Debug(int id = -1) {
            if (id == -1) printf("--- Mesh Debug Info ---\n");
            else printf("--- Mesh Debug Info (%i) ---\n", id);

            printf("[ Vertex information ]\n");
            printf("Vertices:  %zu\n", this->vertices.size());
            printf("Indices:   %zu\n", this->indices.size());
            printf("Max Index: %u\n", *(std::max_element(this->indices.begin(), this->indices.end())));

            printf("\n[ Buffer information ]\n");
            printf("Vertex Array:   %i\n", this->vertexArray);
            printf("Vertex Buffer:  %i\n", this->vertexBuffer);
            printf("Element Buffer: %i\n", this->elementBuffer);

            printf("\n[ Data information ]\n");
            printf("Vertex Data: %p\n", &(this->vertices[0]));
            printf("Index Data:  %p\n", &(this->indices[0]));

            if (id == -1) printf("-----------------------\n");
            else printf("---------------------------\n", id);
        };

        bool Check() {
            return (this->vertices.size() > 0 && this->indices.size() > 0)
                && (this->vertexArray > 0 && this->vertexBuffer > 0 && this->elementBuffer > 0)
                && (*(std::max_element(this->indices.begin(), this->indices.end())) < this->indices.size());
        }

    private:
        void SetupMesh() {
            // Generate buffers
            glGetError();
            GL_CHECK(glGenVertexArrays(1, &(this->vertexArray)));
            GL_CHECK(glGenBuffers(1, &(this->vertexBuffer)));
            GL_CHECK(glGenBuffers(1, &(this->elementBuffer)));

            // Bind data
            GL_CHECK(glBindVertexArray(this->vertexArray));

            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer));
            GL_CHECK(glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &(this->vertices[0]), GL_STATIC_DRAW));

            GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer));
            GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &(this->indices[0]), GL_STATIC_DRAW));

            // Set vertex attributes
            GL_CHECK(glEnableVertexAttribArray(0));
            GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

            // Clean up
            GL_CHECK(glBindVertexArray(0));
        };

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        // std::vector<Texture> textures;

        unsigned int vertexArray, vertexBuffer, elementBuffer;
};

#pragma once
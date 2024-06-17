#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <algorithm>
#include <vector>

#include "../texture.h"
#include "../shader.h"
#include "../shared.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct Material {
    int specularExp; // Ns
    glm::vec3 ambient;  // Ka
    glm::vec3 diffuse;  // Kd
    glm::vec3 specular; // Ks
    glm::vec3 emissive; // Ke
    Texture* diffuseTexture;  // map_Kd
    Texture* normalTexture;   // map_Bump
    Texture* specularTexture; // map_Ks
};

class Mesh {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* material) {
            this->vertices.resize(vertices.size());
            this->indices.resize(indices.size());

            this->material = material;

            std::copy(vertices.begin(), vertices.end(), this->vertices.begin());
            std::copy(indices.begin(), indices.end(), this->indices.begin());

            GL_CHECK();
            this->shader = new Shader("res/shaders/vertex.glsl", "res/shaders/lightingPlus/materialTexture.glsl");
            this->shader->Bind();

            this->shader->SetUniformVec3f("light.position", 1.2f, 1.0f, 2.0f);
            this->shader->SetUniformVec3f("light.specular", 1.0f, 1.0f, 1.0f);
            this->shader->SetUniformVec3f("light.ambient", 1.0f, 1.0f, 1.0f);
            this->shader->SetUniformVec3f("light.diffuse", 1.0f, 1.0f, 1.0f);
            // this->shader->SetUniformVec3f("uObjectColour", material->diffuse);

            this->shader->SetUniform1f("material.shininess", 1.0f);

            Mesh::SetupMesh();
        };

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
            if (this->material->diffuseTexture->GetTextureID() != -1) {
                GL_CHECK(glActiveTexture(GL_TEXTURE0 + this->material->diffuseTexture->GetTextureID() - 1));
                this->shader->SetUniform1i("material.diffuse", this->material->diffuseTexture->GetTextureID());
                GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->material->diffuseTexture->GetTextureID()));
            }

            // this->shader->SetUniform1i("material.specular", material->specularTexture->GetTextureID());

            GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

            glGetError();
            GL_CHECK(glBindVertexArray(this->vertexArray));
            GL_CHECK(glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0));
            GL_CHECK(glBindVertexArray(0));
        };

        void Draw(glm::vec3 camera, glm::mat4 projection, glm::mat4 model, glm::mat4 view) {
            this->shader->Bind();

            this->shader->SetUniformMat4f("uProjection", projection);
            this->shader->SetUniformMat4f("uModel", model);
            this->shader->SetUniformMat4f("uView", view);

            this->shader->SetUniformVec3f("uViewPosition", camera);

            Mesh::Draw();
        }

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

            GL_CHECK(glEnableVertexAttribArray(1));
            GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

            GL_CHECK(glEnableVertexAttribArray(2));
            GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)));

            // Clean up
            GL_CHECK(glBindVertexArray(0));
        };

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        Material* material;
        Shader* shader;

        unsigned int vertexArray, vertexBuffer, elementBuffer;
};

#pragma once
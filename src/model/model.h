#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <filesystem>
#include <signal.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "../texture.h"
#include "mesh.h"

class Model {
    public:
        Model(std::string path) {
            this->path = path.substr(0, path.find_last_of('/'));
            Model::LoadModel(path);
        };

        void Draw() {
            for (unsigned int i = 0; i < this->meshes.size(); i++)
                this->meshes[i]->Draw();
        };

        void Debug() {
            for (unsigned int i = 0; i < this->meshes.size(); i++)
                this->meshes[i]->Debug(i);
        }

    private:
        void LoadModel(std::string path) {
            std::filesystem::path current(std::filesystem::current_path());
            std::filesystem::path mPath(path);
            std::filesystem::path full = current / mPath;

            std::ifstream file;
            // file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            file.open(full.c_str());

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            unsigned int indexOffset = 1U;

            std::string line;
            while (std::getline(file, line)) {
                if (line.size() == 0) continue;

                std::stringstream stream(line);

                std::string prefix;
                stream >> prefix;

                if (prefix == "o") {
                    if (vertices.size() > 0 && indices.size() > 0) {
                        Mesh* mesh = new Mesh(vertices, indices);
                        if (!mesh->Check()) mesh->Debug(this->meshes.size());

                        this->meshes.push_back(mesh);

                        indexOffset += vertices.size();
                        vertices.clear();
                        indices.clear();
                    }
                } if (prefix == "v") {
                    float x, y, z;
                    stream >> x >> y >> z;
                    
                    Vertex vertex;
                    vertex.position = glm::vec3(x, y, z);

                    vertices.push_back(vertex);
                } else if (prefix == "f") {
                    std::string f1, f2, f3, f4;
                    stream >> f1 >> f2 >> f3 >> f4;

                    std::string v1 = f1.substr(0, f1.find("/"));
                    std::string v2 = f2.substr(0, f2.find("/"));
                    std::string v3 = f3.substr(0, f3.find("/"));
                    std::string v4 = f4.substr(0, f4.find("/"));

                    if (v4.size() > 0) {
                        printf("(Four Vertices) %s %s %s %s\n", v1.c_str(), v2.c_str(), v3.c_str(), v4.c_str());

                        indices.push_back(std::stoi(v1) - indexOffset);
                        indices.push_back(std::stoi(v2) - indexOffset);
                        indices.push_back(std::stoi(v3) - indexOffset);

                        indices.push_back(std::stoi(v1) - indexOffset);
                        indices.push_back(std::stoi(v3) - indexOffset);
                        indices.push_back(std::stoi(v4) - indexOffset);
                    } else {
                        indices.push_back(std::stoi(v1) - indexOffset);
                        indices.push_back(std::stoi(v2) - indexOffset);
                        indices.push_back(std::stoi(v3) - indexOffset);
                    }
                } else if (prefix == "c") {
                    std::string i, r, g, b;
                    stream >> i >> r >> g >> b;

                    glm::vec3 colour = glm::vec3(std::stof(r), std::stof(g), std::stof(b));

                    vertices[std::stoi(i) - 1U].normal = colour;
                }
            }

            file.close();

            if (vertices.size() > 0 && indices.size() > 0) {
                Mesh* mesh = new Mesh(vertices, indices);
                if (!mesh->Check()) mesh->Debug(this->meshes.size());

                this->meshes.push_back(mesh);

                vertices.clear();
                indices.clear();
            }
        }
        
        std::vector<Mesh*> meshes;
        std::string path;
};

#pragma once
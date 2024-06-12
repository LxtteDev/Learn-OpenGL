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
#include <tuple>
#include <map>

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

        void Draw(glm::mat4 projection, glm::mat4 model, glm::mat4 view) {
            for (unsigned int i = 0; i < this->meshes.size(); i++)
                this->meshes[i]->Draw(projection, model, view);
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
            std::string material = "";

            unsigned int positionOffset = 1U;
            std::vector<glm::vec3> positions;
            unsigned int normalOffset = 1U;
            std::vector<glm::vec3> normals;
            unsigned int uvOffset = 1U;
            std::vector<glm::vec2> uvs;

            std::map<std::tuple<int, int, int>, int> uniqueVertices;

            std::string line;
            while (std::getline(file, line)) {
                if (line.size() == 0) continue;

                std::stringstream stream(line);

                std::string prefix;
                stream >> prefix;

                if (prefix == "mtllib") {
                    std::string matFile;
                    stream >> matFile;

                    std::filesystem::path matPath = full.parent_path() / matFile;

                    Model::LoadMaterial(matPath.c_str());
                } else if (prefix == "usemtl") {
                    stream >> material;                
                } else if (prefix == "o") {
                    if (vertices.size() > 0 && indices.size() > 0) {
                        if (material.size() > 0) {
                            Mesh* mesh = new Mesh(vertices, indices, this->materials[material]);
                            if (!mesh->Check()) mesh->Debug(this->meshes.size());

                            this->meshes.push_back(mesh);
                        } else {
                            Mesh* mesh = new Mesh(vertices, indices);
                            if (!mesh->Check()) mesh->Debug(this->meshes.size());

                            this->meshes.push_back(mesh);
                        }

                        vertices.clear();
                        indices.clear();
                        material = "";

                        positionOffset += positions.size(); 
                        positions.clear();
                        normalOffset += normals.size(); 
                        normals.clear();
                        uvOffset += uvs.size(); 
                        uvs.clear();

                        uniqueVertices.clear();
                    }
                } if (prefix == "v") { // Vertex Positions
                    float x, y, z;
                    stream >> x >> y >> z;
                    
                    positions.push_back(glm::vec3(x, y, z));
                } else if (prefix == "vn") { // Vertex Normals
                    float x, y, z;
                    stream >> x >> y >> z;

                    normals.push_back(glm::vec3(x, y, z));
                } else if (prefix == "vt") { // UV Coords
                    float x, y;
                    stream >> x >> y;

                    uvs.push_back(glm::vec2(x, y));          
                } else if (prefix == "f") {
                    std::string f1, f2, f3, f4;
                    stream >> f1 >> f2 >> f3 >> f4;

                    std::vector<std::string> v1 = Model::ProccessFace(f1);
                    std::vector<std::string> v2 = Model::ProccessFace(f2);
                    std::vector<std::string> v3 = Model::ProccessFace(f3);

                    // Vertices
                    int i = vertices.size();

                    std::tuple<int, int, int> t1 = std::make_tuple(std::stoi(v1[0]) - 1, std::stoi(v1[1]) - 1, std::stoi(v1[2]) - 1);
                    if (uniqueVertices.find(t1) == uniqueVertices.end()) {
                        Vertex vertex1;
                        vertex1.position = positions[std::stoi(v1[0]) - positionOffset];
                        vertex1.normal = normals[std::stoi(v1[2]) - normalOffset];
                        vertex1.uv = uvs[std::stoi(v1[1]) - uvOffset];

                        vertices.push_back(vertex1);
                        uniqueVertices[t1] = vertices.size() - 1;
                        indices.push_back(vertices.size() - 1);
                    } else indices.push_back(uniqueVertices[t1]);

                    std::tuple<int, int, int> t2 = std::make_tuple(std::stoi(v2[0]) - 1, std::stoi(v2[1]) - 1, std::stoi(v2[2]) - 1);
                    if (uniqueVertices.find(t2) == uniqueVertices.end()) {
                        Vertex vertex2;
                        vertex2.position = positions[std::stoi(v2[0]) - positionOffset];
                        vertex2.normal = normals[std::stoi(v2[2]) - normalOffset];
                        vertex2.uv = uvs[std::stoi(v2[1]) - uvOffset];

                        vertices.push_back(vertex2);
                        uniqueVertices[t2] = vertices.size() - 1;
                        indices.push_back(vertices.size() - 1);
                    } else indices.push_back(uniqueVertices[t2]);

                    std::tuple<int, int, int> t3 = std::make_tuple(std::stoi(v3[0]) - 1, std::stoi(v3[1]) - 1, std::stoi(v3[2]) - 1);
                    if (uniqueVertices.find(t3) == uniqueVertices.end()) {
                        Vertex vertex3;
                        vertex3.position = positions[std::stoi(v3[0]) - positionOffset];
                        vertex3.normal = normals[std::stoi(v3[2]) - normalOffset];
                        vertex3.uv = uvs[std::stoi(v3[1]) - uvOffset];

                        vertices.push_back(vertex3);
                        uniqueVertices[t3] = vertices.size() - 1;
                        indices.push_back(vertices.size() - 1);
                    } else indices.push_back(uniqueVertices[t3]);

                    // Fourth vertex
                    /* if (f4.size() > 0) {
                        printf("Fourth vertex\n");
                        std::string v4 = f4.substr(0, f4.find("/"));
                        std::string vn4 = f4.substr(v4.length() + 1, f4.find("/", v4.length() + 1) - 2);
                        std::string vt4 = f4.substr(v4.length() + vn4.length() + 2, f4.find("/", v4.length() + vn4.length() + 2) - 3);

                        std::tuple<int, int, int> t4 = std::make_tuple(std::stoi(v4) - 1, std::stoi(vn4) - 1, std::stoi(vt4) - 1);
                        if (uniqueVertices.find(t4) == uniqueVertices.end()) {
                            Vertex vertex4;
                            vertex4.position = positions[std::stoi(v4) - positionOffset];
                            vertex4.normal = normals[std::stoi(vn4) - normalOffset];
                            vertex4.uv = uvs[std::stoi(vt4) - uvOffset];

                            vertices.push_back(vertex4);
                            uniqueVertices[t4] = vertices.size() - 1;
                            
                            indices.push_back(uniqueVertices[t1]);
                            indices.push_back(uniqueVertices[t3]);
                            indices.push_back(vertices.size() - 1);
                        }
                    } */
                }
            }

            file.close();

            if (vertices.size() > 0 && indices.size() > 0) {
                if (material.size() > 0) {
                    Mesh* mesh = new Mesh(vertices, indices, this->materials[material]);
                    if (!mesh->Check()) mesh->Debug(this->meshes.size());

                    this->meshes.push_back(mesh);
                } else {
                    Mesh* mesh = new Mesh(vertices, indices);
                    if (!mesh->Check()) mesh->Debug(this->meshes.size());

                    this->meshes.push_back(mesh);
                }

                vertices.clear();
                indices.clear();

                positions.clear();
                normals.clear();
                uvs.clear();

                uniqueVertices.clear();
            }
        }

        void LoadMaterial(std::string path) {
            std::filesystem::path root = std::filesystem::path(path).parent_path();
            std::ifstream file;
            file.open(path);

            std::string line;
            while (std::getline(file, line)) {
                if (line.size() == 0) continue;

                std::stringstream stream(line);

                std::string type;
                stream >> type;

                std::string current = "";
                Material* material;

                if (type == "newmtl") {
                    stream >> current;
                    material = new Material();

                    this->materials[current] = material;
                } else if (type == "Ns") {
                    std::string Ns;
                    stream >> Ns;

                    material->specularExp = std::stoi(Ns);
                } else if (type == "Ka") {
                    std::string a, b, c;
                    stream >> a >> b >> c;

                    material->ambient = glm::vec3(std::stof(a), std::stof(b), std::stof(c));
                } else if (type == "Kd") {
                    std::string a, b, c;
                    stream >> a >> b >> c;

                    material->diffuse = glm::vec3(std::stof(a), std::stof(b), std::stof(c));
                } else if (type == "Ks") {
                    std::string a, b, c;
                    stream >> a >> b >> c;

                    material->specular = glm::vec3(std::stof(a), std::stof(b), std::stof(c));
                } else if (type == "Ke") {
                    std::string a, b, c;
                    stream >> a >> b >> c;

                    material->emissive = glm::vec3(std::stof(a), std::stof(b), std::stof(c));
                } else if (type == "map_Kd") {
                    std::string map_Kd;
                    stream >> map_Kd;

                    material->diffuseTexture = (root / map_Kd).string();
                } else if (type == "map_Bump") {
                    std::string map_Bump;
                    stream >> map_Bump;

                    material->normalTexture = (root / map_Bump).string();
                } else if (type == "map_Ks") {
                    std::string map_Ks;
                    stream >> map_Ks;

                    material->specularTexture = (root / map_Ks).string();
                }
            }
        }

        std::vector<std::string> ProccessFace(std::string face) {
            std::vector<std::string> result;

            if (face.find("/") == std::string::npos) return { face };

            int pos;
            while ((pos = face.find("/")) != std::string::npos) {
                result.push_back(face.substr(0, pos));
                face.erase(0, pos + 1);
            }

            if (face.length() > 0) result.push_back(face);

            return result;
        }
        
        std::unordered_map<std::string, Material*> materials;
        std::vector<Mesh*> meshes;
        std::string path;
};

#pragma once
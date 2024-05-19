#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "shader.h"

struct Light {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction = glm::vec3(0.0f);

    glm::vec3 specular = glm::vec3(1.0f);
    glm::vec3 ambient = glm::vec3(0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f);

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    float cutoff = glm::cos(glm::radians(12.5f));
    float outerCutoff = glm::cos(glm::radians(17.5f));

    int type;
};

class LightShader : public Shader {
    public:
        LightShader(const char* vertexPath)
        : Shader() {
            std::string vertexCode = Shader::ReadFile(vertexPath);
            this->vertexShader = Shader::CompileShader(GL_VERTEX_SHADER, vertexCode);

            this->Rebuild();
        };

        void Bind() override {
            if (this->lights.size() != this->currentLights) this->Rebuild();
            this->currentLights = this->lights.size();
            Shader::Bind();

            for (unsigned int i = 0; i < this->lights.size(); i++) {
                std::string name = "uLights[" + std::to_string(i) + "].";
                // printf("(%s) %f, %f, %f\n", name.append("diffuse").c_str(), this->lights[i].diffuse.x, this->lights[i].diffuse.y, this->lights[i].diffuse.z);
                Shader::SetUniformVec3f((name + "position").c_str(),    this->lights[i].position);
                Shader::SetUniformVec3f((name + "direction").c_str(),   this->lights[i].direction);

                Shader::SetUniformVec3f((name + "ambient").c_str(),     this->lights[i].ambient);
                Shader::SetUniformVec3f((name + "diffuse").c_str(),     this->lights[i].diffuse);
                Shader::SetUniformVec3f((name + "specular").c_str(),    this->lights[i].specular);

                Shader::SetUniform1f(   (name + "constant").c_str(),    this->lights[i].constant);
                Shader::SetUniform1f(   (name + "linear").c_str(),      this->lights[i].linear);
                Shader::SetUniform1f(   (name + "quadratic").c_str(),   this->lights[i].quadratic);

                Shader::SetUniform1f(   (name + "cutoff").c_str(),      this->lights[i].cutoff);
                Shader::SetUniform1f(   (name + "outerCutoff").c_str(), this->lights[i].outerCutoff);

                Shader::SetUniform1i(   (name + "type").c_str(),        this->lights[i].type);
            }
        };

        void AddLightCaster(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
            Light light;
            light.direction = direction;
            light.ambient = ambient;
            light.diffuse = diffuse;
            light.specular = specular;
            light.type = 0;

            this->lights.push_back(light);
        }

        void AddPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
            Light light;
            light.position = position;
            light.ambient = ambient;
            light.diffuse = diffuse;
            light.specular = specular;
            light.type = 1;

            this->lights.push_back(light);
        }

        void AddPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) {
            Light light;
            light.position = position;
            light.ambient = ambient;
            light.diffuse = diffuse;
            light.specular = specular;
            light.constant = constant;
            light.linear = linear;
            light.quadratic = quadratic;
            light.type = 1;

            this->lights.push_back(light);
        }

        void AddSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutoff, float outerCutoff) {
        }

        void AddLight(Light light) {
            this->lights.push_back(light);
        }

    protected:
        unsigned int CompileShader(unsigned int type, std::string source) override {
            int success;
            char message[256];
            const char* sourceStr = type == GL_FRAGMENT_SHADER ? std::string("#version 330 core\n#define NR_LIGHTS ").append(std::to_string(this->lights.size())).append("\n").append(std::string(source)).c_str() : source.c_str();
            // const char* sourceStr = source.c_str();
            printf("%s\n-----\n", sourceStr);
            // printf("%zu\n", this->lights.size());

            unsigned int shader = glCreateShader(type);
            glShaderSource(shader, 1, &sourceStr, NULL);
            glCompileShader(shader);

            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 256, NULL, message);
                printf("Failed to compile %s shader (LightShader): %s\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
            }

            return shader;
        };

        void Rebuild() {
            if (this->id != 0) glDeleteProgram(this->id);

            std::string fragmentCode = Shader::ReadFile("res/shaders/lighting.glsl");
            unsigned int fragmentShader = LightShader::CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

            this->id = glCreateProgram();
            glAttachShader(this->id, this->vertexShader);
            glAttachShader(this->id, fragmentShader);
            glLinkProgram(this->id);

            int success;
            char message[256];

            glGetProgramiv(this->id, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(this->id, 256, NULL, message);
                printf("Failed to link shader programme: %s\n", message);
            }

            glDeleteShader(fragmentShader);
        }

    private:
        unsigned int vertexShader;

        std::vector<Light> lights;
        int currentLights;
};
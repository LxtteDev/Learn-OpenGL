#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture {
    public:
        Texture(const char* path) {
            int width, height, channels;
            stbi_set_flip_vertically_on_load(true);  
            unsigned char* data = stbi_load(path, &width, &height, &channels, 4);

            glGenTextures(1, &(this->id));
            this->Bind();

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            if (data) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                // glGenerateMipmap(GL_TEXTURE_2D); // Mipmaps can help reduce memory usage
            } else printf("Failed to load texture data\n");

            stbi_image_free(data);
        };

        void Bind() {
            glActiveTexture(GL_TEXTURE0 + this->id);
            glBindTexture(GL_TEXTURE_2D, this->id);
        };

        unsigned int GetTextureID() {
            return this->id;
        };
    
    private:
        unsigned int id;
};

#pragma once
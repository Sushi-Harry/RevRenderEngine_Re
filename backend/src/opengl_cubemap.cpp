#include "glad/glad.h"
#include "renderer/cubemap.hpp"
#include "stb_image.h"
#include <iostream>


Cubemap::Cubemap(const std::vector<std::string>& faces){
    stbi_set_flip_vertically_on_load_thread(0);
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_id);

    int width = 0;
    int height = 0;
    int nrChannels = 0;
    unsigned char *data = stbi_load(faces[0].c_str(), &width, &height, &nrChannels, 0);
    if(!data){
        std::cerr << "ERROR::CUBEMAP::BASE_FACE_COULD_NOT_LOAD" << '\n';
        return;
    }
    glTextureStorage2D(_id, 1, GL_RGB8, width, height);
    glTextureSubImage3D(_id, 0, 0, 0, 0, width, width, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    for(int i = 1; i < faces.size(); i++){
        data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if(data){
            glTextureSubImage3D(_id, 0, 0, 0, i, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }else{
            std::cerr << "ERROR::CUBEMAP::BASE_FACE_COULD_NOT_LOAD" << '\n';
        }
    }
    glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load_thread(1);
}

void Cubemap::bind(uint32_t slot) const {
    glBindTextureUnit(slot, _id);
}

Cubemap::~Cubemap(){
    glDeleteTextures(1, &_id);
}

Cubemap& Cubemap::operator=(Cubemap&& other) noexcept{
    if(this != &other){
        glDeleteTextures(1, &_id);
        _id = other._id;
        other._id = 0;
    }
    return *this;
}

//
// Created by 从径源 on 2024/7/22.
//

#ifndef LEARNOPENGL_UTILS_H
#define LEARNOPENGL_UTILS_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
namespace Utils{
    unsigned int loadTexture2D(char const*path,bool gammaCorrection){
        unsigned int textureID;
        glGenTextures(1,&textureID);
        int width,height,nChannels;
        unsigned char*data = stbi_load(path,&width,&height,&nChannels,0);
        if(data){
            GLenum internalFormat;
            GLenum dataFormat;
            switch (nChannels) {
                case(1):
                    internalFormat = GL_RED;
                    dataFormat = GL_RED;
                    break;
                case(3):
                    internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
                    dataFormat = GL_RGB;
                    break;
                case(4):
                    internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
                    dataFormat = GL_RGBA;
                    break;
            }
            glBindTexture(GL_TEXTURE_2D,textureID);
            glTexImage2D(GL_TEXTURE_2D,0,internalFormat,width,height,0,dataFormat,GL_UNSIGNED_BYTE,data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            stbi_image_free(data);
        }else{
            std::cout << "Texture failed to load at path:" << path << std::endl;
            stbi_image_free(data);
        }
        return textureID;
    }

    unsigned int loadTexture3D(char const*path,bool isHDR,bool gammaCorrection){
        std::vector<std::string> faces
                {
                        "right.jpg",
                        "left.jpg",
                        "top.jpg",
                        "bottom.jpg",
                        "front.jpg",
                        "back.jpg"
                };
        unsigned int cubeId;
        glGenTextures(1,&cubeId);
        glBindTexture(GL_TEXTURE_CUBE_MAP,cubeId);
        int width,height,nChannels;
        unsigned char* data;
        for(unsigned int i = 0;i<6;++i){
            data = stbi_load((std::string(path) + "/" + faces[i]).c_str(),&width,&height,&nChannels,0);
            GLuint format;
            GLuint internalFormat;
            if(nChannels == 1) {
                if(isHDR){
                    internalFormat = GL_R16F;
                }else{
                    internalFormat = GL_RED;
                }
                    format = GL_RED;
            }
            if(nChannels == 3){
                if(isHDR){
                    internalFormat = GL_RGB16F;
                }else if(gammaCorrection) {
                    internalFormat = GL_SRGB;
                }else{
                    internalFormat = GL_RGB;
                }
                format = GL_RGB;
            }
            if(nChannels == 4)
            {
                if(isHDR){
                    internalFormat = GL_RGBA16F;
                }else if(gammaCorrection){
                    internalFormat = GL_SRGB_ALPHA;
                }else{
                    internalFormat = GL_RGBA;
                }
                format = GL_RGBA;
            }
            if(data){
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,internalFormat,width,height,0,format,GL_UNSIGNED_BYTE,data);
                stbi_image_free(data);
            }else {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
        return cubeId;
    }

    unsigned int loadTextureHDR(char const*path){
        stbi_set_flip_vertically_on_load(true);
        int width,height,nrComponents;
        float *data = stbi_loadf(path, &width, &height, &nrComponents, 0);
        unsigned int hdrTexture;
        if (data)
        {
            glGenTextures(1, &hdrTexture);
            glBindTexture(GL_TEXTURE_2D, hdrTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load HDR image." << std::endl;
        }
        return hdrTexture;
    }
    auto clip = [](float a,float lo,float hi)->float{return std::max(lo,std::min(a,hi));};
}


#endif //LEARNOPENGL_UTILS_H

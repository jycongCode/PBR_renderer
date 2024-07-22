//
// Created by 从径源 on 2024/7/12.
//

#ifndef LEARNOPENGL_QUAD_H
#define LEARNOPENGL_QUAD_H
#include <Shader.h>

float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

class Quad{
public:
    Quad(unsigned int width,unsigned int height){
        // VAO,VBO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);

        // FBO,RBO
        glGenFramebuffers(1,&quadFBO);
        glBindFramebuffer(GL_FRAMEBUFFER,quadFBO);

        glGenTextures(1,&quadTex);
        glBindTexture(GL_TEXTURE_2D,quadTex);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,quadTex,0);

        glGenRenderbuffers(1,&quadRBO);
        glBindRenderbuffer(GL_RENDERBUFFER,quadRBO);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,width,height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,quadRBO);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

    void Draw(Shader shader){
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glBindVertexArray(quadVAO);
        shader.use();
        shader.setInt("texture0",0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,quadTex);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        glBindVertexArray(0);
    }

    void Draw(Shader shader,GLuint texture){
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(quadVAO);
        shader.use();
        if(texture!=0){
            shader.setInt("texture0",0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,texture);
        }
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        glBindVertexArray(0);
    }

    void DDraw(Shader shader,GLuint gPosition,GLuint gNormal,GLuint gColorSpec){
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(quadVAO);
        shader.use();
        shader.setInt("gPosition",0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,gPosition);

        shader.setInt("gNormal",1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,gNormal);

        shader.setInt("gColorSpec",2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,gColorSpec);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        glBindVertexArray(0);
    }

    void Destroy(){
        glDeleteBuffers(1,&quadVBO);
        glDeleteVertexArrays(1,&quadVAO);
        glDeleteFramebuffers(1,&quadFBO);
        glDeleteTextures(1,&quadTex);
        glDeleteRenderbuffers(1,&quadRBO);
    }

    void ActivateFrameBuffer(){
        glBindFramebuffer(GL_FRAMEBUFFER,quadFBO);
    }
private:
    GLuint quadVAO,quadVBO;
    GLuint quadFBO,quadTex,quadRBO;
};

#endif //LEARNOPENGL_QUAD_H

//
// Created by 从径源 on 2024/7/22.
//

#ifndef PBR_SKYBOX_H
#define PBR_SKYBOX_H
#include "Cube.h"
#include "Shader.h"
#include "Utils.h"
#include "Camera.h"
#include "Window.h"
class Skybox:Cube{
public:
    Skybox(const char* filePath,bool gammaCorrection){
        texture = Utils::loadTexture3D(filePath,false,gammaCorrection);
        shader = new Shader("Skybox");
    }

    void Draw(Shader skyboxShader) override{
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxShader.setInt("skybox",0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,texture);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,0);
        glDepthFunc(GL_LESS);
    }

    void Draw(GLuint skyboxTex,Camera camera,Window window){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glDepthFunc(GL_LEQUAL);
        shader->use();
        shader->setInt("skybox",0);
        shader->setFloat("lod",3.4);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,skyboxTex);
        glm::mat4 view = camera.GetViewMatrix();
        float scrWidth = static_cast<float>(window.GetWindowWidth());
        float scrHeight = static_cast<float>(window.GetWindowHeight());
        float aspectRatio = scrWidth / scrHeight;
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),aspectRatio,camera.nearPlane,camera.farPlane);
        shader->setMat4("view",view);
        shader->setMat4("projection",projection);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindTexture(GL_TEXTURE_CUBE_MAP,0);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

    void Draw(Camera camera,Window window){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        shader->use();
        shader->setInt("skybox",0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,texture);
        glm::mat4 view = camera.GetViewMatrix();
        float scrWidth = static_cast<float>(window.GetWindowWidth());
        float scrHeight = static_cast<float>(window.GetWindowHeight());
        float aspectRatio = scrWidth / scrHeight;
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),aspectRatio,camera.nearPlane,camera.farPlane);
        shader->setMat4("view",view);
        shader->setMat4("projection",projection);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindTexture(GL_TEXTURE_CUBE_MAP,0);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

    void Destroy() override{
        glDeleteVertexArrays(1,&cubeVAO);
        glDeleteBuffers(1,&cubeVBO);
        shader->Destroy();
        delete(shader);
        glDeleteTextures(1,&texture);
    }

private:
    GLuint texture;
    Shader *shader;
};

#endif //PBR_SKYBOX_H

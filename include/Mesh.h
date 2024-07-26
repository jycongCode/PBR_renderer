//
// Created by 从径源 on 2024/6/30.
//

#ifndef HELLOGL_MESH_H
#define HELLOGL_MESH_H
#include <iostream>
#include <vector>
#include <Shader.h>
#include <assimp/scene.h>
#include "Camera.h"
#include "Window.h"
struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
};

struct Texture{
    unsigned int id;
    std::string type;
    aiString path;
};

class Mesh{
public :
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,std::vector<Texture> textures){
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        setupMesh();
    }

    void Draw(Camera camera,Window window,Transform transform,Shader shader,PBRMaterial material,DirLight light,IBL ibl,bool isTextureIncluded = false){
        shader.use();
        shader.setMat4("model",transform.GetModel());
        glm::mat4 view = camera.GetViewMatrix();
        float scrWidth = static_cast<float>(window.GetWindowWidth());
        float scrHeight = static_cast<float>(window.GetWindowHeight());
        float aspectRatio = scrWidth / scrHeight;
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),aspectRatio,camera.nearPlane,camera.farPlane);
        shader.setMat4("view",view);
        shader.setMat4("projection",projection);
        // camera
        shader.setVec3("viewPos",camera.Position);
        // light
        shader.setVec3("light.Direction",light.Direction);
        shader.setVec3("light.Color",light.Color);
        if(isTextureIncluded){
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int reflectNr = 1;
            unsigned int normalNr = 1;
            for(unsigned int i = 0;i<textures.size();i++){
                glActiveTexture(GL_TEXTURE0+i);
                std::string number;
                std::string name = textures[i].type;
                if(name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if(name == "texture_specular")
                    number = std::to_string(specularNr++);
                else if(name == "texture_reflect")
                    number = std::to_string(reflectNr++);
                else if(name == "texture_normal")
                    number = std::to_string(normalNr++);
//            std::cout << "texture loaded:" << name << number << std::endl;
                shader.setInt(("material."+name+number),i);
                glBindTexture(GL_TEXTURE_2D,textures[i].id);
            }
        }else{
            // material
            shader.setInt("material.albedo",0);
            shader.setInt("material.metallic",1);
            shader.setInt("material.roughness",2);
            shader.setInt("material.normal",3);
            shader.setInt("irradianceMap",4);
            shader.setInt("prefilterMap",5);
            shader.setInt("brdfLUT",6);
            shader.setFloat("material.ao",material.ao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,material.albedoTex);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D,material.metallicTex);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D,material.roughnessTex);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D,material.normalTex);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_CUBE_MAP,ibl.irr.irradianceMap);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_CUBE_MAP,ibl.spec.prefilterMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D,ibl.spec.integrateBRDFMap);
        }
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
    }

    void Destroy(){
        glDeleteBuffers(1,&VBO);
        glDeleteBuffers(1,&EBO);
        glDeleteVertexArrays(1,&VAO);
        for(unsigned int i = 0;i<textures.size();++i){
            glDeleteTextures(1,&textures[i].id);
        }
    }
    unsigned int VAO;
private:
    unsigned int VBO,EBO;
    void setupMesh(){
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glGenBuffers(1,&EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);

        glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(offsetof(Vertex,Normal)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(offsetof(Vertex,TexCoords)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(offsetof(Vertex,Tangent)));

        glBindVertexArray(0);
    }
};
#endif //HELLOGL_MESH_H

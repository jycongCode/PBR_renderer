//
// Created by 从径源 on 2024/7/12.
//

#ifndef LEARNOPENGL_PLANE_H
#define LEARNOPENGL_PLANE_H
#include <Shader.h>
float planeVertices[] = {
        // positions            // normals         // texcoords
        25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

        25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
        25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
};

class Plane{
public:
    Plane(){
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindVertexArray(0);
    }

    void Draw(Shader shader,glm::mat4 projection,glm::mat4 view,GLuint texture){
        glBindVertexArray(planeVAO);
        shader.use();
        shader.setMat4("projection",projection);
        shader.setMat4("view",view);
        shader.setMat4("model",glm::mat4(1.0f));
        shader.setInt("texture0",0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);
        glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);
    }

    void Draw(Shader shader,GLuint texture){
        glBindVertexArray(planeVAO);
        shader.use();
        shader.setMat4("model",glm::mat4(1.0f));
        shader.setInt("texture0",0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);
        glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);
    }

    void Destroy(){
        glDeleteBuffers(1,&planeVBO);
        glDeleteVertexArrays(1,&planeVAO);
    }

private:
    GLuint planeVAO,planeVBO;
};
#endif //LEARNOPENGL_PLANE_H

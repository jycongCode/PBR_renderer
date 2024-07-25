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
    Quad(){
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
    }

    void Draw(Shader shader){
        glBindVertexArray(quadVAO);
        shader.use();
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        glBindVertexArray(0);
    }

    void Destroy(){
        glDeleteBuffers(1,&quadVBO);
        glDeleteVertexArrays(1,&quadVAO);
    }

private:
    GLuint quadVAO,quadVBO;
};

#endif //LEARNOPENGL_QUAD_H

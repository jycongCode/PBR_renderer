//
// Created by 从径源 on 2024/7/18.
//

#ifndef LEARNOPENGL_SPHERE_H
#define LEARNOPENGL_SPHERE_H
#include <Shader.h>
#include "Transform.h"
class Sphere{
public:
    Sphere(){
        glGenVertexArrays(1, &sphereVAO);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = static_cast<unsigned int>(indices.size());

        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }

        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    }

    void Draw(Shader shader,Camera camera,Window window,Transform transform){
        glEnable(GL_DEPTH_TEST);
        shader.use();
        shader.setVec3("viewPos",camera.Position);
        shader.setFloat("shininess",32);
        shader.setMat4("model",transform.GetModel());
        glm::mat4 view = camera.GetViewMatrix();
        float scrWidth = static_cast<float>(window.GetWindowWidth());
        float scrHeight = static_cast<float>(window.GetWindowHeight());
        float aspectRatio = scrWidth / scrHeight;
        glm::mat4 projection = glm::perspective(camera.Zoom,aspectRatio,camera.nearPlane,camera.farPlane);
        shader.setMat4("view",view);
        shader.setMat4("projection",projection);
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Draw(Shader shader,Camera camera,Window window,Transform transform,GLuint texture){
        shader.InitDirLight();
        shader.use();
        shader.setInt("tex",0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);
        Draw(shader,camera,window,transform);
    }

    void Destroy(){
        glDeleteVertexArrays(1,&sphereVAO);
        glDeleteBuffers(1,&vbo);
        glDeleteBuffers(1,&ebo);
        data.clear();
        indices.clear();
    }
private:
    unsigned int sphereVAO;
    unsigned int vbo,ebo;
    unsigned int indexCount;
    std::vector<float> data;
    std::vector<unsigned int> indices;
};
#endif //LEARNOPENGL_SPHERE_H

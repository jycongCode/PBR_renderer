//
// Created by 从径源 on 2024/7/22.
//

#ifndef LEARNOPENGL_TRANSFORM_H
#define LEARNOPENGL_TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
enum TransformComponent{
    Position,
    Rotation,
    Scale
};

struct Transform{
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation;
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 GetModel(){
        glm::mat4 model(1.0f);
        model = glm::scale(model,scale);
        model = toMat4(rotation) * model;
        return model;
    }

    void Rotate(glm::vec3 axis,float angle){
        glm::quat delta = glm::angleAxis(angle,axis);
        rotation = delta * rotation;
    }

    void Scale(float x,float y,float z){
        scale = glm::vec3(x,y,z);
    }
};
#endif //LEARNOPENGL_TRANSFORM_H

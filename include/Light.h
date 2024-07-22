//
// Created by 从径源 on 2024/7/22.
//

#ifndef LEARNOPENGL_LIGHT_H
#define LEARNOPENGL_LIGHT_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum LightType{
    Direction,
    Point,
    Spot
};

struct DirLight{
    glm::vec3 Direction;
    glm::vec3 Color;
};

struct PointLight{
    glm::vec3 Position;
    glm::vec3 Color;
};

struct SpotLight{
    glm::vec3 Position;
    glm::vec3 Direction;
    glm::vec3 Color;
};

#endif //LEARNOPENGL_LIGHT_H

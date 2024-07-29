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
    DirLight(float x,float y,float z,float r,float g,float b,float intensity){
        Direction = glm::vec3(x,y,z);
        Color = glm::vec3(r,g,b);
        Intensity = intensity;
    }
    glm::vec3 Direction;
    glm::vec3 Color;
    float Intensity;
};

struct Light
{
    glm::vec3 Position;
    glm::vec3 Direction;
    glm::vec3 Color;
    float Intensity;
};
struct PointLight{
    PointLight(float x,float y,float z,float r,float g,float b,float intensity){
        Position = glm::vec3(x,y,z);
        Color = glm::vec3(r,g,b);
        Intensity = intensity;
    }
    glm::vec3 Position;
    glm::vec3 Color;
    float Intensity;
};

struct SpotLight{
    glm::vec3 Position;
    glm::vec3 Direction;
    glm::vec3 Color;
};

#endif //LEARNOPENGL_LIGHT_H

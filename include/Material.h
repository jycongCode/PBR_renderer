//
// Created by 从径源 on 2024/7/24.
//

#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H
#include "Utils.h"

struct PBRMaterial{
    PBRMaterial(const char* name,bool gammaCorrection){
        std::string prefix = std::string(name);
        albedo = Utils::loadTexture2D((prefix + "/albedo.png").c_str(),gammaCorrection);
        metallic = Utils::loadTexture2D((prefix + "/metallic.png").c_str(), false);
        roughness = Utils::loadTexture2D((prefix + "/roughness.png").c_str(), false);
        normal = Utils::loadTexture2D((prefix + "/normal.png").c_str(), false);
        ao = 1.0f;
    }

    PBRMaterial(){}
    unsigned int albedo;
    unsigned int metallic;
    unsigned int roughness;
    unsigned int normal;

    float ao;
};

struct PBRpcMaterial{
    PBRpcMaterial(float metallic,float roughness,const char* normalPath)
    :metallic(metallic),roughness(roughness)
    {
        normal = Utils::loadTexture2D(normalPath, false);
        ao = 1.0f;
    }


    glm::vec3 albedo = glm::vec3(0.5f,0.5f,0.5f);
    float metallic;
    float roughness;
    unsigned int normal;
    float ao;
};

#endif //PBR_MATERIAL_H

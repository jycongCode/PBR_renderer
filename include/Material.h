//
// Created by 从径源 on 2024/7/24.
//

#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H
#include "Utils.h"

struct PBRMaterial{
    PBRMaterial(const char* name,bool gammaCorrection){
        std::string prefix = std::string(name);
        albedoTex = Utils::loadTexture2D((prefix + "/albedo.png").c_str(),gammaCorrection);
        metallicTex = Utils::loadTexture2D((prefix + "/metallic.png").c_str(),gammaCorrection);
        roughnessTex = Utils::loadTexture2D((prefix + "/roughness.png").c_str(),gammaCorrection);
        normalTex = Utils::loadTexture2D((prefix + "/normal.png").c_str(),gammaCorrection);
        ao = 1.0f;
    }
    unsigned int albedoTex;
    unsigned int metallicTex;
    unsigned int roughnessTex;
    unsigned int normalTex;
    float ao;
};

#endif //PBR_MATERIAL_H

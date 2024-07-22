#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
}fs_in;

struct PBRMaterial{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};
uniform PBRMaterial material;

struct Light{
    vec3 Position;
    vec3 Color;
};
uniform Light lights[4];

uniform vec3 viewPos;
const float PI = 3.14159265359;

float DistributionGGX(vec3 N,vec3 H,float roughness)
{
    float a = roughness * roughness;
    float a2 = a*a;
    float NdotH = max(dot(N,H),0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2-1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV,float roughness){
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0-k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N,vec3 V,vec3 L,float roughness){
    float NdotV = max(dot(N,V),0.0);
    float NdotL = max(dot(N,L),0.0);
    float ggx2 = GeometrySchlickGGX(NdotV,roughness);
    float ggx1 = GeometrySchlickGGX(NdotL,roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta,vec3 F0){
    return F0 + (1.0-F0) * pow(clamp(1.0-cosTheta,0.0,1.0),5.0);
}

void main(){
    vec3 N = normalize(fs_in.Normal);
    vec3 V = normalize(viewPos - fs_in.FragPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0,material.albedo,material.metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0;i<4;++i){
        // per-light radiance
        vec3 L = normalize(lights[i].Position - fs_in.FragPos);
        vec3 H = normalize(L + V);
        float distance = length(lights[i].Position - fs_in.FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lights[i].Color * attenuation;

        // cook-torrance brdf
        float NDF = DistributionGGX(N,H,material.roughness);
        float G = GeometrySmith(N,V,L,material.roughness);
        vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0-material.metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N,V),0.0) * max(dot(N,L),0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        float NdotL = max(dot(N,L),0.0);
        Lo += (kD * material.albedo / PI + specular) * radiance * NdotL;
    }
    vec3 ambient = vec3(0.03) * material.albedo * material.ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color,vec3(1.0/2.2));

    FragColor = vec4(color,1.0);
}

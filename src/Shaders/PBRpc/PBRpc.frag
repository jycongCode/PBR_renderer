#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    mat3 TBN;
}fs_in;

struct PBRMaterial{
    vec3 albedo;
    float metallic;
    float roughness;
    sampler2D normal;
    float ao;
};
uniform PBRMaterial material;

struct Light{
    vec3 Position;
    vec3 Direction;
    vec3 Color;
};
uniform Light light;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;
uniform bool hdr;
uniform float exposure;
uniform bool gammaCorrection;
uniform float gamma;
uniform vec3 viewPos;
const float PI = 3.14159265359;
uniform bool isEnvironmentLight;
uniform bool isDirectLight;

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

vec3 fresnelSchlickRoughness(float cosTheta,vec3 F0,float roughness){
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
void main(){
    vec3 N = normalize(fs_in.TBN * texture(material.normal,fs_in.TexCoord).xyz);
    //    N = fs_in.Normal;
    vec3 V = normalize(viewPos - fs_in.FragPos);

    float roughness = material.roughness;
    float metallic = material.metallic;
    vec3 albedo = material.albedo;

    //    roughness = 0.2;
    //    metallic = 0.6;
    //    albedo = vec3(0.8,0.0,0.0);

    float ao = material.ao;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0,albedo,metallic);

    vec3 Lo = vec3(0.0);
    vec3 L = normalize(light.Position-fs_in.FragPos);
    vec3 H = normalize(L + V);
    vec3 radiance = light.Color;
    // cook-torrance brdf
    float NDF = DistributionGGX(N,H,roughness);
    float G = GeometrySmith(N,V,L,roughness);
    vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0-metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N,V),0.0) * max(dot(N,L),0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    float NdotL = max(dot(N,L),0.0);
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient;

    float distance = length(L);
    if(isDirectLight){
        color += Lo * vec3(1.0 / (distance * distance));
    }

    vec3 F_IBL = fresnelSchlickRoughness(max(dot(N,V),0.0),F0,roughness);
    vec3 R = reflect(-V,N);
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilterColor = textureLod(prefilterMap,R,roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF = texture(brdfLUT,vec2(max(dot(N,V),0.0),roughness)).rg;
    vec3 specularIBL = prefilterColor * (F_IBL * envBRDF.x + envBRDF.y);
    vec3 kD_IBL = vec3(1.0) - F_IBL;
    kD_IBL *= 1.0-metallic;
    vec3 irradiance = texture(irradianceMap,N).rgb;
    vec3 iblDiffuse = irradiance * albedo;
    vec3 ambientIBL = (kD_IBL * iblDiffuse + specularIBL) * ao;

    if(isEnvironmentLight){
        color += ambientIBL;
    }
    if(hdr){
        color = vec3(1.0) - exp(-color * exposure);
    }
    if(gammaCorrection){
        color = pow(color,vec3(1.0/gamma));
    }
    FragColor = vec4(fs_in.TexCoord.x,fs_in.TexCoord.y,0.0,1.0);
    FragColor = vec4(color,1.0);
}
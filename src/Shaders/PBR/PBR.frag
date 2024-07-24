#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    mat3 TBN;
}fs_in;

struct PBRMaterial{
    sampler2D albedo;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D normal;
    float ao;
};
uniform PBRMaterial material;

struct DirLight{
    vec3 Direction;
    vec3 Color;
};
uniform DirLight light;

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
    vec3 N = normalize(fs_in.TBN * texture(material.normal,fs_in.TexCoord).xyz);
    vec3 V = normalize(viewPos - fs_in.FragPos);

    float roughness = texture(material.roughness,fs_in.TexCoord).r;
    float metallic = texture(material.metallic,fs_in.TexCoord).r;
    vec3 albedo = texture(material.albedo,fs_in.TexCoord).rgb;
    float ao = material.ao;
    vec3 F0 = vec3(0.04);
    F0 = mix(F0,albedo,metallic);

    vec3 Lo = vec3(0.0);
    vec3 L = -normalize(light.Direction);
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
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color,vec3(1.0/2.2));

//    FragColor = vec4(fs_in.TexCoord.x,fs_in.TexCoord.y,0.0,1.0);
    FragColor = vec4(color,1.0);
}
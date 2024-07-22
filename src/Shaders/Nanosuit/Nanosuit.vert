#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightDir;
uniform vec3 viewPos;

out VS_OUT{
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec2 TexCoords;
    vec3 Normal;
}vs_out;
void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0);
    vs_out.TexCoords = aTexCoords;
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T,B,N);
    vec3 fragPos = vec3(model * vec4(aPos,1.0));
    vs_out.TangentLightDir = normalize(TBN * lightDir);
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TangentFragPos = TBN * fragPos;
    vs_out.Normal = normalize(normalMatrix * aNormal);
}
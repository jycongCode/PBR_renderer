#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    mat3 TBN;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0);
    vec3 fragPos = vec3(model * vec4(aPos,1.0));
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    vec3 normal = normalize(normalMatrix * aNormal);
    vec3 tangent = normalize(normalMatrix * vec3(-aPos.z,0.0,aPos.x + 0.001));
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(normal,tangent);
    vs_out.TBN = mat3(tangent,bitangent,normal);
    vs_out.FragPos = fragPos;
    vs_out.Normal = normal;
    vs_out.TexCoord = aTexCoord;
}
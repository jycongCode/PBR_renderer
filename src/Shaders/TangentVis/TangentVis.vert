#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out VS_OUT{
    vec3 normal;
    vec3 tangent;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0);
    vec3 fragPos = vec3(model * vec4(aPos,1.0));
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    vec3 normal = normalize(normalMatrix * aNormal);
    vec3 tangent = normalize(normalMatrix * aTangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vs_out.normal = normal;
    vs_out.tangent = tangent;
}
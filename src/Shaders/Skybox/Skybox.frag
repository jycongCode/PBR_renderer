#version 330 core
out vec4 FragColor;
in vec3 TexCoords;
uniform samplerCube skybox;
void main(){
    vec3 color = textureLod(skybox,TexCoords,0).rgb;
    color = color / (color + vec3(1.0));
    color = pow(color,vec3(1.0/2.2));
    FragColor = vec4(color,1.0);
}
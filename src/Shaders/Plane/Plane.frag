#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture0;
void main(){
    FragColor = texture(texture0,TexCoords);
}
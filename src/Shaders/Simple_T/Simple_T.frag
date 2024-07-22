#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec2 TexCoord;

struct PointLight{

};

uniform sampler2D tex;
void main(){
    vec3 color = texture(tex,TexCoord).rgb;
    FragColor = vec4(color,1.0);
}
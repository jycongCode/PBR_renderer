#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texture0;

void main(){
    vec2 color = texture(texture0,TexCoord).rg;
    color = color / (color + vec2(1.0));
//    color = pow(color,vec2(1.0/2.2));
    FragColor = vec4(color,0.0,1.0);
}
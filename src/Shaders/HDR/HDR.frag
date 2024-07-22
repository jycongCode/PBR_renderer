#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture0;
//uniform sampler2D texture1;
uniform float exposure;
uniform bool hdr;
void main(){
    const float gamma = 2.2;
//    vec3 picColor = texture(texture0,TexCoords).rgb;
//    vec3 blurColor = texture(texture1,TexCoords).rgb;
//    vec3 hdrColor = picColor + blurColor;
    vec3 hdrColor = texture(texture0,TexCoords).rgb;
    if(hdr){
        vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
        mapped = pow(mapped,vec3(1.0/gamma));
        FragColor = vec4(mapped,1.0);
    }else{
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}
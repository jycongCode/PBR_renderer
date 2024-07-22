#version 330 core
out float FragColor;
in vec2 TexCoords;
uniform sampler2D texture0;
void main(){
    vec2 texOffset = 1.0 / textureSize(texture0,0);
    float result = 0.0;
    for(int i = -2;i<2;++i){
        for(int j = -2;j<2;++j){
            vec2 offset = vec2(float(i),float(j)) * texOffset;
            result += texture(texture0,TexCoords + offset).r;
        }
    }
    result /= 16.0;
    FragColor = result;
}
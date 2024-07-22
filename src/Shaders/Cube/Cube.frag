#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Light{
    vec3 Position;
    vec3 Color;
};
uniform Light lights[4];

uniform sampler2D texture0;

uniform vec3 viewPos;

void main(){
    vec3 color = texture(texture0, fs_in.TexCoords).rgb;
    vec3 normal = fs_in.Normal;
    // Ambient
    vec3 ambient = 0.0 * color;
    vec3 lighting = vec3(0.0);
    for(int i = 0;i<4;++i){
        // Diffuse
        vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * lights[i].Color * color;

        vec3 result = diffuse;
        float distance = length(fs_in.FragPos - lights[i].Position);
        result *= 1.0 / (distance * distance);
        // Specular
//        vec3 viewDir = normalize(viewPos - fs_in.FragPos);
//        vec3 reflectDir = reflect(-lightDir, normal);
//        float spec = 0.0;
//        vec3 halfwayDir = normalize(lightDir + viewDir);
//        spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
//        vec3 specular = spec * lightColor;
        lighting += result;
    }
    FragColor = vec4(lighting, 1.0f);
    float brightness = dot(lighting,vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0){
        BrightColor = vec4(lighting,1.0f);
    }else{
        BrightColor = vec4(0.0f,0.0f,0.0f,1.0f);
    }
}
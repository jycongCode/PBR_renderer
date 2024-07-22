#version 330 core
struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_reflect1;
};
uniform Material material;

in VS_OUT{
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec2 TexCoords;
    vec3 Normal;
}fs_in;

uniform int NormalMap_On;

out vec4 FragColor;
void main(){
    vec3 color = texture(material.texture_diffuse1,fs_in.TexCoords).rgb;
    vec3 normal;
    if(NormalMap_On == 1){
        normal = vec3(texture(material.texture_normal1,fs_in.TexCoords));
        normal = normal * 2.0-1.0;
        normal = normalize(normal);
    }else{
        normal = fs_in.Normal;
    }
    vec3 lightColor = vec3(0.8);
    // Ambient
    vec3 ambient = 0.1 * color;
    // Diffuse
    float diff = max(dot(fs_in.TangentLightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor * vec3(texture(material.texture_diffuse1,fs_in.TexCoords));
    // Specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-fs_in.TangentLightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(fs_in.TangentLightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * lightColor * vec3(texture(material.texture_specular1,fs_in.TexCoords));
    vec3 lighting = (ambient + diffuse + specular);
    FragColor = vec4(lighting,1.0);
}
#version 330 core
out vec4 FragColor;
in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
}fs_in;

struct DirLight{
    vec3 Direction;
    vec3 Color;
};

uniform DirLight light;
uniform sampler2D tex;
uniform vec3 viewPos;
uniform float shininess;

vec3 CalculateLight(vec3 albedo){
    // ambient
    vec3 ambient = vec3(0.1) * albedo;

    // diffuse
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = light.Direction;
    float diff = max(dot(-lightDir,normal),0.0);
    vec3 diffuse = diff * light.Color * albedo;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfway = normalize(viewDir - lightDir);
    float spec = pow(max(dot(halfway,normal),0.0),shininess);
    vec3 specular = spec * light.Color;
    vec3 result = specular + diffuse + ambient;
    return result;
}
void main(){
    vec3 albedo = texture(tex,fs_in.TexCoord).rgb;
    vec3 color = CalculateLight(albedo);
    FragColor = vec4(color,1.0);
}
#version 330 core
struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_reflect1;
    float shininess;
};
uniform Material material;

struct DirLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
// struct PointLight{
//     vec3 position;
//     float constant;
//     float linear;
//     float quadratic;
//
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };
// #define NR_POINT_LIGHTS 4
// uniform PointLight pointLights[NR_POINT_LIGHTS];
// vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);

out vec4 FragColor;
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
uniform vec3 viewPos;
uniform samplerCube skybox;
void main()
{
    // ambient
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 result = CalcDirLight(dirLight,norm,viewDir);
    FragColor = vec4(result,1.0);
    // diffuse
}

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,TexCoords));

    float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,TexCoords));

    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1,TexCoords));

//     return (ambient + diffuse + specular);
    vec3 viewReflectDir = normalize(reflect(-viewDir,normal));
    vec3 ref = vec3(texture(material.texture_reflect1,TexCoords));
    vec3 reflect = vec3(texture(skybox,viewReflectDir)) * ref;
//     return vec3(texture(skybox,viewReflectDir));
     return (ambient + diffuse + specular + reflect);
}

//
// vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir)
// {
//     vec3 lightDir = normalize(light.position-fragPos);
//     float distance = length(light.position-fragPos);
//     float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance * distance));
//
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
//
//     float diff = max(dot(lightDir,normal),0.0);
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
//
//     vec3 reflectDir = reflect(-lightDir,normal);
//     float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
//     vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));
//
//     return (ambient + diffuse + specular) * attenuation;
// }




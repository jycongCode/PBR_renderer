#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform sampler2D ssao;

struct Light{
    vec3 Position;
    vec3 Color;
    float Radius;
};
uniform Light lights[1];

uniform vec3 viewPos;
uniform float exposure;
uniform bool hdr;
//
void main(){
    vec3 albedo = texture(gColorSpec,TexCoords).rgb;
    vec3 ambient = albedo;
    vec3 result = ambient * texture(ssao,TexCoords).r * 0.3;
    vec3 normal = texture(gNormal,TexCoords).rgb;
    vec3 fragPos = texture(gPosition,TexCoords).rgb;
    for(int i = 0;i<1;++i){
        float distance = length(fragPos - lights[i].Position);

        vec3 lightDir = normalize(lights[i].Position - fragPos);
        //diffuse
        float diff = max(dot(lightDir,normal),0.0);
        vec3 diffuse = diff * lights[i].Color * albedo;

        // specular
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 halfway = normalize(viewDir + lightDir);
        float spec = max(dot(halfway,normal),0.0);
        vec3 specular = spec * texture(gColorSpec,TexCoords).z * lights[0].Color;

        vec3 lighting = diffuse + specular;
        lighting *= 1.0/(distance * distance);
        result += lighting;

    }
    const float gamma = 2.2;
    vec3 hdrColor = result;
    if(hdr){
        vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
        mapped = pow(mapped,vec3(1.0/gamma));
        FragColor = vec4(mapped,1.0);
    }else{
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}
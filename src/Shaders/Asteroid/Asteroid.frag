#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gColorSpec;

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_reflect1;
};
uniform Material material;
uniform bool spec;
in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
}fs_in;

void main()
{
    gPosition = vec4(fs_in.FragPos,1.0);
    gNormal = vec4(fs_in.Normal,0.0);
    float specular = 0.0;
    if(spec){
        specular = texture(material.texture_specular1,fs_in.TexCoord).r;
    }
    vec3 color = texture(material.texture_diffuse1,fs_in.TexCoord).rgb;
    gColorSpec = vec4(0.95,0.95,0.95,0.0);
}





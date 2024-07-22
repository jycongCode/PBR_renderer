#version 330 core
in vec2 TexCoords;
out float FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D ssaoNoise;

uniform vec3 samples[64];
uniform mat4 projection;
uniform float screenWidth;
uniform float screenHeight;

uniform int kernelSize;
uniform float radius;
void main(){
    vec2 noiseScale = vec2(screenWidth / 4.0,screenHeight / 4.0);
    vec3 fragPos = texture(gPosition,TexCoords).xyz;
    vec3 normal = texture(gNormal,TexCoords).rgb;
    vec3 randomVec = texture(ssaoNoise,TexCoords * noiseScale).xyz;

    vec3 tangent = normalize(randomVec - dot(randomVec,normal) * normal);
    vec3 bitangent = cross(tangent,normal);
    mat3 TBN = mat3(tangent,bitangent,normal);

    float occlusion = 0.0;
    float bias = 0.025;
    for(int i = 0;i<kernelSize;++i){
        vec3 samplePos = TBN * samples[i];
        samplePos = fragPos + samplePos * radius;
        vec4 offset = vec4(samplePos,1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;
        float sampleDepth = texture(gPosition,offset.xy).z;
        float rangeCheck = smoothstep(0.0,1.0,radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = occlusion;
}
#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 2) uniform sampler2D baseColorMap;
layout(set = 1, binding = 3) uniform sampler2D metallicRoughnessMap;
layout(set = 1, binding = 4) uniform sampler2D emissiveMap;
layout(set = 1, binding = 5) uniform sampler2D normalMap;
layout(set = 1, binding = 6) uniform sampler2D occlusionMap;

layout(set = 1, binding = 7) uniform Material {
    vec4 baseColor;
    vec4 emissiveFactor;
    float metallicFactor;
    float roughnessFactor;
    float normalFactor;
    float occlusionFactor;

    bool hasBaseColorMap;
    bool hasMetallicRoughnessMap;
    bool hasEmissiveMap;
    bool hasNormalMap;
    bool hasOcclusionMap;

    float PADDING[3];
} material;

void main() {
    vec3 lightDir = vec3(1.1, -1.0, 1.0);
    float attenuation = clamp(dot(fragNormal, -normalize(lightDir)), 0.2, 1.0);
    
    vec3 baseColor = material.baseColor.rgb;
//    if (material.hasBaseColorMap) {
//        baseColor *= texture(baseColorMap, fragTexCoord).rgb;
//    }

    outColor = vec4(baseColor * attenuation, 1.0);
}
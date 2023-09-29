#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec4 fragPosition;
layout(location = 4) in vec4 fragTangent;

layout(location = 0) out vec4 outColor;

layout(binding = 2) uniform sampler2D baseColorMap;
layout(binding = 3) uniform sampler2D metallicRoughnessMap;
layout(binding = 4) uniform sampler2D emissiveMap;
layout(binding = 5) uniform sampler2D normalMap;
layout(binding = 6) uniform sampler2D occlusionMap;

layout(binding = 7) uniform Material {
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

    float PADDING0;
    float PADDING1;
    float PADDING2;
} material;

struct PointLight {
    vec4 position;
    vec4 color;
    float intensity;

    float PADDING0;
    float PADDING1;
    float PADDING2;
};

struct DirectionalLight {
    vec4 direction;
    vec4 color;
    float intensity;

    float PADDING0;
    float PADDING1;
    float PADDING2;
};

layout(binding = 8) uniform Lighting {
    PointLight pointLights[16];
    DirectionalLight directionalLight;
    vec4 eyePosition;

    int pointLightCount;

    float PADDING0;
    float PADDING1;
    float PADDING2;
} lighting;

#define PI 3.14159265359

float saturate(float x) {
    return clamp(x, 0.0, 1.0);
}

float pow5(float x)
{
    float x2 = x * x;
    return x2 * x2 * x;
}

vec3 F_Schlick(vec3 f0, float f90, float VdotH)
{
    return f0 + (f90 - f0) * pow5(clamp(1.0 - VdotH, 0.0, 1.0));
}

vec3 BRDF_lambertian(vec3 f0, float f90, const float VdotH)
{
    return (vec3(1.0) - F_Schlick(f0, f90, VdotH)) * (1.0 / PI);
}

vec3 Fd_Burley(float roughness, float NoV, float NoL, float LoH)
{
    float f90 = 0.5f + 2.0f * roughness * LoH * LoH;
    vec3 lightScatter = F_Schlick(vec3(1.0), f90, NoL);
    vec3 viewScatter = F_Schlick(vec3(1.0), f90, NoV);
    return lightScatter * viewScatter * (1.0 / PI);
}

float V_GGX(float NdotL, float NdotV, float alphaRoughness)
{
    float alphaRoughnessSq = alphaRoughness * alphaRoughness;

    float GGXV = NdotL * sqrt(NdotV * NdotV * (1.0 - alphaRoughnessSq) + alphaRoughnessSq);
    float GGXL = NdotV * sqrt(NdotL * NdotL * (1.0 - alphaRoughnessSq) + alphaRoughnessSq);

    return saturate(0.5f / (GGXV + GGXL));
}

float D_GGX(float NdotH, float alphaRoughness)
{
    float alphaRoughnessSq = alphaRoughness * alphaRoughness;

    float f = (NdotH * NdotH) * (alphaRoughnessSq - 1.0) + 1.0;
    return alphaRoughnessSq / (PI * f * f);
}

vec3 BRDF_specularGGX(vec3 f0, float f90, float alphaRoughness, float VdotH, float NdotL, float NdotV, float NdotH)
{
    float D = D_GGX(NdotH, alphaRoughness);
    float V = V_GGX(NdotL, NdotV, alphaRoughness);
    vec3 F = F_Schlick(f0, f90, VdotH);

    return F * V * D;
}

vec3 ShadePBR(vec3 P, vec3 V, vec3 N, vec3 L, vec3 H, vec3 f0, float NoL, vec3 diffuseColor, float metallic, float roughness) {
    float NoV = saturate(dot(N, V));
    NoL = saturate(NoL);
    float NoH = saturate(dot(N, H));
    float LoH = saturate(dot(L, H));
    float VoH = saturate(dot(V, H));

    float f90 = saturate(dot(f0, vec3(50.0 * 0.33)));

    vec3 Fr = BRDF_specularGGX(f0, f90, roughness, VoH, NoL, NoV, NoH);
    vec3 Fd = diffuseColor * Fd_Burley(roughness, NoV, NoL, LoH);
    vec3 color = Fd + Fr;

    return color * NoL;
}

vec3 Shade(vec3 P, vec3 V, vec3 N, vec3 f0, vec3 diffuseColor, float metallic, float roughness) {
    vec3 Lo = vec3(0.0);

    for (uint i = 0; i < lighting.pointLightCount; i++) {
        vec3 L = normalize(lighting.pointLights[i].position.xyz - P);
        float NoL = dot(N, L);
        if (NoL <= 0.0) continue;

        vec3 H = normalize(V + L);
        float D = distance(lighting.pointLights[i].position.xyz, P);
        float attenuation = lighting.pointLights[i].intensity / (D * D * 50.0);
        vec3 radiance = lighting.pointLights[i].color.rgb;

        Lo += ShadePBR(P, V, N, L, H, f0, NoL, diffuseColor, metallic, roughness) * radiance * attenuation;
    }

    {
        vec3 L = normalize(-lighting.directionalLight.direction.xyz);
        float NoL = dot(N, L);
        if (NoL > 0.0) {
            vec3 H = normalize(V + L);
            float attenuation = lighting.directionalLight.intensity;
            vec3 radiance = lighting.directionalLight.color.rgb;

            Lo += ShadePBR(P, V, N, L, H, f0, NoL, diffuseColor, metallic, roughness) * radiance * attenuation;
        }
    }

    return Lo;
}

void main() { 
    vec3 baseColor = material.baseColor.rgb;
    if (material.hasBaseColorMap) {
        baseColor *= texture(baseColorMap, fragTexCoord).rgb;
    }

    vec3 P = fragPosition.xyz;
    vec3 V = normalize(lighting.eyePosition.xyz - P);
    vec3 N = normalize(fragNormal);
    if (material.hasNormalMap) {
        vec3 normalSample = texture(normalMap, fragTexCoord).rgb;
        normalSample = normalize(normalSample * 2.0 - 1.0);

        vec3 tangent = normalize(fragTangent.xyz);
        tangent = normalize(tangent - N * dot(N, tangent));
        vec3 B = normalize(cross(tangent, N) + 0.0001) * fragTangent.w;

        mat3 TBN = mat3(tangent, B, N);
        N = normalize(TBN * normalSample);
    }

    vec3 emission = material.emissiveFactor.rgb;
    if (material.hasEmissiveMap) {
        emission *= texture(emissiveMap, fragTexCoord).rgb;
    }

    float occlusion = 1.0;
    if (material.hasOcclusionMap) {
        occlusion = mix(1.0, texture(occlusionMap, fragTexCoord).r, material.occlusionFactor);
    }

    float metallic = material.metallicFactor;
    float roughness = material.roughnessFactor;
    if (material.hasMetallicRoughnessMap) {
        vec2 metallicRoughness = texture(metallicRoughnessMap, fragTexCoord).gb;
        metallic *= metallicRoughness.y;
        roughness *= metallicRoughness.x;
    }

    vec3 f0 = mix(vec3(0.04), baseColor, metallic);
    vec3 diffuseColor = baseColor.rgb * (1.0 - metallic);
    vec3 shading = Shade(P, V, N, f0, diffuseColor, metallic, roughness);

    outColor = vec4(shading * occlusion + emission, 1.0);
}
#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 lightDir = vec3(1.1, -1.0, 1.0);
    float attenuation = clamp(dot(fragNormal, -normalize(lightDir)), 0.2, 1.0);
    outColor = vec4(fragColor * attenuation, 1.0);
}
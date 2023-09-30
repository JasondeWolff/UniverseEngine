#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in vec3 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform samplerCube skyboxMap;

void main() {
    outColor = vec4(texture(skyboxMap, fragTexCoord).rgb, 1.0);
}
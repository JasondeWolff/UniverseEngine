#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D skyboxMap;

void main() {
    outColor = vec4(texture(skyboxMap, fragTexCoord).rgb, 1.0);
}
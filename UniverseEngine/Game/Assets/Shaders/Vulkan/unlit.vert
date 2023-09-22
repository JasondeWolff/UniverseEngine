#version 450
#extension GL_KHR_vulkan_glsl: enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;
layout (location = 3) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 mvp;
} ubo;

void main() {
    gl_Position = ubo.mvp * vec4(inPosition, 1.0);
    fragColor = inColor;
}
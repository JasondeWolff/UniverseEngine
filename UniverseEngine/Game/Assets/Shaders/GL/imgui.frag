#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout (binding = 0) uniform sampler2D tex;

void main() {
    outColor = fragColor * texture(tex, fragTexCoord);
}
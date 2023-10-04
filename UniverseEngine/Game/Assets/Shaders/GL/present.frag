#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_KHR_vulkan_glsl : enable

layout (location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D colorImage;

void main() {
    vec3 color = texture(colorImage, fragTexCoord).rgb;
    outColor = vec4(color, 1.0);
}
#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in struct {
    vec4 Color;
    vec2 UV;
} In;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform sampler2D tex;

void main() {
    outColor = In.Color * texture(tex, In.UV.st);
}
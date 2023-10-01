#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in struct {
    vec4 color;
    vec2 texCoord;
} In;

layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform sampler2D tex;

void main() {
    outColor = In.color * texture(tex, In.texCoord);
}
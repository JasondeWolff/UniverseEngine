#version 450
#extension GL_KHR_vulkan_glsl: enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec4 inTangent;
layout (location = 4) in vec4 inColor;

layout(location = 0) out struct {
    vec4 color;
    vec2 texCoord;
} Out;

layout(push_constant) uniform PushConstant {
    mat4 proj;
} pc;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    Out.color = inColor;
    Out.texCoord = inTexCoord;
    gl_Position = pc.proj * vec4(inPosition.xy, 0.0, 1.0);
}
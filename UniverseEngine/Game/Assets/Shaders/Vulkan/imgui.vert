#version 450
#extension GL_KHR_vulkan_glsl: enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec4 inTangent;
layout (location = 4) in vec4 inColor;

layout(location = 0) out struct {
    vec4 Color;
    vec2 UV;
} Out;

layout(push_constant) uniform PushConstant {
    vec2 uScale;
    vec2 uTranslate;
} pc;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    Out.Color = inColor;
    Out.UV = inTexCoord;
    gl_Position = vec4(inPosition.xy * pc.uScale + pc.uTranslate, 0.0, 1.0);
}
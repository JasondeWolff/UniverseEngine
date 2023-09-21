#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;
layout (location = 3) in vec3 inColor;

out vec3 Normal;
out vec2 TexCoords;
out vec3 Color;

layout(push_constant) uniform PushConstants {
    mat4 mvp;
};

void main()
{
    Normal = inNormal;
    TexCoords = inTexCoords;
    Color = inColor;
    gl_Position = mvp * vec4(inPosition, 1.0);
}
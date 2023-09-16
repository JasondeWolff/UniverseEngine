#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aColor;

out vec3 Normal;
out vec2 TexCoords;
out vec3 Color;

layout(push_constant) uniform PushConstants {
    mat4 mvp;
};

void main()
{
    Normal = aNormal;
    TexCoords = aTexCoords;
    Color = aColor;
    gl_Position = mvp * vec4(aPos, 1.0);
}
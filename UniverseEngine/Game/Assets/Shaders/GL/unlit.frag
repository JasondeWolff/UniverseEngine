#version 450 core

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D baseColorMap;

void main() {
    vec3 lightDir = vec3(1.1, -1.0, 1.0);
    float attenuation = clamp(dot(fragNormal, -normalize(lightDir)), 0.2, 1.0);
    
    vec3 color = texture(baseColorMap, fragTexCoord).rgb * attenuation;

    outColor = vec4(color, 1.0);
}
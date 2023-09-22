#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 Color;

void main()
{   
    vec3 lightDir = vec3(1.1, -1.0, 1.0);
    float attenuation = clamp(dot(Normal, -normalize(lightDir)), 0.2, 1.0);
    FragColor = vec4(1.0, 1.0, 1.0, 1.0) * attenuation;
}
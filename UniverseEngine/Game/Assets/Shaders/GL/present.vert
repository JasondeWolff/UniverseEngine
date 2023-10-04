#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_KHR_vulkan_glsl : enable

layout (location = 0) out vec2 fragTexCoord;

void main() {
	vec2 vertices[3]=vec2[3](vec2(-1,-1), vec2(3,-1), vec2(-1, 3));
	gl_Position = vec4(vertices[gl_VertexID],0,1);
	fragTexCoord = 0.5 * gl_Position.xy + vec2(0.5);
}
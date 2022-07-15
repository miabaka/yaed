#version 330 core

void main() {
	gl_Position = vec4(vec2(ivec2(gl_VertexID, (gl_VertexID + 1) & 2) / 2), 0.f, 1.f);
}
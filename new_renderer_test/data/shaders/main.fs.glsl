#version 330 core

flat in uint vMaterial;
in vec2 vTexCoords;

out vec4 outColor;

void main() {
	outColor = vec4(vTexCoords, 0.f, 1.f);
}
#version 330 core

layout (location = 0) in ivec4 aTile;

uniform mat4 uProjection;
uniform float uTileScaleTable[] = { 1.f, 0.5f, 1.6f };

flat out int vMaterial;
out vec2 vTexCoords;

void main() {
	vec2 vertexPosition = vec2(ivec2(gl_VertexID, (gl_VertexID + 1) & 2) / 2);

	vTexCoords = vertexPosition;
	vMaterial = aTile.z;

	vertexPosition = (vertexPosition - 0.5f) * uTileScaleTable[aTile.w] + 0.5f;
	vertexPosition += vec2(aTile.xy);

	gl_Position = uProjection * vec4(vertexPosition, 0.f, 1.f);
}
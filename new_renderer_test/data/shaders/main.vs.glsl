#version 330 core

layout (location = 0) in uvec3 aTile;

uniform mat4 uProjection;

flat out uint vMaterial;
out vec2 vTexCoords;

void main() {
	vec2 vertexPosition = vec2(ivec2(gl_VertexID, (gl_VertexID + 1) & 2) / 2);

	vTexCoords = vertexPosition;
	vMaterial = aTile.z >> 1;

	vertexPosition = (vertexPosition - 0.5f) * (bool(aTile.z & 1u) ? 1.6f : 1.f)+ 0.5f;
	vertexPosition += vec2(aTile.xy);

	gl_Position = uProjection * vec4(vertexPosition, 0.f, 1.f);
}
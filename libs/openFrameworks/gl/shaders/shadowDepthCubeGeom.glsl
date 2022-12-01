static const std::string depthCubeGeometryShaderSource = R"(
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

struct Light {
	mat4 viewProjectionMatrix;
};
uniform Light light[6];
uniform int uStartLayer = 0;

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main() {
	int i = 0;
	for(int face = 0; face < 6; ++face) {
//	for(gl_Layer=0; gl_Layer<6; gl_Layer++) {
		gl_Layer = face + uStartLayer * 6;
//		gl_Layer = face; // built-in variable that specifies to which face we render.
		for(i = 0; i < 3; i++) {// for each triangle's vertices
			FragPos = gl_in[i].gl_Position;
			gl_Position = light[face].viewProjectionMatrix * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}
)";

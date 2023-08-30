#version 330

out vec4 v_position;
out vec4 v_color;
uniform mat4 modelViewProjectionMatrix;
uniform int numVertices;
in vec4  position;
const float PI = 3.141592;

float radicalInverse_VdC(uint bits) {
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
vec2 hammersley(uint i, uint N) {
    return vec2(float(i)/float(N), radicalInverse_VdC(i));
}

vec3 hemisphereSample_uniform(uint i, uint N){
     vec2 E = hammersley(i,N);
     float u = E.x;
     float v = E.y;
     float phi = v * 2.0 * PI;
     float cosTheta = 1.0 - u;
     float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
     return vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

void main(){
	vec3 pos = hemisphereSample_uniform(uint(gl_VertexID), uint(numVertices));
	v_position = vec4(pos * -600, 1.0);
	v_color = vec4(abs(pos.x), abs(pos.y), abs(pos.z), 1.0);
}

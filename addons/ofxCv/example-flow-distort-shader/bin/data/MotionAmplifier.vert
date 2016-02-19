#version 120

uniform sampler2DRect source;
uniform sampler2DRect flow;
uniform float scaleFactor;
uniform float strength;
uniform float sourceRescale;
uniform float flowRescale;
varying vec2 texCoord;

void main() {
    vec2 baseCoord = gl_Vertex.xy;
    texCoord = baseCoord * sourceRescale;
    vec2 offset = texture2DRect(flow, baseCoord * flowRescale).xy;
    vec4 position = gl_Vertex;
    offset = (offset - vec2(.5, .5)) / scaleFactor;
    position.x += strength * offset.x;
    position.y += strength * offset.y;
    position.z += length(offset);
    gl_Position = gl_ModelViewProjectionMatrix * position;
}
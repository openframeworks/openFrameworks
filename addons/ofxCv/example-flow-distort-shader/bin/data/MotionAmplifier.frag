#version 120

uniform sampler2DRect source;
varying vec2 texCoord;

void main() {
    gl_FragColor = texture2DRect(source, texCoord);
}
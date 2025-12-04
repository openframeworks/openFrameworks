OF_GLSL_SHADER_HEADER

// this is how we receive the texture
uniform sampler2D tex0;
uniform vec2 resolution;

varying vec2 texCoordVarying;

void main()
{
    gl_FragColor = texture2D(tex0, texCoordVarying/resolution);
}

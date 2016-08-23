static const string vertexShader = R"(
OUT vec2 v_texcoord; // pass the texCoord if needed
OUT vec3 v_transformedNormal;
OUT vec3 v_normal;
OUT vec3 v_eyePosition;
#if HAS_COLOR
OUT vec4 v_color;
#endif

IN vec4 position;
IN vec4 color;
IN vec4 normal;
IN vec2 texcoord;

// these are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;


void main (void){
    vec4 eyePosition = modelViewMatrix * position;
    vec3 tempNormal = (normalMatrix * normal).xyz;
    v_transformedNormal = normalize(tempNormal);
    v_normal = normal.xyz;
    v_eyePosition = (eyePosition.xyz) / eyePosition.w;

    v_texcoord = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
    #if HAS_COLOR
        v_color = color;
    #endif
    gl_Position = modelViewProjectionMatrix * position;
}
)";

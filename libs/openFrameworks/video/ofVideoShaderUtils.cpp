#include "ofConstants.h"
#include "ofShader.h"

#define stringify(x) string("#version 440\n")+#x

static string VERTEX_SHADER = stringify(
	uniform mat4 projectionMatrix;
	uniform mat4 modelViewMatrix;
	uniform mat4 textureMatrix;
	uniform mat4 modelViewProjectionMatrix;

	in vec4  position;
	in vec2  texcoord;
	in vec4  color;
	in vec3  normal;

	out vec4 colorVarying;
	out vec2 texCoordVarying;
	out vec4 normalVarying;

	void main()
	{
		colorVarying = color;
		texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
		gl_Position = modelViewProjectionMatrix * position;
	}
);

static string FRAGMENT_SHADER_YUY2 = stringify(
	uniform sampler2DRect src_tex_unit0;
	uniform vec4 globalColor;

	in vec4 colorVarying;
	in vec2 texCoordVarying;

	out vec4 fragColor;) + ""

    "const vec3 offset = vec3(-0.0625, -0.5, -0.5);\n"
    "const vec3 rcoeff = vec3(1.164, 0.000, 1.596);\n"
    "const vec3 gcoeff = vec3(1.164,-0.391,-0.813);\n"
    "const vec3 bcoeff = vec3(1.164, 2.018, 0.000);\n"


	"void main(){\n"
    "    float r,g,b;\n"
    "    vec3 yuv;\n"
	"    int texX = int(texCoordVarying.x);\n"
	"    yuv.x=texture(src_tex_unit0,texCoordVarying).r;\n"
	"    if(texX%2==1){\n"
    "        yuv.y=texture(src_tex_unit0,vec2(texCoordVarying.x-1.0,texCoordVarying.y)).g;\n"
    "        yuv.z=texture(src_tex_unit0,texCoordVarying).g;\n"
	"    }else{\n"
	"        yuv.y=texture(src_tex_unit0,texCoordVarying).g;\n"
	"        yuv.z=texture(src_tex_unit0,vec2(texCoordVarying.x+1.0,texCoordVarying.y)).g;\n"
	"    }\n"
    "    yuv += offset;\n"
    "    r = dot(yuv, rcoeff);\n"
    "    g = dot(yuv, gcoeff);\n"
    "    b = dot(yuv, bcoeff);\n"
    "    fragColor=vec4(r,g,b,1.0) * globalColor;\n"
	"}\n";

static string FRAGMENT_SHADER_NV12_NV21 = stringify(
	uniform sampler2DRect Ytex;
	uniform sampler2DRect UVtex;
	uniform vec4 globalColor;

	in vec4 colorVarying;
	in vec2 texCoordVarying;

	out vec4 fragColor;) + ""

    "const vec3 offset = vec3(-0.0625, -0.5, -0.5);\n"
    "const vec3 rcoeff = vec3(1.164, 0.000, 1.596);\n"
    "const vec3 gcoeff = vec3(1.164,-0.391,-0.813);\n"
    "const vec3 bcoeff = vec3(1.164, 2.018, 0.000);\n"


	"void main(){\n"
    "    float r,g,b;\n"
    "    vec3 yuv;\n"
	"    yuv.x=texture(Ytex,texCoordVarying).r;\n"
	"    yuv.yz=texture(UVtex,texCoordVarying * vec2(0.5,0.5)).%r%g;\n"
    "    yuv += offset;\n"
    "    r = dot(yuv, rcoeff);\n"
    "    g = dot(yuv, gcoeff);\n"
    "    b = dot(yuv, bcoeff);\n"
    "    fragColor=vec4(r,g,b,1.0) * globalColor;\n"
	"}\n";

static string FRAGMENT_SHADER_PLANAR_YUV = stringify(
	uniform sampler2DRect Ytex;
	uniform sampler2DRect Utex;
	uniform sampler2DRect Vtex;
    uniform vec2 tex_scaleY;
    uniform vec2 tex_scaleU;
    uniform vec2 tex_scaleV;
	uniform vec4 globalColor;

	in vec4 colorVarying;
	in vec2 texCoordVarying;

	out vec4 fragColor;) + ""

    "const vec3 offset = vec3(-0.0625, -0.5, -0.5);\n"
    "const vec3 rcoeff = vec3(1.164, 0.000, 1.596);\n"
    "const vec3 gcoeff = vec3(1.164,-0.391,-0.813);\n"
    "const vec3 bcoeff = vec3(1.164, 2.018, 0.000);\n"


	"void main(){\n"
    "    float r,g,b;\n"
    "    vec3 yuv;\n"
	"    yuv.x=texture(Ytex,texCoordVarying * tex_scaleY).r;\n"
	"    yuv.y=texture(Utex,texCoordVarying * tex_scaleU).r;\n"
	"    yuv.z=texture(Vtex,texCoordVarying * tex_scaleV).r;\n"
    "    yuv += offset;\n"
    "    r = dot(yuv, rcoeff);\n"
    "    g = dot(yuv, gcoeff);\n"
    "    b = dot(yuv, bcoeff);\n"
    "    fragColor=vec4(r,g,b,1.0) * globalColor;\n"
	"}\n";

static ofShader & getShaderPlanarYUY2(){
	static ofShader * shader = new ofShader;
	return *shader;
}

static ofShader & getShaderNV12(){
	static ofShader * shader = new ofShader;
	return *shader;
}

static ofShader & getShaderNV21(){
	static ofShader * shader = new ofShader;
	return *shader;
}

static ofShader & getShaderPlanarYUV(){
	static ofShader * shader = new ofShader;
	return *shader;
}

static string getVertexShaderSource(){
	return VERTEX_SHADER;
}

static string getFragmentShaderSource(ofPixelFormat pixelFormat){
	string src;
	switch(pixelFormat){
		case OF_PIXELS_YUY2:
			src = FRAGMENT_SHADER_YUY2;
			break;
		case OF_PIXELS_NV12:
			src = FRAGMENT_SHADER_NV12_NV21;
			ofStringReplace(src,"%r%g","rg");
			break;
		case OF_PIXELS_NV21:
			src = FRAGMENT_SHADER_NV12_NV21;
			ofStringReplace(src,"%r%g","gr");
			break;
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
			src = FRAGMENT_SHADER_PLANAR_YUV;
			break;
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
		case OF_PIXELS_RGB565:
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
		case OF_PIXELS_GRAY:
		default:
			break;
	}
	return src;
}

ofShader * ofGetVideoShader(ofPixelFormat pixelFormat){
	ofShader * shader = NULL;
	switch(pixelFormat){
		case OF_PIXELS_YUY2:
			shader = &getShaderPlanarYUY2();
			break;
		case OF_PIXELS_NV12:
			shader = &getShaderNV12();
			break;
		case OF_PIXELS_NV21:
			shader = &getShaderNV21();
			break;
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
			shader = &getShaderPlanarYUV();
			break;
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
		case OF_PIXELS_RGB565:
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
		case OF_PIXELS_GRAY:
		default:
			break;
	}
	if(shader && !shader->isLoaded()){
		shader->setupShaderFromSource(GL_VERTEX_SHADER,getVertexShaderSource());
		shader->setupShaderFromSource(GL_FRAGMENT_SHADER,getFragmentShaderSource(pixelFormat));
		shader->bindDefaults();
		shader->linkProgram();
	}
	return shader;
}

void ofSetVideoShaderUniforms(ofBaseVideoDraws & video, ofShader & shader){
	switch(video.getPixelFormat()){
		case OF_PIXELS_NV12:
		case OF_PIXELS_NV21:
			shader.setUniformTexture("Ytex",video.getTextureReference(0),0);
			shader.setUniformTexture("UVtex",video.getTextureReference(1),1);
			shader.setUniform2f("tex_scaleY",video.getTextureScale(0).x,video.getTextureScale(0).y);
			shader.setUniform2f("tex_scaleUV",video.getTextureScale(1).x,video.getTextureScale(1).y);
			break;
		case OF_PIXELS_YV12:
			shader.setUniformTexture("Ytex",video.getTextureReference(0),0);
			shader.setUniformTexture("Utex",video.getTextureReference(2),1);
			shader.setUniformTexture("Vtex",video.getTextureReference(1),2);
			shader.setUniform2f("tex_scaleY",video.getTextureScale(0).x,video.getTextureScale(0).y);
			shader.setUniform2f("tex_scaleU",video.getTextureScale(2).x,video.getTextureScale(2).y);
			shader.setUniform2f("tex_scaleV",video.getTextureScale(1).x,video.getTextureScale(1).y);
			break;
		case OF_PIXELS_I420:
			shader.setUniformTexture("Ytex",video.getTextureReference(0),0);
			shader.setUniformTexture("Utex",video.getTextureReference(1),1);
			shader.setUniformTexture("Vtex",video.getTextureReference(2),2);
			shader.setUniform2f("tex_scaleY",video.getTextureScale(0).x,video.getTextureScale(0).y);
			shader.setUniform2f("tex_scaleU",video.getTextureScale(1).x,video.getTextureScale(1).y);
			shader.setUniform2f("tex_scaleV",video.getTextureScale(2).x,video.getTextureScale(2).y);
			break;
		default:
			break;
	}
}

#include "ofVideoWriter.h"
#include "ofGraphics.h"


/*
 Tirar mais ideias daqui:
 http://codefromabove.com/2015/01/av-foundation-saving-a-sequence-of-raw-rgb-frames-to-a-movie/
 fazer shader pra corrigir a cor
 testar melhores color depth
 kCVPixelFormatType_32BGRA
 
 */

using std::cout;
using std::endl;
//---------------------------------------------------------------------------
ofVideoWriter::ofVideoWriter() {
	
	std::string vert;
	std::string frag;
	if (ofIsGLProgrammableRenderer()) {
		vert = R""""(
#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec2 texcoord;
out vec2 texCoord;

void main() {
	texCoord = texcoord;
	gl_Position = modelViewProjectionMatrix * position;
}

)"""";
		
		frag = R""""(
#version 150 

uniform sampler2DRect tex;     // fbo texture to draw to
in vec2 texCoord;
out vec4 outputColor;

void main (void) { 	
  outputColor = texture(tex, texCoord).bgra;
} 


)"""";
	} else {
		vert = R""""(
varying vec2 texcoord0;
 
void main() {
	gl_Position = ftransform();
	texcoord0 = vec2(gl_TextureMatrix[0] * gl_MultiTexCoord0);
}
)"""";
		
		frag = R""""(
uniform sampler2DRect tex0;
varying vec2 texcoord0;
 
void main (void) {
	gl_FragColor = texture2DRect(tex0, texcoord0).bgra;
}
)"""";
	}
	
	shader.setupShaderFromSource( GL_VERTEX_SHADER, vert );
	shader.setupShaderFromSource( GL_FRAGMENT_SHADER, frag );
	shader.bindDefaults();
	shader.linkProgram();
}

//---------------------------------------------------------------------------
ofVideoWriter::ofVideoWriter(const of::filesystem::path & _fileName) {
	// FIXME: Begin or setup?
	begin(_fileName);
}

//---------------------------------------------------------------------------
void ofVideoWriter::setFbo(ofFbo * _f) {
	cout << "ofVideoWriter::setFbo " << endl;
	_fbo = _f;
	
	fboShader.allocate(_fbo->getWidth(), _fbo->getHeight(), GL_RGBA16F);
	fboShader.begin();
	ofClear(0, 255);
	fboShader.end();
}
//---------------------------------------------------------------------------
void ofVideoWriter::begin(const of::filesystem::path & _fileName) {
	cout << "ofVideoWriter::begin" << endl;

	fileName = ofToDataPath(_fileName);
	
	videoWriter = [[ofAVFoundationVideoWriter alloc] init];
	
	glm::ivec2 dimensions { _fbo->getWidth(), _fbo->getHeight() };
	videoWriter.dimensions = dimensions;
	[videoWriter initPath:[NSString stringWithUTF8String:fileName.c_str()]];
	
	isBegin = true;
}

//---------------------------------------------------------------------------
void ofVideoWriter::addFrame() {
	if (isBegin && _fbo != nullptr) {
		if (videoWriter != nullptr) {
//			videoWriter._fbo = _fbo;
			videoWriter._fbo = &fboShader;
			fboShader.begin();
			ofClear(0, 0);
			shader.begin();
			_fbo->draw(0, 0);
			shader.end();
			fboShader.end();
			[videoWriter addFrame];
		}
	}
}

//---------------------------------------------------------------------------
void ofVideoWriter::end() {
	cout << "ofVideoWriter::end" << endl;
	isBegin = false;

	{
		[videoWriter stopRecording];
	}
}

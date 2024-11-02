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
	videoWriter = [[ofAVFoundationVideoWriter alloc] init];
	
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

void ofVideoWriter::setFbo(ofFbo * _f) {
	_fbo = _f;
}

void ofVideoWriter::setFps(int f) {
	fps = f;
}

void ofVideoWriter::setOutputFilename(const of::filesystem::path & _fileName) {
	fileName = ofToDataPath(_fileName);
	useCustomName = true;
}

void ofVideoWriter::begin() {
	if (_fbo == nullptr) {
		return;
	}
	
	videoWriter.fps = fps;

	fboShader.allocate(_fbo->getWidth(), _fbo->getHeight(), GL_RGBA16F);
	fboShader.begin();
	ofClear(0, 255);
	fboShader.end();
	
	if (!useCustomName) {
		std::string uniqueName {
			ofPathToString(ofFilePath::getCurrentExePath().filename()) +
			"_" +
			ofToString(ofGetYear()) +
			ofToString(ofGetMonth(), 2, '0') +
			ofToString(ofGetDay(), 2, '0') + "-" +
			ofToString(ofGetHours(), 2, '0') +
			ofToString(ofGetMinutes(), 2, '0') +
			ofToString(ofGetSeconds(), 2, '0') + ".mov"
		};
		
		fileName = uniqueName;
		fileName = ofToDataPath(fileName);
	}
	
//	cout << "ofVideoWriter::begin " << fileName<< endl;
	
	videoWriter._fbo = &fboShader;
	[videoWriter initPath:[NSString stringWithUTF8String:fileName.c_str()]];
	isRecording = true;
}

//---------------------------------------------------------------------------
void ofVideoWriter::addFrame() {
	if (isRecording) {
		fboShader.begin();
		ofClear(0, 0);
		shader.begin();
		_fbo->draw(0, 0);
		shader.end();
		fboShader.end();
		[videoWriter addFrame];
	}
}

//---------------------------------------------------------------------------
void ofVideoWriter::end() {
	if (isRecording) {
		[videoWriter stopRecording];
	} else {
		cout << "ofVideoWriter::end() : not recording " << endl;
	}
	isRecording = false;
	
	if (autoOpen) {
		std::string command { "open " + ofPathToString(fileName)  };
//		cout << "autoOpen command = " << command << endl;
//		cout << "ofVideoWriter::end " << fileName << endl;
		ofSystem(command);
	}
}


void ofVideoWriter::toggleRecording() {
	if (isRecording) {
		end();
	} else {
		begin();
	}
//	isRecording ^= 1;
}

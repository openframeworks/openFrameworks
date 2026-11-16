#include "ofApp.h"

#ifdef TARGET_WIN32
	#include <windows.h>
#endif

static const int side = 20; // side^3 instanced boxes

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(false);
	ofSetFrameRate(0);
	ofBackground(20);

	auto glString = [](GLenum name) {
		const GLubyte * s = glGetString(name);
		return s ? std::string(reinterpret_cast<const char *>(s)) : std::string("(none)");
	};
	renderer = glString(GL_RENDERER);
	version = glString(GL_VERSION);

	// GLon12 reports its renderer as "D3D12 (<adapter>)", WARP is the Microsoft Basic Render Driver
	if (renderer.rfind("D3D12", 0) != 0) {
		mode = "system OpenGL driver (GLon12 not in use)";
	} else if (renderer.find("Microsoft Basic Render Driver") != std::string::npos) {
		mode = "GLon12 on WARP (software D3D12)";
	} else {
		mode = "GLon12 on the GPU (D3D12)";
	}
	ofLogNotice("glon12Example") << "GL_RENDERER: " << renderer;
	ofLogNotice("glon12Example") << "GL_VERSION: " << version;
	ofLogNotice("glon12Example") << mode;

	box = ofMesh::box(1, 1, 1, 1, 1, 1);
	shader.setupShaderFromSource(GL_VERTEX_SHADER, R"(#version 330
		uniform mat4 modelViewProjectionMatrix;
		uniform int side;
		uniform float time;
		in vec4 position;
		in vec3 normal;
		out vec3 color;
		void main() {
			int i = gl_InstanceID;
			vec3 cell = vec3(i % side, (i / side) % side, i / (side * side));
			vec3 p = position.xyz * (0.5 + 0.3 * sin(time + cell.x * 0.4 + cell.y * 0.3))
				+ (cell - float(side) * 0.5) * 1.5;
			color = mix(cell / float(side), vec3(0.5) + 0.5 * normal, 0.4);
			gl_Position = modelViewProjectionMatrix * vec4(p, 1.0);
		})");
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, R"(#version 330
		in vec3 color;
		out vec4 fragColor;
		void main() {
			fragColor = vec4(color, 1.0);
		})");
	shader.bindDefaults();
	shader.linkProgram();

	cam.setDistance(60);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofEnableDepthTest();
	cam.begin();
	shader.begin();
	shader.setUniform1i("side", side);
	shader.setUniform1f("time", ofGetElapsedTimef());
	box.drawInstanced(OF_MESH_FILL, side * side * side);
	shader.end();
	cam.end();
	ofDisableDepthTest();

	std::string info = mode + "\n"
		+ "GL_RENDERER: " + renderer + "\n"
		+ "GL_VERSION:  " + version + "\n"
		+ ofToString(side * side * side) + " instanced boxes, " + ofToString(ofGetFrameRate(), 0) + " fps\n";
#ifdef TARGET_WIN32
	if (warpRequested && mode.find("WARP") == std::string::npos) {
		info += "WARP was requested but isn't active: LIBGL_ALWAYS_SOFTWARE is set, or GLon12 isn't in use\n";
	}
	info += "\nw: restart on " + std::string(warpRequested ? "the GPU" : "WARP");
#endif
	ofDrawBitmapStringHighlight(info, 20, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'w') {
		restart(!warpRequested);
	}
}

//--------------------------------------------------------------
// The adapter is chosen once per process, so switching means starting the app again
void ofApp::restart(bool warp) {
#ifdef TARGET_WIN32
	wchar_t exe[MAX_PATH];
	GetModuleFileNameW(nullptr, exe, MAX_PATH);
	std::wstring cmd = L"\"" + std::wstring(exe) + L"\"" + (warp ? L" --warp" : L"");
	STARTUPINFOW si = { sizeof(si) };
	PROCESS_INFORMATION pi = {};
	if (CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		ofExit();
	} else {
		ofLogError("glon12Example") << "couldn't restart: " << GetLastError();
	}
#endif
}

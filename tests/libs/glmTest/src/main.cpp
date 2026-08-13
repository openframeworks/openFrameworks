#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ofApp: public ofxUnitTestsApp{
	void run(){
		glm::mat4 identity(1.0f);
		glm::vec4 v(1.0f, 2.0f, 3.0f, 1.0f);
		glm::vec4 transformed = identity * v;
		ofxTestEq(transformed.x, v.x, "identity matrix leaves x unchanged");
		ofxTestEq(transformed.y, v.y, "identity matrix leaves y unchanged");
		ofxTestEq(transformed.z, v.z, "identity matrix leaves z unchanged");

		glm::mat4 translated = glm::translate(identity, glm::vec3(10.0f, 0.0f, 0.0f));
		glm::vec4 moved = translated * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		ofxTestEq(moved.x, 10.0f, "translate() moves x by 10");

		glm::vec3 a(1.0f, 0.0f, 0.0f);
		glm::vec3 b(0.0f, 1.0f, 0.0f);
		ofxTestEq(glm::dot(a, b), 0.0f, "perpendicular unit vectors dot to 0");
		glm::vec3 crossed = glm::cross(a, b);
		ofxTestEq(crossed.z, 1.0f, "cross(x,y) == z");
	}
};

int main(){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}

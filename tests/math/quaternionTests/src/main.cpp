#include "ofMain.h"
#include "ofxUnitTests.h"
#include "ofAppNoWindow.h"

bool aprox_eq(const glm::vec3 & v1, const glm::vec3 & v2){
    bool eq = abs(v1.x - v2.x) < 0.0001 &&
           abs(v1.y - v2.y) < 0.0001 &&
           abs(v1.z - v2.z) < 0.0001;
    if(!eq){
        ofLogError() << "value1: " << v1;
        ofLogError() << "value2: " << v2;
    }
    return eq;
}

class ofApp: public ofxUnitTestsApp{
public:
    void run(){
        glm::quat q1 = glm::angleAxis(glm::radians(30.0f),glm::normalize(glm::vec3(1,0,0)));
        glm::quat q2 = glm::angleAxis(glm::radians(30.0f),glm::normalize(glm::vec3(0,0,1)));
        glm::quat q3 = glm::angleAxis(glm::radians(30.0f),glm::normalize(glm::vec3(0,1,0)));
        glm::vec3 v(100,100,100);
        auto q12 = q1 * q2;
        auto q13 = q1 * q3;
        auto q23 = q2 * q3;
        auto q21 = q2 * q1;
        auto q31 = q3 * q1;
        auto q32 = q3 * q2;
        ofLogNotice() << "testing #4461";
        ofxTest(aprox_eq(v*q12,(v*q1)*q2),"v*q12 == (v*q1)*q2");
        ofxTest(aprox_eq(v*q13,(v*q1)*q3),"v*q13 == (v*q1)*q3");
        ofxTest(aprox_eq(v*q23,(v*q2)*q3),"v*q23 == (v*q2)*q3");
        ofxTest(aprox_eq(v*q21,(v*q2)*q1),"v*q21 == (v*q2)*q1");
        ofxTest(aprox_eq(v*q31,(v*q3)*q1),"v*q31 == (v*q3)*q1");
        ofxTest(aprox_eq(v*q32,(v*q3)*q2),"v*q32 == (v*q3)*q2");
        ofLogNotice() << "end testing #4461";
    }
};

//========================================================================
int main( ){
    ofInit();
    auto window = std::make_shared<ofAppNoWindow>();
    auto app = std::make_shared<ofApp>();
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(window, app);
    return ofRunMainLoop();


}


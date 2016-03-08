#include "ofMain.h"
#include "ofxUnitTests.h"
#include "ofAppNoWindow.h"

bool aprox_eq(const ofVec3f & v1, const ofVec3f & v2){
    bool eq = fabs(v1.x - v2.x) < 0.001 &&
              fabs(v1.y - v2.y) < 0.001 &&
              fabs(v1.z - v2.z) < 0.001;
    if(!eq){
        ofLogError() << "value1: " << v1;
        ofLogError() << "value2: " << v2;
    }
    return eq;
}

bool aprox_eq(const ofQuaternion & v1, const ofQuaternion & v2) {
	bool eq = fabs(v1.x() - v2.x()) < 0.001 &&
		      fabs(v1.y() - v2.y()) < 0.001 &&
		      fabs(v1.z() - v2.z()) < 0.001 &&
		      fabs(v1.w() - v2.w()) < 0.001;
	if (!eq) {
		ofLogError() << "value1: " << v1;
		ofLogError() << "value2: " << v2;
	}
	return eq;
}
class ofApp: public ofxUnitTestsApp{
public:
    void run(){
		{
			ofLogNotice() << "start orbit test";
			ofNode n1;
			n1.orbit(45, 45, 100);
			auto pos = n1.getGlobalPosition();
			auto orient = n1.getGlobalOrientation();

			test(aprox_eq(pos, { 50.f, -70.7107f, 50.f }), "\tposition");
			test(aprox_eq(orient, { 0.353553f, 0.353553f, -0.146447f, 0.853553f }), "\torientation");
			ofLogNotice() << "end orbit test";
		}

		{
			ofLogNotice() << "parent pos/orient start";
			
			ofNode n1;
			ofNode n2;
			n2.setParent(n1);
			n1.orbit(90, 0, 100);
			n2.truck(-100.f);
			n2.orbit(90, 0, 100, n1);

			auto pos1 = n1.getGlobalPosition();
			auto orient1 = n1.getGlobalOrientation();
			auto orient2 = n2.getGlobalOrientation();
			
			test(aprox_eq(pos1, { 100.f, 0.f, 0.f }), "\tposition1");
			test(aprox_eq(orient1, { 0.f, 0.707107f, 0.f, 0.707107f }), "\torientation1");
			test(aprox_eq(orient2, { 0.f, 1.f, 0.f, 0.f }), "\torientation2");
			
			ofLogNotice() << "parent pos/orient end";

		}

		{
			ofLogNotice() << "rotateAround vs orbit start";
			ofNode mNode;
			ofNode mNode1;
			ofNode mNode2;

			mNode1 = mNode2;
			float angle = 47;

			mNode2.orbit(angle, 0, 100, mNode);

			mNode1.dolly(100);
			mNode1.rotateAround(ofQuaternion(angle, { 0.f,1.f,0.f }), mNode.getGlobalPosition());

			auto pos1 = mNode1.getGlobalPosition();
			auto pos2 = mNode2.getGlobalPosition();

			test(aprox_eq(pos1, { 73.1354f, 0.f, 68.1998f }), "\tglobal position");
			test(aprox_eq(pos1, pos2), "\tposition equality");

			ofLogNotice() << "rotateAround vs orbit end";

		}

		{
			ofLogNotice() << "parent simple pos start";
			ofNode n1;
			ofNode n2;
			n1.setGlobalPosition({ 100.f,0.f,0.f });
			n1.setGlobalOrientation(ofQuaternion(-90, { 0.f,-1.f,1.f }));
			n2.setParent(n1);
			n2.truck(50.f);
			auto pos = n2.getGlobalPosition();

			test(aprox_eq(pos, { 100.f, -35.3553f, -35.3553f }), "\tposition");
			ofLogNotice() << "parent simple pos end";

		}

    }
};

//========================================================================
int main( ){
    ofInit();
    auto window = make_shared<ofAppNoWindow>();
    auto app = make_shared<ofApp>();
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(window, app);
    return ofRunMainLoop();


}


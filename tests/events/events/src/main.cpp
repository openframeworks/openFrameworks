#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

namespace {
	int lastIntFromCFunc = 0;
	int lastIntFromCFuncWithToken = 0;
	int selfUnregisterValue = 0;
	bool toggleVoidFunc = false;
	ofEvent<const int> selfUnregisterEvent;

	void intFunctListener(const int & i){
		lastIntFromCFunc = i;
	}

	void intFunctListenerWithToken(const int & i){
		lastIntFromCFuncWithToken = i;
	}

	void selfUnregister(const int & i){
		selfUnregisterValue = i;
		ofRemoveListener(selfUnregisterEvent, selfUnregister);
	}

	void voidFunc(){
		toggleVoidFunc = !toggleVoidFunc;
	}
}

class ofApp: public ofxUnitTestsApp{
	int lastInt = 0;
	int lastIntFromLambda = 0;
	int lastIntWithToken = 0;
	bool toggleForVoidListener = false;
	bool toggleForVoidLambdaListener = false;
	bool toggleForVoidListenerWithToken = false;

	void intListener(const int & i){
		lastInt = i;
	}

	void intListenerWithToken(const int & i){
		lastIntWithToken = i;
	}

	void voidListener(){
		toggleForVoidListener = !toggleForVoidListener;
	}

	void voidListenerWithToken(){
		toggleForVoidListenerWithToken = !toggleForVoidListenerWithToken;
	}

	void run(){
		{
			ofEvent<const int> intEvent;
			ofAddListener(intEvent, this, &ofApp::intListener);
			ofAddListener(intEvent, intFunctListener);
			ofEventListener listenerLambda(intEvent.newListener([&](const int & i){
				lastIntFromLambda = i;
			}, 0));
			ofEventListener listenerMember(intEvent.newListener(this, &ofApp::intListenerWithToken));
			ofEventListener listenerFunc(intEvent.newListener(intFunctListenerWithToken));

			ofNotifyEvent(intEvent, 5);
			ofxTestEq(lastInt, 5, "Testing int event to member function");
			ofxTestEq(lastIntWithToken, 5, "Testing int event to member function with release token");
			ofxTestEq(lastIntFromLambda, 5, "Testing int event to lambda function");
			ofxTestEq(lastIntFromCFunc, 5, "Testing int event to c function");
			ofxTestEq(lastIntFromCFuncWithToken, 5, "Testing int event to c function with release token");

			intEvent.disable();
			ofNotifyEvent(intEvent, 6);
			ofxTestEq(lastInt, 5, "Testing disabled int event to member function");
			ofxTestEq(lastIntWithToken, 5, "Testing disabled int event to member function with release token");
			ofxTestEq(lastIntFromLambda, 5, "Testing disabled int event to lambda function");
			ofxTestEq(lastIntFromCFunc, 5, "Testing disabled int event to c function");
			ofxTestEq(lastIntFromCFuncWithToken, 5, "Testing disabled int event to c function with release token");

			intEvent.enable();
			ofNotifyEvent(intEvent, 6);
			ofxTestEq(lastInt, 6, "Testing re-enabled int event to member function");
			ofxTestEq(lastIntWithToken, 6, "Testing re-enabled int event to member function with release token");
			ofxTestEq(lastIntFromLambda, 6, "Testing re-enabled int event to lambda function");
			ofxTestEq(lastIntFromCFunc, 6, "Testing re-enabled int event to c function");
			ofxTestEq(lastIntFromCFuncWithToken, 6, "Testing re-enabled int event to c function with release token");

			ofRemoveListener(intEvent, this, &ofApp::intListener);
			ofRemoveListener(intEvent, &intFunctListener);
			listenerLambda.unsubscribe();
			listenerMember.unsubscribe();
			listenerFunc.unsubscribe();
			ofNotifyEvent(intEvent, 5);
			ofxTestEq(lastInt, 6, "Testing remove listener on int event to member function");
			ofxTestEq(lastIntWithToken, 6, "Testing remove listener on int event to member function with release token");
			ofxTestEq(lastIntFromLambda, 6, "Testing remove listener on int event to lambda function");
			ofxTestEq(lastIntFromCFunc, 6, "Testing remove listener on int event to c function");
			ofxTestEq(lastIntFromCFuncWithToken, 6, "Testing remove listener on int event to c function with release token");
		}

		{
			ofEventListener listener;
			{
				ofEvent<const int> intEvent;
				listener = intEvent.newListener([&](const int & i){
					lastIntFromLambda = i;
				}, 0);
			}
			listener.unsubscribe();
		}

		{
			lastIntFromCFunc = 0;
			auto listener = selfUnregisterEvent.newListener(selfUnregister);
			selfUnregisterEvent.notify(5);
			ofxTestEq(selfUnregisterValue, 5, "Testing remove listener on event callback, first call");
			selfUnregisterEvent.notify(6);
			ofxTestEq(selfUnregisterValue, 5, "Testing remove listener on event callback, second call");
		}

		{
			ofEvent<void> voidEvent;
			ofAddListener(voidEvent, this, &ofApp::voidListener);
			ofEventListener listenerVoidLambda(voidEvent.newListener([&]{
				toggleForVoidLambdaListener = !toggleForVoidLambdaListener;
			}));
			ofEventListener listenerVoidMember(voidEvent.newListener(this, &ofApp::voidListenerWithToken));
			ofEventListener listenerVoidFunc(voidEvent.newListener(voidFunc));

			voidEvent.notify();
			ofxTestEq(toggleForVoidListener, true, "Void event with member function");
			ofxTestEq(toggleForVoidLambdaListener, true, "Void event with lambda function");
			ofxTestEq(toggleForVoidListenerWithToken, true, "Void event with member function with release token");
			ofxTestEq(toggleVoidFunc, true, "Void event with c function");

			voidEvent.disable();
			voidEvent.notify();
			ofxTestEq(toggleForVoidListener, true, "Disabled void event with member function");
			ofxTestEq(toggleForVoidLambdaListener, true, "Disabled void event with lambda function");
			ofxTestEq(toggleForVoidListenerWithToken, true, "Disabled void event with member function with release token");
			ofxTestEq(toggleVoidFunc, true, "Disabled void event with c function");

			voidEvent.enable();
			voidEvent.notify();
			ofxTestEq(toggleForVoidListener, false, "Re-enabled void event with member function");
			ofxTestEq(toggleForVoidLambdaListener, false, "Re-enabled void event with lambda function");
			ofxTestEq(toggleForVoidListenerWithToken, false, "Re-enabled void event with member function with release token");
			ofxTestEq(toggleVoidFunc, false, "Re-enabled void event with c function");

			ofRemoveListener(voidEvent, this, &ofApp::voidListener);
			listenerVoidLambda.unsubscribe();
			listenerVoidMember.unsubscribe();
			listenerVoidFunc.unsubscribe();
			ofxTestEq(toggleForVoidListener, false, "Unregistered void event with member function");
			ofxTestEq(toggleForVoidLambdaListener, false, "Unregistered void event with lambda function");
			ofxTestEq(toggleForVoidListenerWithToken, false, "Unregistered void event with member function with release token");
			ofxTestEq(toggleVoidFunc, false, "Unregistered void event with c function");
		}


		{
			ofEvent<const int> e;
			auto listener = e.newListener([](const int & v){
			});
			auto listenerSender = e.newListener([](const void * sender, const int & v){
			});
			auto listenerBool = e.newListener([](const int & v){
				return false;
			});
			auto listenerSenderBool = e.newListener([](const void * sender, const int & v){
				return false;
			});
			e.notify(5);

			ofEvent<void> voidE;
			auto voidListener = voidE.newListener([]{

			});
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

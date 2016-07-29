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
			auto listenerLambda = intEvent.newListener([&](const int & i){
				lastIntFromLambda = i;
			}, 0);
			auto listenerMember = intEvent.newListener(this, &ofApp::intListenerWithToken);
			auto listenerFunc = intEvent.newListener(intFunctListenerWithToken);

			ofNotifyEvent(intEvent, 5);
			test_eq(lastInt, 5, "Testing int event to member function");
			test_eq(lastIntWithToken, 5, "Testing int event to member function with release token");
			test_eq(lastIntFromLambda, 5, "Testing int event to lambda function");
			test_eq(lastIntFromCFunc, 5, "Testing int event to c function");
			test_eq(lastIntFromCFuncWithToken, 5, "Testing int event to c function with release token");

			intEvent.disable();
			ofNotifyEvent(intEvent, 6);
			test_eq(lastInt, 5, "Testing disabled int event to member function");
			test_eq(lastIntWithToken, 5, "Testing disabled int event to member function with release token");
			test_eq(lastIntFromLambda, 5, "Testing disabled int event to lambda function");
			test_eq(lastIntFromCFunc, 5, "Testing disabled int event to c function");
			test_eq(lastIntFromCFuncWithToken, 5, "Testing disabled int event to c function with release token");

			intEvent.enable();
			ofNotifyEvent(intEvent, 6);
			test_eq(lastInt, 6, "Testing re-enabled int event to member function");
			test_eq(lastIntWithToken, 6, "Testing re-enabled int event to member function with release token");
			test_eq(lastIntFromLambda, 6, "Testing re-enabled int event to lambda function");
			test_eq(lastIntFromCFunc, 6, "Testing re-enabled int event to c function");
			test_eq(lastIntFromCFuncWithToken, 6, "Testing re-enabled int event to c function with release token");

			ofRemoveListener(intEvent, this, &ofApp::intListener);
			ofRemoveListener(intEvent, &intFunctListener);
			listenerLambda.unsubscribe();
			listenerMember.unsubscribe();
			listenerFunc.unsubscribe();
			ofNotifyEvent(intEvent, 5);
			test_eq(lastInt, 6, "Testing remove listener on int event to member function");
			test_eq(lastIntWithToken, 6, "Testing remove listener on int event to member function with release token");
			test_eq(lastIntFromLambda, 6, "Testing remove listener on int event to lambda function");
			test_eq(lastIntFromCFunc, 6, "Testing remove listener on int event to c function");
			test_eq(lastIntFromCFuncWithToken, 6, "Testing remove listener on int event to c function with release token");
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
			test_eq(selfUnregisterValue, 5, "Testing remove listener on event callback, first call");
			selfUnregisterEvent.notify(6);
			test_eq(selfUnregisterValue, 5, "Testing remove listener on event callback, second call");
		}

		{
			ofEvent<void> voidEvent;
			ofAddListener(voidEvent, this, &ofApp::voidListener);
			auto listenerVoidLambda = voidEvent.newListener([&]{
				toggleForVoidLambdaListener = !toggleForVoidLambdaListener;
			});
			auto listenerVoidMember = voidEvent.newListener(this, &ofApp::voidListenerWithToken);
			auto listenerVoidFunc = voidEvent.newListener(voidFunc);

			voidEvent.notify();
			test_eq(toggleForVoidListener, true, "Void event with member function");
			test_eq(toggleForVoidLambdaListener, true, "Void event with lambda function");
			test_eq(toggleForVoidListenerWithToken, true, "Void event with member function with release token");
			test_eq(toggleVoidFunc, true, "Void event with c function");

			voidEvent.disable();
			voidEvent.notify();
			test_eq(toggleForVoidListener, true, "Disabled void event with member function");
			test_eq(toggleForVoidLambdaListener, true, "Disabled void event with lambda function");
			test_eq(toggleForVoidListenerWithToken, true, "Disabled void event with member function with release token");
			test_eq(toggleVoidFunc, true, "Disabled void event with c function");

			voidEvent.enable();
			voidEvent.notify();
			test_eq(toggleForVoidListener, false, "Re-enabled void event with member function");
			test_eq(toggleForVoidLambdaListener, false, "Re-enabled void event with lambda function");
			test_eq(toggleForVoidListenerWithToken, false, "Re-enabled void event with member function with release token");
			test_eq(toggleVoidFunc, false, "Re-enabled void event with c function");

			ofRemoveListener(voidEvent, this, &ofApp::voidListener);
			listenerVoidLambda.unsubscribe();
			listenerVoidMember.unsubscribe();
			listenerVoidFunc.unsubscribe();
			test_eq(toggleForVoidListener, false, "Unregistered void event with member function");
			test_eq(toggleForVoidLambdaListener, false, "Unregistered void event with lambda function");
			test_eq(toggleForVoidListenerWithToken, false, "Unregistered void event with member function with release token");
			test_eq(toggleVoidFunc, false, "Unregistered void event with c function");
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

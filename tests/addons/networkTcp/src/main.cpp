#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include "ofxNetwork.h"

class ofApp: public ofxUnitTestsApp{
public:
	void testNonBlocking(){
		int port = ofRandom(15000, 65535);

		ofxTCPServer server;
		test(server.setup(port,false), "non blocking server");

		ofxTCPClient client;
		test(client.setup("127.0.0.1",port,false), "non blocking client");

		bool received = false;
		std::string messageSent = "message";
		std::string messageReceived;

		test(client.send(messageSent), "send non blocking from client");
		for(int i=0;i<10 && !received;i++){
			ofSleepMillis(200);
			messageReceived = server.receive(0);
			if(messageReceived == messageSent){
				received = true;
			}
		}
		test(received, "receive non blocking from server");

		test(server.send(0,messageSent), "send non blocking from server");
		messageReceived = "";
		for(int i=0;i<10 && !received;i++){
			ofSleepMillis(200);
			messageReceived = client.receive();
			if(messageReceived == messageSent){
				received = true;
			}
		}
		test(received, "receive non blocking from client");
	}

	void testBlocking(){
		int port = ofRandom(15000, 65535);

		ofxTCPServer server;
		test(server.setup(port,true), "blocking server");

		ofxTCPClient client;
		test(client.setup("127.0.0.1",port,true), "blocking client");

		std::string messageSent = "message";
		test(client.send(messageSent), "send blocking from client");
		test_eq(server.receive(0), messageSent, "receive blocking from server");
		test(server.send(0, messageSent), "send blocking from server");
		test_eq(client.receive(), messageSent, "receive blocking from client");
	}

	void disconnectionAutoDetection(){
		int port = ofRandom(15000, 65535);

		ofxTCPServer server;
		test(server.setup(port,true), "blocking server");

		ofxTCPClient client;
		test(client.setup("127.0.0.1",port,true), "blocking client");

		test(server.isConnected(), "server is still connected");
		test(client.isConnected(), "client is still connected");

		test(server.disconnectClient(0), "server closes correctly");
		test(!client.isConnected(), "client detects disconnection");

		test(client.setup("127.0.0.1",port,true), "client reconnects");

		test(client.close(), "client disconnects");
		test(!server.isClientConnected(0), "server detects disconnection");
		test(!server.isClientConnected(1), "server detects disconnection");
	}

	void run(){
		ofSeedRandom(ofGetSeconds());
		testNonBlocking();
		testBlocking();
		disconnectionAutoDetection();
	}
};

//========================================================================
int main( ){
	auto window = make_shared<ofAppNoWindow>();
	auto app = make_shared<ofApp>();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(window, app);
	ofRunMainLoop();
}


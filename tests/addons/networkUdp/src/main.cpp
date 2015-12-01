#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include "ofxNetwork.h"

class ofApp: public ofxUnitTestsApp{
	void testNonBlocking(){
		int port = ofRandom(15000, 65535);
		ofxUDPManager server;
		test(server.Create(),"create udp socket");
		test(server.SetNonBlocking(true), "set non-blocking");
		test(server.Bind(port), "bind udp socket");
		test(server.Connect("127.0.0.1", port+1), "set ip and port to send for udp socket");

		std::string messageSent="message";
		std::vector<char> bufferReceive(messageSent.size(),0);
		std::string messageReceived;

		ofxUDPManager client;
		test(client.Create(), "create udp socket");
		test(client.SetNonBlocking(true), "set udp socket non blocking");
		test(client.Bind(port+1), "bind udp socket");
		test(client.Connect("127.0.0.1", port), "set ip and port to send for udp socket");


		test(client.Send(messageSent.c_str(), messageSent.size()), "client send non blocking");

		auto received = 0;
		auto left = messageSent.size();
		for(int i=0;i<10 && received<messageSent.size();i++){
			auto ret = server.Receive(bufferReceive.data() + received, left);
			test(ret>=0, "server receiving non block");
			if(ret>0){
				received += ret;
				left -= ret;
			}
			ofSleepMillis(10);
		}

		messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
		test_eq(messageSent, messageReceived, "client messageSent == server messageReceived");


		test(server.Send(messageSent.c_str(), messageSent.size()), "server send non blocking");

		received = 0;
		left = messageSent.size();
		bufferReceive.assign(messageSent.size(),0);
		for(int i=0;i<10 && received<messageSent.size();i++){
			auto ret = client.Receive(bufferReceive.data() + received, left);
			test(ret>=0, "client receiving non block");
			if(ret>0){
				received += ret;
				left -= ret;
			}
			ofSleepMillis(10);
		}

		messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
		test_eq(messageSent, messageReceived, "server messageSent == client messageReceived");
	}

	void testBlocking(){
		int port = ofRandom(15000, 65535);
		ofxUDPManager server;
		test(server.Create(),"create udp socket");
		test(server.SetNonBlocking(false), "set blocking");
		test(server.Bind(port), "bind udp socket");
		test(server.Connect("127.0.0.1", port+1), "set ip and port to send for udp socket");

		std::string messageSent="message";
		std::vector<char> bufferReceive(messageSent.size(),0);
		std::string messageReceived;

		ofxUDPManager client;
		test(client.Create(), "create udp socket");
		test(client.SetNonBlocking(false), "set udp socket blocking");
		test(client.Bind(port+1), "bind udp socket");
		test(client.Connect("127.0.0.1", port), "set ip and port to send for udp socket");


		test(client.Send(messageSent.c_str(), messageSent.size()), "client send blocking");
		auto ret = server.Receive(bufferReceive.data(), messageSent.size());
		test(ret>=0, "server receiving block");
		messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
		test_eq(messageSent, messageReceived, "client messageSent == server messageReceived");


		test(server.Send(messageSent.c_str(), messageSent.size()), "server send blocking");
		bufferReceive.assign(messageSent.size(),0);
		ret = client.Receive(bufferReceive.data(), messageSent.size());
		test(ret>=0, "client receiving block");
		messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
		test_eq(messageSent, messageReceived, "server messageSent == client messageReceived");
	}

	void testTimeOutRecv(){
		int port = ofRandom(15000, 65535);
		ofxUDPManager server;
		test(server.Create(),"create udp socket");
		test(server.SetNonBlocking(false), "set blocking");
		test(server.Bind(port), "bind udp socket");
		server.SetTimeoutReceive(5);

		char c;
		auto then = ofGetElapsedTimeMillis();
		auto ret = server.Receive(&c,1);
		auto now = ofGetElapsedTimeMillis();
		test_eq(ret,SOCKET_TIMEOUT,"socket receive timeout");
		test(now-then>=5000, "socket receive timeouts after 5s");
	}

	void run(){
		testNonBlocking();
		testBlocking();
		testTimeOutRecv();
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

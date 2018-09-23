#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include "ofxNetwork.h"

class ofApp: public ofxUnitTestsApp{
	void testNonBlocking(){
		int port = ofRandom(15000, 65535);
		ofxUDPManager server;
		ofxTest(server.Create(),"create udp socket");
		ofxTest(server.SetNonBlocking(true), "set non-blocking");
		ofxTest(server.Bind(port), "bind udp socket");
		ofxTest(server.Connect("127.0.0.1", port+1), "set ip and port to send for udp socket");

		std::string messageSent="message";
		std::vector<char> bufferReceive(messageSent.size(),0);
		std::string messageReceived;

		ofxUDPManager client;
		ofxTest(client.Create(), "create udp socket");
		ofxTest(client.SetNonBlocking(true), "set udp socket non blocking");
		ofxTest(client.Bind(port+1), "bind udp socket");
		ofxTest(client.Connect("127.0.0.1", port), "set ip and port to send for udp socket");


		ofxTest(client.Send(messageSent.c_str(), messageSent.size()), "client send non blocking");

		auto received = 0;
		auto left = messageSent.size();
		for(int i=0;i<10 && received<messageSent.size();i++){
			auto ret = server.Receive(bufferReceive.data() + received, left);
			ofxTest(ret>=0, "server receiving non block");
			if(ret>0){
				received += ret;
				left -= ret;
			}
			ofSleepMillis(10);
		}

		messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
		ofxTestEq(messageSent, messageReceived, "client messageSent == server messageReceived");


		ofxTest(server.Send(messageSent.c_str(), messageSent.size()), "server send non blocking");

		received = 0;
		left = messageSent.size();
		bufferReceive.assign(messageSent.size(),0);
		for(int i=0;i<10 && received<messageSent.size();i++){
			auto ret = client.Receive(bufferReceive.data() + received, left);
			ofxTest(ret>=0, "client receiving non block");
			if(ret>0){
				received += ret;
				left -= ret;
			}
			ofSleepMillis(10);
		}

		messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
		ofxTestEq(messageSent, messageReceived, "server messageSent == client messageReceived");
	}

	void testBlocking(){
		int port = ofRandom(15000, 65535);
		ofxUDPManager server;
		ofxTest(server.Create(),"create udp socket");
		ofxTest(server.SetNonBlocking(false), "set blocking");
		ofxTest(server.Bind(port), "bind udp socket");
		ofxTest(server.Connect("127.0.0.1", port+1), "set ip and port to send for udp socket");

		std::string messageSent="message";
		std::vector<char> bufferReceive(messageSent.size(),0);
		std::string messageReceived;

		ofxUDPManager client;
		ofxTest(client.Create(), "create udp socket");
		ofxTest(client.SetNonBlocking(false), "set udp socket blocking");
		ofxTest(client.Bind(port+1), "bind udp socket");
		ofxTest(client.Connect("127.0.0.1", port), "set ip and port to send for udp socket");


		ofxTest(client.Send(messageSent.c_str(), messageSent.size()), "client send blocking");
		auto ret = server.Receive(bufferReceive.data(), messageSent.size());
		ofxTest(ret>=0, "server receiving block");
		messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
		ofxTestEq(messageSent, messageReceived, "client messageSent == server messageReceived");


		ofxTest(server.Send(messageSent.c_str(), messageSent.size()), "server send blocking");
		bufferReceive.assign(messageSent.size(),0);
		ret = client.Receive(bufferReceive.data(), messageSent.size());
		ofxTest(ret>=0, "client receiving block");
		messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
		ofxTestEq(messageSent, messageReceived, "server messageSent == client messageReceived");


		ofxTest(client.Send(messageSent.c_str(), messageSent.size()), "client send blocking 2nd time");
		ret = server.Receive(bufferReceive.data(), messageSent.size());
		ofxTest(ret>=0, "server receiving block 2nd time");
		messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
		ofxTestEq(messageSent, messageReceived, "2nd client messageSent == server messageReceived");


		ofxTest(server.Send(messageSent.c_str(), messageSent.size()), "server send blocking 2nd time");
		bufferReceive.assign(messageSent.size(),0);
		ret = client.Receive(bufferReceive.data(), messageSent.size());
		ofxTest(ret>=0, "client receiving block 2nd time");
		messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
		ofxTestEq(messageSent, messageReceived, "2nd server messageSent == client messageReceived");
	}

	void testTimeOutRecv(){
		int port = ofRandom(15000, 65535);
		ofxUDPManager server;
		ofxTest(server.Create(),"create udp socket");
		ofxTest(server.SetNonBlocking(false), "set blocking");
		ofxTest(server.Bind(port), "bind udp socket");
		server.SetTimeoutReceive(5);

		char c;
		auto then = ofGetElapsedTimeMillis();
		auto ret = server.Receive(&c,1);
		auto now = ofGetElapsedTimeMillis();
		ofxTestEq(ret,SOCKET_TIMEOUT,"socket receive timeout");
		// seems timers in the test servers are not very accurate so
		// we test this with a margin of 500ms
        ofxTestGt(now-then, 4500, "socket receive timeouts after 5s");
	}

    void testPortsStayBound(){
        ofLogNotice() << "----------------------";
        ofLogNotice() << "testPortsStayBound";
        ofLogNotice() << "tests #3656";

        int serverport = ofRandom(15000, 65535);
        int clientport = serverport-1;
        ofxUDPManager server;
        ofxTest(server.Create(),"create udp socket");
        ofxTest(server.SetNonBlocking(false), "set blocking");
        ofxTest(server.Bind(serverport), "bind udp socket");
        ofxTest(server.Connect("127.0.0.1", clientport), "set ip and port to send for udp socket");

        std::string messageSent="message";
        std::vector<char> bufferReceive(messageSent.size(),0);
        std::string messageReceived;

        ofxUDPManager client;
        ofxTest(client.Create(), "create udp socket");
        ofxTest(client.SetNonBlocking(false), "set udp socket blocking");
        ofxTest(client.Bind(clientport), "bind udp socket");
        ofxTest(client.Connect("127.0.0.1", serverport), "set ip and port to send for udp socket");


        std::string receivedAddress;
        int receivedPort;

        ofxTest(client.Send(messageSent.c_str(), messageSent.size()), "client send blocking");
        auto ret = server.Receive(bufferReceive.data(), messageSent.size());
        ofxTest(ret>=0, "server receiving block");
        messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
        ofxTestEq(messageSent, messageReceived, "client messageSent == server messageReceived");
        ofxTest(server.GetRemoteAddr(receivedAddress, receivedPort), "Could get remote address and port");
        ofxTestEq(receivedPort, clientport, "server received from clients bound port");


        ofxTest(server.Send(messageSent.c_str(), messageSent.size()), "server send blocking");
        bufferReceive.assign(messageSent.size(),0);
        ret = client.Receive(bufferReceive.data(), messageSent.size());
        ofxTest(ret>=0, "client receiving block");
        messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
        ofxTestEq(messageSent, messageReceived, "server messageSent == client messageReceived");
        ofxTest(client.GetRemoteAddr(receivedAddress, receivedPort), "Could get remote address and port");
        ofxTestEq(receivedPort, serverport, "client received from servers bound port");


        ofxTest(client.Send(messageSent.c_str(), messageSent.size()), "client send blocking 2nd time");
        ret = server.Receive(bufferReceive.data(), messageSent.size());
        ofxTest(ret>=0, "server receiving block 2nd time");
        messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
        ofxTestEq(messageSent, messageReceived, "2nd client messageSent == server messageReceived");
        ofxTest(server.GetRemoteAddr(receivedAddress, receivedPort), "Could get remote address and port");
        ofxTestEq(receivedPort, clientport, "server received from clients bound port");


        ofxTest(server.Send(messageSent.c_str(), messageSent.size()), "server send blocking 2nd time");
        bufferReceive.assign(messageSent.size(),0);
        ret = client.Receive(bufferReceive.data(), messageSent.size());
        ofxTest(ret>=0, "client receiving block 2nd time");
        messageReceived.assign(bufferReceive.begin(), bufferReceive.end());
        ofxTestEq(messageSent, messageReceived, "2nd server messageSent == client messageReceived");
        ofxTest(client.GetRemoteAddr(receivedAddress, receivedPort), "Could get remote address and port");
        ofxTestEq(receivedPort, serverport, "client received from servers bound port");
    }

	void run(){
		testNonBlocking();
		testBlocking();
		testTimeOutRecv();
        testPortsStayBound();
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

#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include "ofxNetwork.h"

class ofApp: public ofxUnitTestsApp{
public:
	void testNonBlocking(int timeout = 0){
		ofLogNotice() << "---------------------------------------";
		ofLogNotice() << "testNonBlocking";

		int port = ofRandom(15000, 65535);

		ofxTCPServer server;
		test(server.setup(port,false), "non blocking server");

		ofxTCPClient client;
		test(client.setup("127.0.0.1",port,false), "non blocking client");

		server.waitConnectedClient(500);

		bool received = false;
		std::string messageSent = "message";
		std::string messageReceived;

		test(client.send(messageSent), "send non blocking from client");
		for(int i=0;i<server.getLastID();i++){
			if(!server.isClientConnected(i)){
				continue;
			}
			for(int i=0;i<10 && !received;i++){
				ofSleepMillis(200);
				messageReceived = server.receive(i);
				if(messageReceived == messageSent){
					received = true;
				}
			}
			test(received, "receive non blocking from server");
		}

		for(int i=0;i<server.getLastID();i++){
			if(!server.isClientConnected(i)){
				continue;
			}
			test(server.send(i,messageSent), "send non blocking from server");
			messageReceived = "";
			for(int i=0;i<10 && !received;i++){
				ofSleepMillis(200);
				messageReceived = client.receive();
				if(messageReceived == messageSent){
					received = true;
				}
			}
			test(received, "receive non blocking from client");
			break;
		}
	}

	void testBlocking(){
		ofLogNotice() << "";
		ofLogNotice() << "---------------------------------------";
		ofLogNotice() << "testBlocking";

		int port = ofRandom(15000, 65535);

		ofxTCPServer server;
		test(server.setup(port,true), "blocking server");

		ofxTCPClient client;
		test(client.setup("127.0.0.1",port,true), "blocking client");

		// wait for connection to be made
		server.waitConnectedClient(500);

		std::string messageSent = "message";
		test(client.send(messageSent), "send blocking from client");
		test_eq(server.receive(0), messageSent, "receive blocking from server");
		test(server.send(0, messageSent), "send blocking from server");
		test_eq(client.receive(), messageSent, "receive blocking from client");
	}

	void disconnectionAutoDetection(){
		ofLogNotice() << "";
		ofLogNotice() << "---------------------------------------";
		ofLogNotice() << "disconnectionAutoDetection";

		int port = ofRandom(15000, 65535);

		ofxTCPServer server;
		test(server.setup(port,true), "blocking server");

		ofxTCPClient client;
		test(client.setup("127.0.0.1",port,true), "blocking client");

		// wait for connection to be made
		server.waitConnectedClient(500);

		test(server.isConnected(), "server is still connected");
		test(client.isConnected(), "client is still connected");

		test(server.disconnectAllClients(), "server closes correctly");
		test(!client.isConnected(), "client detects disconnection");

		test(client.setup("127.0.0.1",port,true), "client reconnects");

		test(client.close(), "client disconnects");
		test(!server.isClientConnected(0), "server detects disconnection");
		test(!server.isClientConnected(1), "server detects disconnection");
	}

	void testSendRaw(){
		ofLogNotice() << "";
		ofLogNotice() << "---------------------------------------";
		ofLogNotice() << "testSendRaw";

		int port = ofRandom(15000, 65535);

		ofxTCPServer server;
		test(server.setup(port,true), "blocking server");

		ofxTCPClient client;
		test(client.setup("127.0.0.1", port, true), "blocking client");

		std::string messageSent = "message";
		test(client.sendRaw(messageSent), "send blocking from client");

		// wait for connection to be made
		server.waitConnectedClient(500);


		std::vector<char> messageReceived(messageSent.size()+1, 0);
		int received = 0;
		do{
			auto ret = server.receiveRawBytes(0, messageReceived.data() + received, messageSent.size());
			test(ret>0, "received blocking from server");
			if(ret>0){
				received += ret;
			}else{
				break;
			}
		}while(received<messageSent.size());

		test_eq(messageSent, std::string(messageReceived.data()), "messageSent == messageReceived");
	}

	void testSendRawBytes(){
		ofLogNotice() << "";
		ofLogNotice() << "---------------------------------------";
		ofLogNotice() << "testSendRawBytes";

		int port = ofRandom(15000, 65535);

		ofxTCPServer server;
		test(server.setup(port,true), "blocking server");

		ofxTCPClient client;
		test(client.setup("127.0.0.1", port, true), "blocking client");

		// wait for connection to be made
		server.waitConnectedClient(500);


		std::string messageSent = "message";
		test(client.sendRawBytes(messageSent.c_str(), messageSent.size()), "send blocking from client");

		std::vector<char> messageReceived(messageSent.size()+1, 0);
		int received = 0;
		do{
			auto ret = server.receiveRawBytes(0, messageReceived.data() + received, messageSent.size());
			test(ret>0, "received blocking from server");
			if(ret>0){
				received += ret;
			}else{
				break;
			}
		}while(received<messageSent.size());

		test_eq(messageSent, std::string(messageReceived.data()), "messageSent == messageReceived");
	}

	void testWrongConnect(){
		ofLogNotice() << "";
		ofLogNotice() << "---------------------------------------";
		ofLogNotice() << "testWrongConnect";

		ofxTCPManager client;
		client.SetTimeoutConnect(5);
		test(client.Create(), "socket creation");
		auto then = ofGetElapsedTimeMillis();
		test(!client.Connect("127.0.0.1", 200), "connect to non open port, if this fails the port might be really open:");
        auto now = ofGetElapsedTimeMillis();
		test(now-then<6000, "Connect waits 5s to timeout, waited: " + ofToString(now - then));
	}

	void testReceiveTimeout(){
		ofLogNotice() << "";
		ofLogNotice() << "---------------------------------------";
		ofLogNotice() << "testReceiveTimeout";

		int port = ofRandom(15000, 65535);
		ofxTCPManager server;
		test(server.Create(), "server socket creation");
		test(server.Bind(port), "server socket bind");
		test(server.Listen(1), "server socket listen");
		std::condition_variable done;
		std::mutex mtx;
		std::thread serverThread([&]{
			ofxTCPManager client;
			test(server.Accept(client), "server socket accept");
			std::unique_lock<std::mutex> lck(mtx);
			done.wait(lck);
		});

		ofxTCPManager client;
		test(client.Create(), "client socket create");
		test(client.Connect("127.0.0.1", port), "client socket connect");

		ofSleepMillis(100);
		client.SetTimeoutReceive(5);
		auto then = ofGetElapsedTimeMillis();
		char buffer;
		test_eq(client.Receive(&buffer,1), SOCKET_TIMEOUT, "socket timeouts on no receive");
		auto now = ofGetElapsedTimeMillis();
		// seems timers in the test servers are not very accurate so
		// we test this with a margin of 500ms
		test_gt(now-then, 4500, "Connect waits 5s to timeout, waited: " + ofToString(now - then));
		done.notify_all();
		serverThread.join();
	}

	void testSendMaxSize(){
		ofLogNotice() << "";
		ofLogNotice() << "---------------------------------------";
		ofLogNotice() << "testSendMaxSize tests #3478";

		int port = ofRandom(15000, 65535);

		ofxTCPServer server;
		test(server.setup(port,true), "blocking server");

		ofxTCPClient client;
		test(client.setup("127.0.0.1", port, true), "blocking client");

		// wait for connection to be made
		server.waitConnectedClient(500);

		string str;
		string received;
		for(int i=0;i<TCP_MAX_MSG_SIZE;i++){
			str.append(ofToString((int)ofRandom(10)));
		}
		test(server.sendRawMsg(0, str.c_str(), str.size()), "could send max size");
		do{
			received += client.receiveRaw();
		}while(received.size()<str.size());
		test_eq(received, str, "received max size message == sent message");
	}

	void run(){
		ofSeedRandom(ofGetSeconds());
		testNonBlocking();
		testBlocking();
		disconnectionAutoDetection();
		testSendRaw();
		testSendRawBytes();
		testWrongConnect();
		testReceiveTimeout();
		testSendMaxSize();
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


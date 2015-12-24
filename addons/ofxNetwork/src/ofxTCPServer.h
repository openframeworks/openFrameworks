#pragma once

#include "ofConstants.h"
#include "ofThread.h"
#include "ofxTCPManager.h"
#include <map>
#include <condition_variable>

#define TCP_MAX_CLIENTS  32

//forward decleration
class ofxTCPClient;

class ofxTCPServer : public ofThread{

	public:

		ofxTCPServer();
		~ofxTCPServer();

		// ofxTCPServer can't be copied to avoid problems with destruction
		ofxTCPServer(const ofxTCPServer & mom) = delete;
		ofxTCPServer & operator=(const ofxTCPServer & mom) = delete;

		void setVerbose(bool _verbose);
		bool setup(int _port, bool blocking = false);
		void setMessageDelimiter(std::string delim);
	
		bool close();
		bool disconnectClient(int clientID);
        bool disconnectAllClients();

		int getNumClients(); //total number of clients - not sutible for iterating through clients with
		int getLastID(); //this returns the last current id number if you want to loop through with a for loop 
		
		int getPort();
		bool isConnected();

		int getClientPort(int clientID);
		std::string getClientIP(int clientID);

		bool isClientConnected(int clientID);

		//send data as a string - a short message
		//is added to the end of the string which is
		//used to indicate the end of the message to
		//the receiver see: STR_END_MSG (ofTCPClient.h)
		bool send(int clientID, std::string message);
		bool sendToAll(std::string message);


		// same as send for binary data
		bool sendRawMsg(int clientID, const char * rawMsg, const int numBytes);
		bool sendRawMsgToAll(const char * rawMsg, const int numBytes);

		//send and receive raw bytes lets you send and receive
		//byte (char) arrays without modifiying or appending the data.
		//Strings terminate on null bytes so this is the better option
		//if you are trying to send something other than just ascii strings

		bool sendRawBytes(int clientID, const char * rawBytes, const int numBytes);
		bool sendRawBytesToAll(const char * rawBytes, const int numBytes);

		//the received message length in bytes
		int getNumReceivedBytes(int clientID);

		//get the message as a string
		//this will only work with messages coming via
		//send() and sendToAll()
		//or from messages terminating with the STR_END_MSG
		//which by default is  [/TCP]
		//eg: if you want to send "Hello World" from other
		//software and want to receive it as a string
		//sender should send "Hello World[/TCP]"
		std::string receive(int clientID);

		// same as receive for binary data
		int receiveRawMsg(int clientID, char * receiveBytes,  int numBytes);

		//pass in buffer to be filled - make sure the buffer
		//is at least as big as numBytes
		int receiveRawBytes(int clientID, char * receiveBytes,  int numBytes);

		//fill a buffer as much as possible but leave the data on the TCP stack
		//amount of filled-bytes returned
		int peekReceiveRawBytes(int clientID, char * receiveBytes,  int numBytes);

		void waitConnectedClient();
		void waitConnectedClient(int ms);

	private:
		ofxTCPClient & getClient(int clientID);
		bool isClientSetup(int clientID);

		void threadedFunction();

		ofxTCPManager			TCPServer;
		std::map<int,std::shared_ptr<ofxTCPClient> >	TCPConnections;
		std::mutex					mConnectionsLock;
        std::condition_variable serverReady;

		bool			connected;
		std::string			str;
		int				idCount, port;
		bool			bClientBlocking;
		std::string			messageDelimiter;

};

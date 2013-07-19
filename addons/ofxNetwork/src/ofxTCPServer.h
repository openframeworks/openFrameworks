#pragma once

#include "ofConstants.h"
#include "ofThread.h"
#include "ofxTCPManager.h"
#include <map>

#define TCP_MAX_CLIENTS  32

//forward decleration
class ofxTCPClient;

class ofxTCPServer : public ofThread{

	public:

		ofxTCPServer();
		~ofxTCPServer();
		void setVerbose(bool _verbose);
		bool setup(int _port, bool blocking = false);
		void setMessageDelimiter(string delim);
	
		bool close();
		bool disconnectClient(int clientID);

		int getNumClients(); //total number of clients - not sutible for iterating through clients with
		int getLastID(); //this returns the last current id number if you want to loop through with a for loop 
		
		int getPort();
		bool isConnected();

		int getClientPort(int clientID);
		string getClientIP(int clientID);

		bool isClientConnected(int clientID);

		//send data as a string - a short message
		//is added to the end of the string which is
		//used to indicate the end of the message to
		//the receiver see: STR_END_MSG (ofTCPClient.h)
		bool send(int clientID, string message);
		bool sendToAll(string message);


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
		string receive(int clientID);

		// same as receive for binary data
		int receiveRawMsg(int clientID, char * receiveBytes,  int numBytes);

		//pass in buffer to be filled - make sure the buffer
		//is at least as big as numBytes
		int receiveRawBytes(int clientID, char * receiveBytes,  int numBytes);




	private:
		// private copy so this can't be copied to avoid problems with destruction
		ofxTCPServer(const ofxTCPServer & mom){};
		ofxTCPServer & operator=(const ofxTCPServer & mom){return *this;}

		ofxTCPClient & getClient(int clientID);
		bool isClientSetup(int clientID);

		void threadedFunction();

		ofxTCPManager			TCPServer;
		map<int,ofPtr<ofxTCPClient> >	TCPConnections;
		ofMutex					mConnectionsLock;

		bool			connected;
		string			str;
		int				idCount, port;
		bool			bClientBlocking;
		string			messageDelimiter;

};

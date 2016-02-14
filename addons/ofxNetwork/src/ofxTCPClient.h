#pragma once

#include "ofConstants.h"
#include "ofxTCPManager.h"
#include "ofFileUtils.h"
#include "ofTypes.h"

#define TCP_MAX_MSG_SIZE 512
//#define STR_END_MSG "[/TCP]"
//#define STR_END_MSG_LEN 6


class ofxTCPClient{


	public:

		ofxTCPClient();
		~ofxTCPClient();

		void threadedFunction();

		void setVerbose(bool _verbose);
		bool setup(string ip, int _port, bool blocking = false);
		void setMessageDelimiter(string delim);
		bool close();

	
		//send data as a string - a short message
		//is added to the end of the string which is
		//used to indicate the end of the message to
		//the receiver see: STR_END_MSG (ofxTCPClient.h)
		bool send(string message);

		//send data as a string without the end message
		bool sendRaw(string message);

		//same as send for binary messages
		bool sendRawMsg(const char * msg, int size);

		//the received message length in bytes
		int getNumReceivedBytes();

		//send and receive raw bytes lets you send and receive
		//byte (char) arrays without modifiying or appending the data.
		//Strings terminate on null bytes so this is the better option
		//if you are trying to send something other than just ascii strings
		bool sendRawBytes(const char * rawBytes, const int numBytes);


		//get the message as a string
		//this will only work with messages coming via
		//send() and sendToAll()
		//or from messages terminating with the STR_END_MSG
		//which by default is  [/TCP]
		//eg: if you want to send "Hello World" from other
		//software and want to receive it as a string
		//sender should send "Hello World[/TCP]"
		string receive();

		//no terminating string you will need to be sure
		//you are receiving all the data by using a loop
		string receiveRaw();

		//pass in buffer to be filled - make sure the buffer
		//is at least as big as numBytes
		int receiveRawBytes(char * receiveBytes, int numBytes);

		//fill a buffer as much as possible but leave the data on the TCP stack
		//amount of filled-bytes returned
		int peekReceiveRawBytes(char * receiveBytes, int numBytes);

		//same as receive for binary data
		//pass in buffer to be filled - make sure the buffer
		//is at least as big as numBytes
		int receiveRawMsg(char * receiveBuffer, int numBytes);


		bool isConnected();
		int getPort();
		string getIP();




private:
		// private copy so this can't be copied to avoid problems with destruction
		ofxTCPClient(const ofxTCPManager & mom){};
		ofxTCPClient & operator=(const ofxTCPClient & mom){return *this;}

        //don't use this one
        //for server to use internally only!
        //--------------------------
		bool setupConnectionIdx(int _index, bool blocking);
		bool isClosingCondition(int messageSize, int errorCode);
		friend class ofxTCPServer;

		ofxTCPManager	TCPClient;

		char			tmpBuff[TCP_MAX_MSG_SIZE+1];
		ofBuffer 		tmpBuffReceive;
		ofBuffer 		tmpBuffSend;
		string			str, tmpStr, ipAddr;
		int				index, messageSize, port;
		bool			connected;
		string 			partialPrevMsg;
		string			messageDelimiter;
};

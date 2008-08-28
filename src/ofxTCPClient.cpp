#include "ofxTCPClient.h"
#include "ofAppRunner.h"  


//--------------------------
ofxTCPClient::ofxTCPClient(){
	
	verbose		= true;
	connected	= false;
	messageSize = 0;
	port		= 0;
	index		= -1;
	str			= "";
	tmpStr		= "";
	ipAddr		="000.000.000.000";
	
	memset(tmpBuff,  0, TCP_MAX_MSG_SIZE);
}

//--------------------------
ofxTCPClient::~ofxTCPClient(){
	close();
}

//--------------------------
void ofxTCPClient::setVerbose(bool _verbose){
	verbose = _verbose;
}

//--------------------------
bool ofxTCPClient::setup(string ip, int _port, bool blocking){
	
	if( !TCPClient.Create() ){
		if(verbose)printf("ofxTCPClient: Create() failed\n");
		return false;
	}else if( !TCPClient.Connect((char *)ip.c_str(), _port) ){
		if(verbose)printf("ofxTCPClient: Connect(%s, %i) failed\n", ip.c_str(), _port);
		TCPClient.Close(); //we free the connection
		return false;
	}
	
	TCPClient.SetNonBlocking(!blocking);
	
	port		= _port;
	ipAddr		= ip;
	connected	= true;

	return true;
}

//don't use this one
//for server to use internally only!
//--------------------------
bool ofxTCPClient::setup(int _index, bool blocking){
	index = _index;
	
	//this fetches the port that the server 
	//sets up the connection on
	//different to the server's listening port
	InetAddr addr;
	if( TCPClient.GetRemoteAddr(&addr) ){
		port   =  addr.GetPort();
		ipAddr =  addr.DottedDecimal();
	}
	
	TCPClient.SetNonBlocking(!blocking);
	
	connected = true;
	return true;
}


//--------------------------
bool ofxTCPClient::close(){
	if( connected ){
			
		if( !TCPClient.Close() ){
			if(verbose)printf("ofxTCPClient: Close() failed\n");
			return false;
		}else{
			connected = false;
			return true;
		}
	}else{
		return true;
	}
}

//--------------------------
bool ofxTCPClient::send(string message){

	//fuck this is ghetto 
	//it is the only way I can think of
	//to make sure we get the full message
	//there must be a better way than this :)
	message += STR_END_MSG;
	message += (char)0; //for flash

	if( !TCPClient.SendAll( message.c_str(), message.length() ) ){
		if(verbose)printf("ofxTCPClient: send() failed\n");
		return false;
	}else{
		return true;
	}
}

//--------------------------
bool ofxTCPClient::sendRawBytes(const char* rawBytes, const int numBytes){
	if( numBytes <= 0) return false;
	
	if( !TCPClient.SendAll(rawBytes, numBytes) ){
		if(verbose)printf("ofxTCPClient: sendRawBytes() failed\n");
		return false;
	}else{
		return true;
	}
}


//this only works after you have called recieve
//--------------------------
int ofxTCPClient::getNumReceivedBytes(){
	return messageSize;
}

//--------------------------
string ofxTCPClient::receive(){
	
	tmpStr = "";
	str    = "";

	//by default get all data in the buffer
	while( receiveRawBytes(tmpBuff, TCP_MAX_MSG_SIZE) > 0 ){
		tmpStr += tmpBuff;
		memset(tmpBuff,  0, TCP_MAX_MSG_SIZE);
	}
		
	int pos = tmpStr.find(STR_END_MSG, 0);
	if(pos >= 0) str = tmpStr.substr(0, pos);	
	
	return str;
}

//--------------------------
int ofxTCPClient::receiveRawBytes(char * receiveBuffer, int numBytes){	
	return messageSize = TCPClient.Receive(receiveBuffer, numBytes);
}

//--------------------------
bool ofxTCPClient::isConnected(){
	return connected;
}

//--------------------------
int ofxTCPClient::getPort(){
	return port;
}

//--------------------------
string ofxTCPClient::getIP(){
	return ipAddr;
}		







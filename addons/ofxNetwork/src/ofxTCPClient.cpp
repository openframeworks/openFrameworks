#include "ofxTCPClient.h"
#include "ofAppRunner.h"


//--------------------------
ofxTCPClient::ofxTCPClient(){

	verbose		= false;
	connected	= false;
	messageSize = 0;
	port		= 0;
	index		= -1;
	str			= "";
	tmpStr		= "";
	ipAddr		="000.000.000.000";

	partialPrevMsg = "";

	memset(tmpBuff,  0, TCP_MAX_MSG_SIZE+1);
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
	connected 	= true;
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
	// tcp is a stream oriented protocol
	// so there's no way to be sure were
	// a message ends without using a terminator
	// note that you will receive a trailing [/TCP]\0
	// if sending from here and receiving from receiveRaw or
	// other applications
	if(!connected){
		if(verbose)printf("ofxTCPClient: trying to send while not connected\n");
		return false;
	}
	message = partialPrevMsg + message + STR_END_MSG;
	message += (char)0; //for flash
	int ret = TCPClient.SendAll( message.c_str(), message.length() );
	if( ret == 0 ){
		if(verbose)printf("ofxTCPClient: other side disconnected\n");
		close();
		return false;
	}else if(ret<0){
		if(verbose)printf("ofxTCPClient: sendAll() failed\n");
		return false;
	}else if(ret<(int)message.length()){
		// in case of partial send, store the
		// part that hasn't been sent and send
		// with the next message to not corrupt
		// next messages
		partialPrevMsg=message.substr(ret);
		return true;
	}else{
		partialPrevMsg = "";
		return true;
	}
}

//--------------------------
bool ofxTCPClient::sendRaw(string message){
	if( message.length() == 0) return false;

	if( !TCPClient.SendAll(message.c_str(), message.length()) ){
		if(verbose)printf("ofxTCPClient: sendRawBytes() failed\n");
		close();
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
		close();
		return false;
	}else{
		return true;
	}
}


//this only works after you have called receive
//--------------------------
int ofxTCPClient::getNumReceivedBytes(){
	return messageSize;
}

//--------------------------
static void removeZeros(char * buffer, int size){
	for(int i=0;i<size-1;i++){
		if(buffer[i]==(char)0){
			for(int j=i;j<size-1;j++){
				buffer[j]=buffer[j+1];
			}
			buffer[size-1]=(char)0;
		}
	}
}

//--------------------------
string ofxTCPClient::receive(){

	str    = "";
	int length=-2;
	//only get data from the buffer if we don't have already some complete message
	if(tmpStr.find(STR_END_MSG)==string::npos){
		memset(tmpBuff,  0, TCP_MAX_MSG_SIZE+1); //one more so there's always a \0 at the end for string concat
		length = TCPClient.Receive(tmpBuff, TCP_MAX_MSG_SIZE);
		if(length>0){ // don't copy the data if there was an error or disconnection
			removeZeros(tmpBuff,length);
			tmpStr += tmpBuff;
		}
	}

	// check for connection reset or disconnection
	if((length==-1 && errno==TCP_CONNRESET ) || length == 0){
		close();
		if(tmpStr.length()==0) // return if there's no more data left in the buffer
			return "";
	}

	// process any available data
	if(tmpStr.find(STR_END_MSG)!=string::npos){
		str=tmpStr.substr(0,tmpStr.find(STR_END_MSG));
		tmpStr=tmpStr.substr(tmpStr.find(STR_END_MSG)+STR_END_MSG_LEN);
	}
	return str;
}

//--------------------------
int ofxTCPClient::receiveRawBytes(char * receiveBuffer, int numBytes){
	messageSize = TCPClient.Receive(receiveBuffer, numBytes);
	if(messageSize==0){
		close();
	}
	return messageSize;
}

//--------------------------
string ofxTCPClient::receiveRaw(){
	messageSize = TCPClient.Receive(tmpBuff, TCP_MAX_MSG_SIZE);
	if(messageSize==0){
		close();
	}else if(messageSize<TCP_MAX_MSG_SIZE) {
        // null terminate!!
        tmpBuff[messageSize] = 0;
    }

	return tmpBuff;
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







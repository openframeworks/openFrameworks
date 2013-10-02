#include "ofxTCPClient.h"
#include "ofAppRunner.h"
#include "ofxNetworkUtils.h"

//--------------------------
ofxTCPClient::ofxTCPClient(){

	connected	= false;
	messageSize = 0;
	port		= 0;
	index		= -1;
	str			= "";
	tmpStr		= "";
	ipAddr		="000.000.000.000";

	partialPrevMsg = "";
	messageDelimiter = "[/TCP]";
	memset(tmpBuff,  0, TCP_MAX_MSG_SIZE+1);
}

//--------------------------
ofxTCPClient::~ofxTCPClient(){
	close();
}

//--------------------------
void ofxTCPClient::setVerbose(bool _verbose){
    ofLogWarning("ofxTCPClient") << "setVerbose(): is deprecated, replaced by ofLogWarning and ofLogError";
}

//--------------------------
bool ofxTCPClient::setup(string ip, int _port, bool blocking){

	if( !TCPClient.Create() ){
		ofLogError("ofxTCPClient") << "setup(): couldn't create client";
		return false;
	}else if( !TCPClient.Connect((char *)ip.c_str(), _port) ){
		ofLogError("ofxTCPClient") << "setup(): couldn't connect to " << ip << " " << _port;
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
			ofLogError("ofxTCPClient") << "close(): couldn't close client";
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
void ofxTCPClient::setMessageDelimiter(string delim){
	if(delim != ""){
		messageDelimiter = delim; 
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
		ofLogWarning("ofxTCPClient") << "send(): not connected, call setup() first";
		return false;
	}
	message = partialPrevMsg + message + messageDelimiter;
	message += (char)0; //for flash
	int ret = TCPClient.SendAll( message.c_str(), message.length() );
	if( ret == 0 ){
		ofLogWarning("ofxTCPClient") << "send(): client disconnected";
		close();
		return false;
	}else if(ret<0){
		ofLogError("ofxTCPClient") << "send(): sending failed";
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

bool ofxTCPClient::sendRawMsg(const char * msg, int size){

	// tcp is a stream oriented protocol
	// so there's no way to be sure were
	// a message ends without using a terminator
	// note that you will receive a trailing [/TCP]\0
	// if sending from here and receiving from receiveRaw or
	// other applications
	if(!connected){
		ofLogWarning("ofxTCPClient") << "sendRawMsg(): not connected, call setup() first";
		return false;
	}
	tmpBuffSend.append(msg,size);
	tmpBuffSend.append(messageDelimiter.c_str(),messageDelimiter.size());

	int ret = TCPClient.SendAll( tmpBuffSend.getBinaryBuffer(), tmpBuffSend.size() );
	if( ret == 0 ){
		ofLogWarning("ofxTCPClient") << "sendRawMsg(): client disconnected";
		close();
		return false;
	}else if(ret<0){
		ofLogError("ofxTCPClient") << "sendRawMsg(): sending failed";
		return false;
	}else if(ret<size){
		// in case of partial send, store the
		// part that hasn't been sent and send
		// with the next message to not corrupt
		// next messages
		tmpBuffSend.set(&tmpBuffSend.getBinaryBuffer()[ret],tmpBuffSend.size()-ret);
		return true;
	}else{
		tmpBuffSend.clear();
		return true;
	}
}

//--------------------------
bool ofxTCPClient::sendRaw(string message){
	if( message.length() == 0) return false;

	if( !TCPClient.SendAll(message.c_str(), message.length()) ){
		ofLogError("ofxTCPClient") << "sendRawBytes(): sending failed";
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
		ofLogError("ofxTCPClient") << "sendRawBytes(): sending failed";
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
	if(tmpStr.find(messageDelimiter)==string::npos){
		memset(tmpBuff,  0, TCP_MAX_MSG_SIZE+1); //one more so there's always a \0 at the end for string concat
		length = TCPClient.Receive(tmpBuff, TCP_MAX_MSG_SIZE);
		if(length>0){ // don't copy the data if there was an error or disconnection
			removeZeros(tmpBuff,length);
			tmpStr += tmpBuff;
		}
	}

	// check for connection reset or disconnection
	if((length==-1 && (ofxNetworkCheckError() == ECONNRESET) ) || length == 0){
		close();
		if(tmpStr.length()==0) // return if there's no more data left in the buffer
			return "";
	}

	// process any available data
	if(tmpStr.find(messageDelimiter)!=string::npos){
		str=tmpStr.substr(0,tmpStr.find(messageDelimiter));
		tmpStr=tmpStr.substr(tmpStr.find(messageDelimiter)+messageDelimiter.size());
	}
	return str;
}


static int findDelimiter(char * data, int size, string delimiter){
	unsigned int posInDelimiter=0;
	for(int i=0;i<size;i++){
		if(data[i]==delimiter[posInDelimiter]){
			posInDelimiter++;
			if(posInDelimiter==delimiter.size()) return i-delimiter.size()+1;
		}else{
			posInDelimiter=0;
		}
	}
	return -1;
}

int ofxTCPClient::receiveRawMsg(char * receiveBuffer, int numBytes){
	int length=-2;
	//only get data from the buffer if we don't have already some complete message
	if(findDelimiter(tmpBuffReceive.getBinaryBuffer(),tmpBuffReceive.size(),messageDelimiter)==-1){
		memset(tmpBuff,  0, TCP_MAX_MSG_SIZE);
		length = receiveRawBytes(tmpBuff, TCP_MAX_MSG_SIZE);
		if(length>0){ // don't copy the data if there was an error or disconnection
			tmpBuffReceive.append(tmpBuff,length);
		}
	}

	// process any available data
	int posDelimiter = findDelimiter(tmpBuffReceive.getBinaryBuffer(),tmpBuffReceive.size(),messageDelimiter);
	if(posDelimiter>0){
		memcpy(receiveBuffer,tmpBuffReceive.getBinaryBuffer(),posDelimiter);
		if(tmpBuffReceive.size() > posDelimiter + (int)messageDelimiter.size()){
			memcpy(tmpBuff,tmpBuffReceive.getBinaryBuffer()+posDelimiter+messageDelimiter.size(),tmpBuffReceive.size()-(posDelimiter+messageDelimiter.size()));
			tmpBuffReceive.set(tmpBuff,tmpBuffReceive.size()-(posDelimiter+messageDelimiter.size()));
		}else{
			tmpBuffReceive.clear();
		}
	}

	if(posDelimiter>0){
		return posDelimiter;
	}else{
		return 0;
	}
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







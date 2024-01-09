#include "ofxTCPClient.h"
#include "ofAppRunner.h"
#include "ofxNetworkUtils.h"
#include "ofLog.h"

using std::string;

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

	ofxTCPSettings settings(ip, _port);

	settings.blocking = blocking;

	return setup(settings);
}

//--------------------------
bool ofxTCPClient::setup(const ofxTCPSettings & settings){

	if( !TCPClient.Create() ){
		ofLogError("ofxTCPClient") << "setup(): couldn't create client";
		return false;
	}else if( !TCPClient.Connect((char *)settings.address.c_str(), settings.port) ){
		ofLogError("ofxTCPClient") << "setup(): couldn't connect to " << settings.address << " " << settings.port;
		TCPClient.Close(); //we free the connection
		return false;
	}

	TCPClient.SetNonBlocking(!settings.blocking);

	setMessageDelimiter(settings.messageDelimiter);

	port		= settings.port;
	ipAddr		= settings.address;
	connected	= true;
	return true;
}

//don't use this one
//for server to use internally only!
//--------------------------
bool ofxTCPClient::setupConnectionIdx(int _index, bool blocking){
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
            ofLogVerbose("ofxTCPClient") << "closing client";
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
	int errorCode = ofxNetworkGetLastError();
	if( ret<0 ) ofxNetworkLogError(errorCode);
	if( isClosingCondition(ret, errorCode) ){
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

//--------------------------
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

    int ret = TCPClient.SendAll( tmpBuffSend.getData(), tmpBuffSend.size() );
    int errorCode = ofxNetworkGetLastError();
	if( ret<0 ) ofxNetworkLogError(errorCode);
	if( isClosingCondition(ret, errorCode) ){
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
		tmpBuffSend.set(&tmpBuffSend.getData()[ret],tmpBuffSend.size()-ret);
		return true;
	}else{
		tmpBuffSend.clear();
		return true;
	}
}

//--------------------------
bool ofxTCPClient::sendRaw(string message){
	if( message.length() == 0) return false;
    int ret = TCPClient.SendAll(message.c_str(), message.length());
    int errorCode = ofxNetworkGetLastError();
    if( ret<0 ) ofxNetworkLogError(errorCode);
	if( isClosingCondition(ret, errorCode) ){
		ofLogError("ofxTCPClient") << "sendRawBytes(): sending failed";
		close();
		return false;
	}else{
		return ret > 0;
	}
}

//--------------------------
bool ofxTCPClient::sendRawBytes(const char* rawBytes, const int numBytes){
	if( numBytes <= 0) return false;
	int ret = TCPClient.SendAll(rawBytes, numBytes);
    int errorCode = ofxNetworkGetLastError();
    if( ret<0 ) ofxNetworkLogError(errorCode);
	if( isClosingCondition(ret, errorCode) ){
		ofLogError("ofxTCPClient") << "sendRawBytes(): sending failed";
		close();
		return false;
	}else{
		return ret > 0;
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
bool ofxTCPClient::isClosingCondition(int messageSize, int errorCode){
	return (messageSize == SOCKET_ERROR && (errorCode == OFXNETWORK_ERROR(CONNRESET) || errorCode == OFXNETWORK_ERROR(CONNABORTED) || errorCode == OFXNETWORK_ERROR(CONNREFUSED) || errorCode == EPIPE || errorCode == OFXNETWORK_ERROR(NOTCONN)))
		|| (messageSize == 0 && !TCPClient.IsNonBlocking() && TCPClient.GetTimeoutReceive()!=NO_TIMEOUT);
}

//--------------------------
string ofxTCPClient::receive(){
	str    = "";
	char byte;
	while(peekReceiveRawBytes(&byte, 1)>0){
		int length=0;
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
		int errorCode = ofxNetworkGetLastError();
		if( length<0 ) ofxNetworkLogError(errorCode);
		if(isClosingCondition(length,errorCode)){
			close();
			if(tmpStr.length()==0) {
				// return if there's no more data left in the buffer
				return "";
			}
		}

		// process any available data
		if(tmpStr.find(messageDelimiter)!=string::npos){
			str=tmpStr.substr(0,tmpStr.find(messageDelimiter));
			tmpStr=tmpStr.substr(tmpStr.find(messageDelimiter)+messageDelimiter.size());
			return str;
		}
	}
	return "";
}

//--------------------------
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

//--------------------------
int ofxTCPClient::receiveRawMsg(char * receiveBuffer, int numBytes){
	int length=-2;
	//only get data from the buffer if we don't have already some complete message
	if(findDelimiter(tmpBuffReceive.getData(),tmpBuffReceive.size(),messageDelimiter)==-1){
		memset(tmpBuff,  0, TCP_MAX_MSG_SIZE);
		length = receiveRawBytes(tmpBuff, TCP_MAX_MSG_SIZE);
		if(length>0){ // don't copy the data if there was an error or disconnection
			tmpBuffReceive.append(tmpBuff,length);
		}
	}

	// process any available data
	int posDelimiter = findDelimiter(tmpBuffReceive.getData(),tmpBuffReceive.size(),messageDelimiter);
	if(posDelimiter>0){
		memcpy(receiveBuffer,tmpBuffReceive.getData(),posDelimiter);
		if(tmpBuffReceive.size() > (unsigned int) posDelimiter + messageDelimiter.size()){
			memcpy(tmpBuff,tmpBuffReceive.getData()+posDelimiter+messageDelimiter.size(),tmpBuffReceive.size()-(posDelimiter+messageDelimiter.size()));
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
    int errorCode = ofxNetworkGetLastError();
    if(messageSize<0) ofxNetworkLogError(errorCode);
	//	0 is not an error... -1 is
	if(isClosingCondition(messageSize, errorCode)){
		close();
	}
	return messageSize;
}

//--------------------------
int ofxTCPClient::peekReceiveRawBytes(char * receiveBuffer, int numBytes){
    messageSize = TCPClient.PeekReceive(receiveBuffer, numBytes);
    int errorCode = ofxNetworkGetLastError();
    if(messageSize<0) ofxNetworkLogError(errorCode);
    if(isClosingCondition(messageSize, errorCode)){
		close();
	}
	return messageSize;
}

//--------------------------
string ofxTCPClient::receiveRaw(){
    messageSize = TCPClient.Receive(tmpBuff, TCP_MAX_MSG_SIZE);
    int errorCode = ofxNetworkGetLastError();
    if(messageSize<0) ofxNetworkLogError(errorCode);
	if(isClosingCondition(messageSize, errorCode)){
		close();
	}else if(messageSize>=0 && messageSize<TCP_MAX_MSG_SIZE) {
        // null terminate!!
        tmpBuff[messageSize] = 0;
    }

	return tmpBuff;
}

//--------------------------
bool ofxTCPClient::isConnected(){
	if (connected) {
		if (!TCPClient.CheckIsConnected()) {
			close();
		}
	}
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

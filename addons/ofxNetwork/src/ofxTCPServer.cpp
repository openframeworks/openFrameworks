#include "ofxTCPServer.h"
#include "ofxTCPClient.h"

//--------------------------
ofxTCPServer::ofxTCPServer(){
	verbose		= true;
	connected	= false;
	count		= 0;
	port		= 0;
	str			= "";

	TCPConnections = new ofxTCPClient[TCP_MAX_CLIENTS];
}

//--------------------------
ofxTCPServer::~ofxTCPServer(){
	close();
}

//--------------------------
void ofxTCPServer::setVerbose(bool _verbose){
	verbose = _verbose;
}

//--------------------------
bool ofxTCPServer::setup(int _port, bool blocking){
	if( !TCPServer.Create() ){
		if(verbose)printf("ofxTCPServer: create() failed\n");
		return false;
	}
	if( !TCPServer.Bind(_port) ){
		if(verbose)printf("ofxTCPServer: bind(port = %i) failed\n", _port);
		return false;
	}

	connected		= true;
	port			= _port;
	bClientBlocking = blocking;

	startThread(true, false);
	return true;
}

//--------------------------
bool ofxTCPServer::close(){

	if(!connected) return true;

	for(int i = 0; i < count; i++){
		TCPConnections[i].close();
	}

	stopThread(); //stop the thread

	if( !TCPServer.Close() ){
		if(verbose)printf("ofxTCPServer: unable to close connection\n");
		return false;
	}else{
		connected = false;
		return true;
	}
}

//--------------------------
bool ofxTCPServer::disconnectClient(int clientID){
	if( !isClientSetup(clientID) ){
		if(verbose)printf("ofxTCPServer: client %i doesn't exist\n", clientID);
		return false;
	}
	else if(TCPConnections[clientID].close()){
		return true;
	}
	return false;
}

//--------------------------
bool ofxTCPServer::send(int clientID, string message){
	if( !isClientSetup(clientID) ){
		if(verbose)printf("ofxTCPServer: client %i doesn't exist\n", clientID);
		return false;
	}
	else{
		TCPConnections[clientID].send(message);
		return true;
	}
}

//--------------------------
bool ofxTCPServer::sendToAll(string message){
	if(count == 0) return false;

	for(int i = 0; i < count; i++){
		if(TCPConnections[i].isConnected())TCPConnections[i].send(message);
	}
	return true;
}

//--------------------------
string ofxTCPServer::receive(int clientID){
	if( !isClientSetup(clientID) ){
		if(verbose)printf("ofxTCPServer: client %i doesn't exist\n", clientID);
		return "client doesn't exist";
	}

	return TCPConnections[clientID].receive();
}

//--------------------------
bool ofxTCPServer::sendRawBytes(int clientID, const char * rawBytes, const int numBytes){
	if( !isClientSetup(clientID) ){
		if(verbose)printf("ofxTCPServer: client %i doesn't exist\n", clientID);
		return false;
	}
	else{
		return TCPConnections[clientID].sendRawBytes(rawBytes, numBytes);
	}
}

//--------------------------
bool ofxTCPServer::sendRawBytesToAll(const char * rawBytes, const int numBytes){
	if(count == 0 || numBytes <= 0) return false;

	for(int i = 0; i < count; i++){
		if(TCPConnections[i].isConnected())TCPConnections[i].sendRawBytes(rawBytes, numBytes);
	}
	return true;
}

//--------------------------
int ofxTCPServer::getNumReceivedBytes(int clientID){
	if( !isClientSetup(clientID) ){
		if(verbose)printf("ofxTCPServer: client %i doesn't exist\n", clientID);
		return 0;
	}

	return TCPConnections[clientID].getNumReceivedBytes();
}

//--------------------------
int ofxTCPServer::receiveRawBytes(int clientID, char * receiveBytes,  int numBytes){
	if( !isClientSetup(clientID) ){
		if(verbose)printf("ofxTCPServer: client %i doesn't exist\n", clientID);
		return 0;
	}

	return TCPConnections[clientID].receiveRawBytes(receiveBytes, numBytes);
}

//--------------------------
int ofxTCPServer::getClientPort(int clientID){
	if( !isClientSetup(clientID) ){
		if(verbose)printf("ofxTCPServer: client %i doesn't exist\n", clientID);
		return 0;
	}
	else return TCPConnections[clientID].getPort();
}

//--------------------------
string ofxTCPServer::getClientIP(int clientID){
	if( !isClientSetup(clientID) ){
		if(verbose)printf("ofxTCPServer: client %i doesn't exist\n", clientID);
		return "000.000.000.000";
	}
	else return TCPConnections[clientID].getIP();
}

//--------------------------
int ofxTCPServer::getNumClients(){
	return count;
}

//--------------------------
int ofxTCPServer::getPort(){
	return port;
}

//--------------------------
bool ofxTCPServer::isConnected(){
	return connected;
}

//--------------------------
bool ofxTCPServer::isClientSetup(int clientID){
	return (clientID < count && clientID < TCP_MAX_CLIENTS);
}

//--------------------------
bool ofxTCPServer::isClientConnected(int clientID){
	return isClientSetup(clientID) && TCPConnections[clientID].isConnected();
}

//don't call this
//--------------------------
void ofxTCPServer::threadedFunction(){

	while( isThreadRunning() ){

		if(count == TCP_MAX_CLIENTS){
			if(verbose)printf("ofxTCPServer: reached max connected clients! \nofxTCPServer: no more connections accepted\n");
			break;
		}

		if( !TCPServer.Listen(TCP_MAX_CLIENTS) ){
			if(verbose)printf("ofxTCPServer: Listen() failed\n");
		}

		if( !TCPServer.Accept(TCPConnections[count].TCPClient) ){
			if(verbose)printf("ofxTCPServer: Accept() failed\n");
			continue;
		}else{
			TCPConnections[count].setup(count, bClientBlocking);
			count++;
			if(verbose)printf("ofxTCPServer: client %i connected on port %i\n", count, TCPConnections[count].getPort());
		}
	}
	if(verbose)printf("ofxTCPServer: listen thread ended\n");
}






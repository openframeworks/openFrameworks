#include "ofxTCPServer.h"
#include "ofxTCPClient.h"

//--------------------------
ofxTCPServer::ofxTCPServer(){
	verbose		= true;
	connected	= false;
	idCount		= 0;
	port		= 0;
	str			= "";
	strEndMsg = "[/TCP]";
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
void ofxTCPServer::setStrEndMsg(string message){
	if(message != ""){
		strEndMsg = message;
	}
}

//--------------------------
bool ofxTCPServer::close(){

	if(!connected) return true;

	map<int,ofxTCPClient>::iterator it;
	for(it=TCPConnections.begin(); it!=TCPConnections.end(); it++){
		it->second.close();
	}
	TCPConnections.clear();

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
		TCPConnections.erase(clientID);
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
		if(!TCPConnections[clientID].isConnected()) TCPConnections.erase(clientID);
		return true;
	}
}

//--------------------------
bool ofxTCPServer::sendToAll(string message){
	if(TCPConnections.size() == 0) return false;

	map<int,ofxTCPClient>::iterator it;
	vector<int> disconnect;
	for(it=TCPConnections.begin(); it!=TCPConnections.end(); it++){
	    int err = 0;
		if(it->second.isConnected()) err = it->second.send(message);
		if(!TCPConnections[it->first].isConnected()) disconnect.push_back(it->first);
	}
	for(int i=0; i<(int)disconnect.size(); i++){
    	TCPConnections.erase(disconnect[i]);
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
	if(TCPConnections.size() == 0 || numBytes <= 0) return false;

	map<int,ofxTCPClient>::iterator it;
	for(it=TCPConnections.begin(); it!=TCPConnections.end(); it++){
		if(it->second.isConnected())it->second.sendRawBytes(rawBytes, numBytes);
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
	return TCPConnections.size()-1;
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
	return TCPConnections.find(clientID)!=TCPConnections.end();
}

//--------------------------
bool ofxTCPServer::isClientConnected(int clientID){
	return isClientSetup(clientID) && TCPConnections[clientID].isConnected();
}

//don't call this
//--------------------------
void ofxTCPServer::threadedFunction(){

	while( isThreadRunning() ){

		if(TCPConnections.size() == TCP_MAX_CLIENTS){
			if(verbose)printf("ofxTCPServer: reached max connected clients! \nofxTCPServer: no more connections accepted\n");
			break;
		}

		if( !TCPServer.Listen(TCP_MAX_CLIENTS) ){
			if(verbose)printf("ofxTCPServer: Listen() failed\n");
		}


		if( !TCPServer.Accept(TCPConnections[idCount].TCPClient) ){
			if(verbose)printf("ofxTCPServer: Accept() failed\n");
			continue;
		}else{
			TCPConnections[idCount].setup(idCount, bClientBlocking);
			TCPConnections[idCount].setStrEndMsg(strEndMsg);
			if(verbose)printf("ofxTCPServer: client %i connected on port %i\n", idCount, TCPConnections[idCount].getPort());
			idCount++;
		}
	}
	if(verbose)printf("ofxTCPServer: listen thread ended\n");
}






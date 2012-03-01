#include "ofxTCPServer.h"
#include "ofxTCPClient.h"
#include "ofUtils.h"

//--------------------------
ofxTCPServer::ofxTCPServer(){
	connected	= false;
	idCount		= 0;
	port		= 0;
	str			= "";
	messageDelimiter = "[/TCP]";
}

//--------------------------
ofxTCPServer::~ofxTCPServer(){
	close();
}

//--------------------------
void ofxTCPServer::setVerbose(bool _verbose){
	ofLog(OF_LOG_WARNING, "ofxTCPClient: setVerbose is deprecated replaced for OF_LOG_WARNING and OF_LOG_ERROR");
}

//--------------------------
bool ofxTCPServer::setup(int _port, bool blocking){
	if( !TCPServer.Create() ){
		ofLog(OF_LOG_ERROR,"ofxTCPServer: create() failed");
		return false;
	}
	if( !TCPServer.Bind(_port) ){
		ofLog(OF_LOG_ERROR,"ofxTCPServer: bind(port = " + ofToString(_port) + ") failed");
		return false;
	}

	connected		= true;
	port			= _port;
	bClientBlocking = blocking;

	startThread(true, false);
	return true;
}

//--------------------------
void ofxTCPServer::setMessageDelimiter(string delim){
	if(delim != ""){
		messageDelimiter = delim;
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
		ofLog(OF_LOG_WARNING, "ofxTCPServer: unable to close connection");
		return false;
	}else{
		connected = false;
		return true;
	}
}

//--------------------------
bool ofxTCPServer::disconnectClient(int clientID){
	if( !isClientSetup(clientID) ){
		ofLog(OF_LOG_WARNING, "ofxTCPServer: client " + ofToString(clientID) + " doesn't exist");
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
		ofLog(OF_LOG_WARNING, "ofxTCPServer: client " + ofToString(clientID) + " doesn't exist\n");
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
		ofLog(OF_LOG_WARNING, "ofxTCPServer: client " + ofToString(clientID) + " doesn't exist");
		return "client doesn't exist";
	}
	
	if( !TCPConnections[clientID].isConnected() ){
		disconnectClient(clientID);
		return "";
	}

	return TCPConnections[clientID].receive();
}

//--------------------------
bool ofxTCPServer::sendRawBytes(int clientID, const char * rawBytes, const int numBytes){
	if( !isClientSetup(clientID) ){
		ofLog(OF_LOG_WARNING, "ofxTCPServer: client " + ofToString(clientID)+ " doesn't exist");
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
		ofLog(OF_LOG_WARNING, "ofxTCPServer: client " + ofToString(clientID)+ " doesn't exist");
		return 0;
	}

	return TCPConnections[clientID].getNumReceivedBytes();
}

//--------------------------
int ofxTCPServer::receiveRawBytes(int clientID, char * receiveBytes,  int numBytes){
	if( !isClientSetup(clientID) ){
		ofLog(OF_LOG_WARNING, "ofxTCPServer: client " + ofToString(clientID) + " doesn't exist");
		return 0;
	}

	return TCPConnections[clientID].receiveRawBytes(receiveBytes, numBytes);
}

//--------------------------
int ofxTCPServer::getClientPort(int clientID){
	if( !isClientSetup(clientID) ){
		ofLog(OF_LOG_WARNING, "ofxTCPServer: client " + ofToString(clientID)+ " doesn't exist");
		return 0;
	}
	else return TCPConnections[clientID].getPort();
}

//--------------------------
string ofxTCPServer::getClientIP(int clientID){
	if( !isClientSetup(clientID) ){
		ofLog(OF_LOG_WARNING, "ofxTCPServer: client " + ofToString(clientID) + " doesn't exist");
		return "000.000.000.000";
	}
	else return TCPConnections[clientID].getIP();
}

//--------------------------
int ofxTCPServer::getNumClients(){
	return TCPConnections.size()-1;
}

//--------------------------
int ofxTCPServer::getLastID(){
	return idCount;
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
		
		int acceptId;
		for(acceptId = 0; acceptId <= idCount; acceptId++){
			if(!isClientConnected(acceptId)) break;
		}
		
		if(acceptId == TCP_MAX_CLIENTS){
			ofLog(OF_LOG_WARNING, "ofxTCPServer: reached max connected clients! \nofxTCPServer: no more connections accepted");
			break;
		}

		if( !TCPServer.Listen(TCP_MAX_CLIENTS) ){
			ofLog(OF_LOG_ERROR, "ofxTCPServer: Listen() failed");
		}
		
		if( !TCPServer.Accept(TCPConnections[acceptId].TCPClient) ){
			ofLog(OF_LOG_ERROR, "ofxTCPServer: Accept() failed\n");
		}else{
			TCPConnections[acceptId].setup(acceptId, bClientBlocking);
			TCPConnections[acceptId].setMessageDelimiter(messageDelimiter);
			ofLog(OF_LOG_VERBOSE, "ofxTCPServer: client " + ofToString(acceptId) + " connected on port " + ofToString(TCPConnections[acceptId].getPort()) );
			if(acceptId == idCount) idCount++;
		}
	}
	ofLog(OF_LOG_VERBOSE, "ofxTCPServer: listen thread ended");
}






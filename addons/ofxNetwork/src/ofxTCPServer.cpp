#include "ofxTCPServer.h"
#include "ofxTCPClient.h"
#include "ofUtils.h"
#include "ofLog.h"

//--------------------------
ofxTCPServer::ofxTCPServer(){
	connected	= false;
	idCount		= 0;
	port		= 0;
	str			= "";
	messageDelimiter = "[/TCP]";
	bClientBlocking = false;
}

//--------------------------
ofxTCPServer::~ofxTCPServer(){
	close();
}

//--------------------------
void ofxTCPServer::setVerbose(bool _verbose){
	ofLogWarning("ofxTCPServer") << "setVerbose(): is deprecated, replaced by ofLogWarning and ofLogError";
}

//--------------------------
bool ofxTCPServer::setup(int _port, bool blocking){
	ofxTCPSettings settings(_port);

	settings.blocking = blocking;

	return setup(settings);
}


//--------------------------
bool ofxTCPServer::setup(const ofxTCPSettings &settings){
	if( !TCPServer.Create() ){
		ofLogError("ofxTCPServer") << "setup(): couldn't create server";
		return false;
	}
	if( !TCPServer.Bind(settings.port, settings.reuse) ){
		ofLogError("ofxTCPServer") << "setup(): couldn't bind to port " << settings.port;
		return false;
	}

	connected		= true;
	port           	= settings.port;
	bClientBlocking	= settings.blocking;

	setMessageDelimiter(settings.messageDelimiter);

	std::unique_lock<std::mutex> lck(mConnectionsLock);
	startThread();
    serverReady.wait(lck);

	return true;
}

//--------------------------
void ofxTCPServer::setMessageDelimiter(std::string delim){
	if(delim != ""){
		messageDelimiter = delim;
	}
}

//--------------------------
bool ofxTCPServer::close(){
    stopThread();
	if( !TCPServer.Close() ){
		ofLogWarning("ofxTCPServer") << "close(): couldn't close connections";
		waitForThread(false); // wait for the thread to finish
		return false;
    }else{
        ofLogVerbose("ofxTCPServer") << "Closing server";
		waitForThread(false); // wait for the thread to finish
		return true;
	}
}

ofxTCPClient & ofxTCPServer::getClient(int clientID){
	return *TCPConnections.find(clientID)->second;
}

//--------------------------
bool ofxTCPServer::disconnectClient(int clientID){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if( !isClientSetup(clientID) ){
		ofLogWarning("ofxTCPServer") << "disconnectClient(): client " << clientID << " doesn't exist";
		return false;
	}else if(getClient(clientID).close()){
		TCPConnections.erase(clientID);
		return true;
	}
	return false;
}

//--------------------------
bool ofxTCPServer::disconnectAllClients(){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
    TCPConnections.clear();
    return true;
}

//--------------------------
bool ofxTCPServer::send(int clientID, std::string message){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if( !isClientSetup(clientID) ){
		ofLogWarning("ofxTCPServer") << "send(): client " << clientID << " doesn't exist";
		return false;
	}else{
        auto ret = getClient(clientID).send(message);
		if(!getClient(clientID).isConnected()) TCPConnections.erase(clientID);
        return ret;
	}
}

//--------------------------
bool ofxTCPServer::sendToAll(std::string message){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if(TCPConnections.size() == 0) return false;

	std::vector<int> disconnect;
	for(auto & conn: TCPConnections){
		if(conn.second->isConnected()) conn.second->send(message);
		else disconnect.push_back(conn.first);
	}
	for(int i=0; i<(int)disconnect.size(); i++){
    	TCPConnections.erase(disconnect[i]);
    }
	return true;
}

//--------------------------
std::string ofxTCPServer::receive(int clientID){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if( !isClientSetup(clientID) ){
		ofLogWarning("ofxTCPServer") << "receive(): client " << clientID << " doesn't exist";
		return "client " + ofToString(clientID) + "doesn't exist";
	}
	
	if( !getClient(clientID).isConnected() ){
        TCPConnections.erase(clientID);
		return "";
	}

	return getClient(clientID).receive();
}

//--------------------------
bool ofxTCPServer::sendRawBytes(int clientID, const char * rawBytes, const int numBytes){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if( !isClientSetup(clientID) ){
		ofLogWarning("ofxTCPServer") << "sendRawBytes(): client " << clientID << " doesn't exist";
		
		return false;
	}
	else{
		return getClient(clientID).sendRawBytes(rawBytes, numBytes);
	}
}

//--------------------------
bool ofxTCPServer::sendRawBytesToAll(const char * rawBytes, const int numBytes){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if(TCPConnections.size() == 0 || numBytes <= 0) return false;

	for(auto & conn: TCPConnections){
		if(conn.second->isConnected()){
			conn.second->sendRawBytes(rawBytes, numBytes);
		}
	}
	return true;
}


//--------------------------
bool ofxTCPServer::sendRawMsg(int clientID, const char * rawBytes, const int numBytes){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if( !isClientSetup(clientID) ){
		ofLogWarning("ofxTCPServer") << "sendRawMsg(): client " << clientID << " doesn't exist";
		return false;
	}
	else{
		return getClient(clientID).sendRawMsg(rawBytes, numBytes);
	}
}

//--------------------------
bool ofxTCPServer::sendRawMsgToAll(const char * rawBytes, const int numBytes){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if(TCPConnections.empty() || numBytes <= 0) return false;

	for(auto & conn: TCPConnections){
		if(conn.second->isConnected()){
			conn.second->sendRawMsg(rawBytes, numBytes);
		}
	}
	return true;
}

//--------------------------
int ofxTCPServer::getNumReceivedBytes(int clientID){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if( !isClientSetup(clientID) ){
		ofLogWarning("ofxTCPServer") << "getNumReceivedBytes(): client " << clientID << " doesn't exist";
		return 0;
	}

	return getClient(clientID).getNumReceivedBytes();
}

//--------------------------
int ofxTCPServer::receiveRawBytes(int clientID, char * receiveBytes,  int numBytes){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if( !isClientSetup(clientID) ){
		ofLogWarning("ofxTCPServer") << "receiveRawBytes(): client " << clientID << " doesn't exist";
		return 0;
	}

	return getClient(clientID).receiveRawBytes(receiveBytes, numBytes);
}


//--------------------------
int ofxTCPServer::peekReceiveRawBytes(int clientID, char * receiveBytes,  int numBytes){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if( !isClientSetup(clientID) ){
		ofLog(OF_LOG_WARNING, "ofxTCPServer: client " + ofToString(clientID) + " doesn't exist");
		return 0;
	}

	return getClient(clientID).peekReceiveRawBytes(receiveBytes, numBytes);
}

//--------------------------
int ofxTCPServer::receiveRawMsg(int clientID, char * receiveBytes,  int numBytes){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if( !isClientSetup(clientID) ){
		ofLogWarning("ofxTCPServer") << "receiveRawMsg(): client " << clientID << " doesn't exist";
		return 0;
	}

	return getClient(clientID).receiveRawMsg(receiveBytes, numBytes);
}

//--------------------------
int ofxTCPServer::getClientPort(int clientID){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if( !isClientSetup(clientID) ){
		ofLogWarning("ofxTCPServer") << "getClientPort(): client " << clientID << " doesn't exist";
		return 0;
	}
	else return getClient(clientID).getPort();
}

//--------------------------
std::string ofxTCPServer::getClientIP(int clientID){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if( !isClientSetup(clientID) ){
		ofLogWarning("ofxTCPServer") << "getClientIP(): client " << clientID << " doesn't exist";
		return "000.000.000.000";
	}
	else return getClient(clientID).getIP();
}

//--------------------------
int ofxTCPServer::getNumClients(){
	return TCPConnections.size();
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
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	return isClientSetup(clientID) && getClient(clientID).isConnected();
}


void ofxTCPServer::waitConnectedClient(){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if(TCPConnections.empty()){
		serverReady.wait(lck);
	}
}

void ofxTCPServer::waitConnectedClient(int ms){
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	if(TCPConnections.empty()){
		serverReady.wait_for(lck, std::chrono::milliseconds(ms));
	}
}

//don't call this
//--------------------------
void ofxTCPServer::threadedFunction(){

	ofLogVerbose("ofxTCPServer") << "listening thread started";
	while( isThreadRunning() ){
		
		int acceptId;
		for(acceptId = 0; acceptId <= idCount; acceptId++){
			if(!isClientConnected(acceptId)) break;
		}
		
		if(acceptId == TCP_MAX_CLIENTS){
			ofLogWarning("ofxTCPServer") << "no longer accepting connections, maximum number of clients reached: " << TCP_MAX_CLIENTS;
			break;
		}

		if( !TCPServer.Listen(TCP_MAX_CLIENTS) ){
			if(isThreadRunning()) ofLogError("ofxTCPServer") << "listening failed";
		}

        {
			std::unique_lock<std::mutex> lck( mConnectionsLock );
            serverReady.notify_one();
        }
		
		//	we need to lock here, but can't as it blocks...
		//	so use a temporary to not block the lock 
		std::shared_ptr<ofxTCPClient> client(new ofxTCPClient);
		if( !TCPServer.Accept( client->TCPClient ) ){
			if(isThreadRunning()) ofLogError("ofxTCPServer") << "couldn't accept client " << acceptId;
		}else{
			std::unique_lock<std::mutex> lck( mConnectionsLock );
			//	take owenership of socket from NewClient
			TCPConnections[acceptId] = client;
            TCPConnections[acceptId]->setupConnectionIdx(acceptId, bClientBlocking);
			TCPConnections[acceptId]->setMessageDelimiter(messageDelimiter);
			ofLogVerbose("ofxTCPServer") << "client " << acceptId << " connected on port " << TCPConnections[acceptId]->getPort();
			if(acceptId == idCount) idCount++;
			serverReady.notify_all();
		}
	}
	idCount = 0;
	std::unique_lock<std::mutex> lck( mConnectionsLock );
	TCPConnections.clear();
	connected = false;
	ofLogVerbose("ofxTCPServer") << "listening thread stopped";
}






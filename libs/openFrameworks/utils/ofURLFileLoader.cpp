#include "ofURLFileLoader.h"
#include "ofBaseTypes.h"
#include "ofAppRunner.h"
#include "ofUtils.h"

#include "ofConstants.h"

#ifndef TARGET_IMPLEMENTS_URL_LOADER
	#include "Poco/Net/HTTPSession.h"
	#include "Poco/Net/HTTPClientSession.h"
	#include "Poco/Net/HTTPSClientSession.h"
	#include "Poco/Net/HTTPRequest.h"
	#include "Poco/Net/HTTPResponse.h"
	#include "Poco/StreamCopier.h"
	#include "Poco/Path.h"
	#include "Poco/URI.h"
	#include "Poco/Exception.h"
	#include "Poco/URIStreamOpener.h"
	#include "Poco/Net/HTTPStreamFactory.h"
	#include "Poco/Net/HTTPSStreamFactory.h"
	#include "Poco/Net/SSLManager.h"
	#include "Poco/Net/KeyConsoleHandler.h"
	#include "Poco/Net/ConsoleCertificateHandler.h"

	#include "ofThreadChannel.h"

	#include "ofThread.h"

	using namespace Poco::Net;
	using namespace Poco;

	static bool factoryLoaded = false;
#endif

int	ofHttpRequest::nextID = 0;

ofEvent<ofHttpResponse> & ofURLResponseEvent(){
	static ofEvent<ofHttpResponse> * event = new ofEvent<ofHttpResponse>;
	return *event;
}

#ifndef TARGET_IMPLEMENTS_URL_LOADER
class ofURLFileLoaderImpl: public ofThread, public ofBaseURLFileLoader{
public:
	ofURLFileLoaderImpl();
    ofHttpResponse get(const string& url);
    int getAsync(const string& url, const string& name=""); // returns id
    ofHttpResponse saveTo(const string& url, const std::filesystem::path& path);
    int saveAsync(const string& url, const std::filesystem::path& path);
	void remove(int id);
	void clear();
    void stop();
    ofHttpResponse handleRequest(const ofHttpRequest & request);
    int handleRequestAsync(const ofHttpRequest& request); // returns id

protected:
	// threading -----------------------------------------------
	void threadedFunction();
    void start();
    void update(ofEventArgs & args);  // notify in update so the notification is thread safe

private:
	// perform the requests on the thread

	ofThreadChannel<ofHttpRequest> requests;
	ofThreadChannel<ofHttpResponse> responses;
	ofThreadChannel<int> cancelRequestQueue;
	set<int> cancelledRequests;
};

ofURLFileLoaderImpl::ofURLFileLoaderImpl() {
	if(!factoryLoaded){
		try {
			HTTPStreamFactory::registerFactory();
			HTTPSStreamFactory::registerFactory();
			SharedPtr<PrivateKeyPassphraseHandler> pConsoleHandler = new KeyConsoleHandler(false);
			SharedPtr<InvalidCertificateHandler> pInvalidCertHandler = new ConsoleCertificateHandler(true);
			Context::Ptr pContext = new Context(Context::CLIENT_USE, "", Context::VERIFY_NONE);
			SSLManager::instance().initializeClient(pConsoleHandler, pInvalidCertHandler, pContext);
			factoryLoaded = true;
		}
		catch (Poco::SystemException & PS) {
			ofLogError("ofURLFileLoader") << "couldn't create factory: " << PS.displayText();
		}
		catch (Poco::ExistsException & PS) {
			ofLogError("ofURLFileLoader") << "couldn't create factory: " << PS.displayText();
		}
	}
}

ofHttpResponse ofURLFileLoaderImpl::get(const string& url) {
    ofHttpRequest request(url,url);
    return handleRequest(request);
}


int ofURLFileLoaderImpl::getAsync(const string& url, const string& name){
    ofHttpRequest request(url, name.empty() ? url : name);
    requests.send(request);
    start();
    return request.getId();
}


ofHttpResponse ofURLFileLoaderImpl::saveTo(const string& url, const std::filesystem::path& path){
    ofHttpRequest request(url,path.string(),true);
    return handleRequest(request);
}

int ofURLFileLoaderImpl::saveAsync(const string& url, const std::filesystem::path& path){
    ofHttpRequest request(url,path.string(),true);
	requests.send(request);
	start();
	return request.getId();
}

void ofURLFileLoaderImpl::remove(int id){
	cancelRequestQueue.send(id);
}

void ofURLFileLoaderImpl::clear(){
	ofHttpResponse resp;
	ofHttpRequest req;
	while(requests.tryReceive(req)){}
	while(responses.tryReceive(resp)){}
}

void ofURLFileLoaderImpl::start() {
     if (!isThreadRunning()){
		ofAddListener(ofEvents().update,this,&ofURLFileLoaderImpl::update);
        startThread();
    }
}

void ofURLFileLoaderImpl::stop() {
    stopThread();
    requests.close();
    responses.close();
    waitForThread();
}

void ofURLFileLoaderImpl::threadedFunction() {
	setThreadName("ofURLFileLoader " + ofToString(getThreadId()));
	while( isThreadRunning() ){
		int cancelled;
		while(cancelRequestQueue.tryReceive(cancelled)){
			cancelledRequests.insert(cancelled);
		}
		ofHttpRequest request;
		if(requests.receive(request)){
			if(cancelledRequests.find(request.getId())==cancelledRequests.end()){
				ofHttpResponse response(handleRequest(request));
				int status = response.status;
				if(!responses.send(move(response))){
					break;
				}
				if(status==-1){
					// retry
					requests.send(request);
				}
			}else{
				cancelledRequests.erase(cancelled);
			}
		}else{
			break;
		}
	}
}

ofHttpResponse ofURLFileLoaderImpl::handleRequest(const ofHttpRequest & request) {
	try {
		URI uri(request.url);
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";
		std::string pocoMethod;
		if(request.method==ofHttpRequest::GET){
			pocoMethod = HTTPRequest::HTTP_GET;
		}else{
			pocoMethod = HTTPRequest::HTTP_POST;
		}
		HTTPRequest req(pocoMethod, path, HTTPMessage::HTTP_1_1);
        for(map<string,string>::const_iterator it = request.headers.cbegin(); it!=request.headers.cend(); it++){
			req.add(it->first,it->second);
		}
		HTTPResponse res;
		std::unique_ptr<HTTPClientSession> session;
		if(uri.getScheme()=="https"){
			 //const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "rootcert.pem" ) );
			session.reset(new HTTPSClientSession(uri.getHost(), uri.getPort()));//,context);
		}else{
			session.reset(new HTTPClientSession(uri.getHost(), uri.getPort()));
		}
		session->setTimeout(Poco::Timespan(120,0));
		if(request.contentType!=""){
			req.setContentType(request.contentType);
		}
		if(request.body!=""){
			req.setContentLength( request.body.length() );
			auto & send = session->sendRequest(req);
			send.write(request.body.c_str(), request.body.size());
			send << std::flush;
		}else{
			session->sendRequest(req);
		}

		auto & rs = session->receiveResponse(res);
		if(!request.saveTo){
			return ofHttpResponse(request,rs,res.getStatus(),res.getReason());
		}else{
			ofFile saveTo(request.name,ofFile::WriteOnly,true);
			char aux_buffer[1024];
			rs.read(aux_buffer, 1024);
			std::streamsize n = rs.gcount();
			while (n > 0){
				// we resize to size+1 initialized to 0 to have a 0 at the end for strings
				saveTo.write(aux_buffer,n);
				if (rs.good()){
					rs.read(aux_buffer, 1024);
					n = rs.gcount();
				}
				else n = 0;
			}
			return ofHttpResponse(request,res.getStatus(),res.getReason());
		}

	} catch (const Exception& exc) {
        ofLogError("ofURLFileLoader") << "handleRequest(): "+ exc.displayText();

        return ofHttpResponse(request,-1,exc.displayText());

    } catch (...) {
    	return ofHttpResponse(request,-1,"ofURLFileLoader: fatal error, couldn't catch Exception");
    }

	return ofHttpResponse(request,-1,"ofURLFileLoader: fatal error, couldn't catch Exception");
	
}


int ofURLFileLoaderImpl::handleRequestAsync(const ofHttpRequest& request){
	requests.send(request);
	start();
	return request.getId();
}

void ofURLFileLoaderImpl::update(ofEventArgs & args){
	ofHttpResponse response;
	while(responses.tryReceive(response)){
		ofNotifyEvent(ofURLResponseEvent(),response);
	}

}

ofURLFileLoader::ofURLFileLoader()
:impl(new ofURLFileLoaderImpl){}

#elif defined(TARGET_EMSCRIPTEN)
#include "ofxEmscriptenURLFileLoader.h"
ofURLFileLoader::ofURLFileLoader()
:impl(new ofxEmscriptenURLFileLoader){}
#endif

ofHttpResponse ofURLFileLoader::get(const string& url){
	return impl->get(url);
}

int ofURLFileLoader::getAsync(const string& url, const string& name){
	return impl->getAsync(url,name);
}

ofHttpResponse ofURLFileLoader::saveTo(const string& url, const std::filesystem::path & path){
	return impl->saveTo(url,path);
}

int ofURLFileLoader::saveAsync(const string& url, const std::filesystem::path & path){
	return impl->saveAsync(url,path);
}

void ofURLFileLoader::remove(int id){
	impl->remove(id);
}

void ofURLFileLoader::clear(){
	impl->clear();
}

void ofURLFileLoader::stop(){
	impl->stop();
}

ofHttpResponse ofURLFileLoader::handleRequest(const ofHttpRequest & request){
	return impl->handleRequest(request);
}

int ofURLFileLoader::handleRequestAsync(const ofHttpRequest& request){
	return impl->handleRequestAsync(request);
}

static bool initialized = false;
static ofURLFileLoader & getFileLoader(){
	static ofURLFileLoader * fileLoader = new ofURLFileLoader;
	initialized = true;
	return *fileLoader;
}


ofHttpRequest::ofHttpRequest()
:saveTo(false)
,method(GET)
,id(nextID++)
{
}

ofHttpRequest::ofHttpRequest(const string& url, const string& name,bool saveTo)
:url(url)
,name(name)
,saveTo(saveTo)
,method(GET)
,id(nextID++)
{
}

int ofHttpRequest::getId() const {
	return id;
}

int ofHttpRequest::getID(){
	return id;
}


ofHttpResponse::ofHttpResponse()
:status(0)
{
}

ofHttpResponse::ofHttpResponse(const ofHttpRequest& request, const ofBuffer& data, int status, const string& error)
:request(request)
,data(data)
,status(status)
,error(error)
{
}

ofHttpResponse::ofHttpResponse(const ofHttpRequest& request, int status, const string& error)
:request(request)
,status(status)
,error(error)
{
}

ofHttpResponse::operator ofBuffer&(){
	return data;
}



ofHttpResponse ofLoadURL(const string& url){
	return getFileLoader().get(url);
}

int ofLoadURLAsync(const string&  url, const string&  name){
	return getFileLoader().getAsync(url,name);
}

ofHttpResponse ofSaveURLTo(const string& url, const string& path){
	return getFileLoader().saveTo(url,path);
}

int ofSaveURLAsync(const string& url, const string& path){
	return getFileLoader().saveAsync(url,path);
}

void ofRemoveURLRequest(int id){
	getFileLoader().remove(id);
}

void ofRemoveAllURLRequests(){
	getFileLoader().clear();
}

void ofStopURLLoader(){
	getFileLoader().stop();
}

void ofURLFileLoaderShutdown(){
	if(initialized){
		ofRemoveAllURLRequests();
		ofStopURLLoader();
		#ifndef TARGET_IMPLEMENTS_URL_LOADER
			Poco::Net::uninitializeSSL();
		#endif
	}
}

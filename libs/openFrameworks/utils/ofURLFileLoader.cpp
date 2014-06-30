#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/FilePartSource.h"
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
#include "Poco/Net/HTTPRequest.h"
#include "ofURLFileLoader.h"
#include "ofAppRunner.h"
#include "ofUtils.h"

using namespace Poco::Net;

using namespace Poco;

#include "ofConstants.h"



static bool factoryLoaded = false;
int ofHttpRequest::nextID = 0;



ofEvent<ofHttpResponse> & ofURLResponseEvent(){
    static ofEvent<ofHttpResponse> * event = new ofEvent<ofHttpResponse>;
    return *event;
}

ofURLFileLoader::ofURLFileLoader() {
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
    }
}

ofHttpResponse ofURLFileLoader::get(string url) {
    ofHttpRequest request(url,url);
    request.type = OF_HTTP_GET;
    return handleRequest(request);
}

ofHttpResponse ofURLFileLoader::get(ofHttpRequest request){
    return handleRequest(request);
}


int ofURLFileLoader::getAsync(string url, string name){
    if(name=="") name=url;
    ofHttpRequest request(url,name);
    request.type = OF_HTTP_GET;
    lock();
    requests.push_back(request);
    unlock();
    start();
    return request.getID();
}

int ofURLFileLoader::getAsync(ofHttpRequest req){
    if(req.url != ""){
        if(req.name=="") req.name = req.url;
        lock();
        requests.push_back(req);
        unlock();
        start();
        return req.getID();
    }
    return -1;
}

ofHttpResponse ofURLFileLoader::saveTo(string url, string path){
    ofHttpRequest request(url,path,true);
    return handleRequest(request);
}

int ofURLFileLoader::saveAsync(string url, string path){
    ofHttpRequest request(url,path,true);
    lock();
    requests.push_back(request);
    unlock();
    start();
    return request.getID();
}

void ofURLFileLoader::remove(int id){
    Poco::ScopedLock<ofMutex> lock(mutex);
    for(int i=0;i<(int)requests.size();i++){
        if(requests[i].getID()==id){
            requests.erase(requests.begin()+i);
            return;
        }
    }
    ofLogError("ofURLFileLoader") << "remove(): request " <<  id << " not found";
}

void ofURLFileLoader::clear(){
    Poco::ScopedLock<ofMutex> lock(mutex);
    requests.clear();
    while(!responses.empty()) responses.pop();
}

void ofURLFileLoader::start() {
     if (isThreadRunning() == false){
		ofAddListener(ofEvents().update,this,&ofURLFileLoader::update);
        startThread(true);
    }else{
        ofLogVerbose("ofURLFileLoader") << "start(): signaling new request condition";
        condition.signal();
    }
}

void ofURLFileLoader::stop() {
    lock();
    stopThread();
    condition.signal();
    unlock();
    waitForThread();
}

void ofURLFileLoader::threadedFunction() {
    ofLogVerbose("ofURLFileLoader") << "threadedFunction(): starting thread";
    lock();
    while( isThreadRunning() == true ){
        if(requests.size()>0){
            ofLogVerbose("ofURLFileLoader") << "threadedFunction(): querying request " << requests.front().name;
            ofHttpRequest request(requests.front());
            unlock();
            
            ofHttpResponse response(handleRequest(request));
            
            lock();
            if(response.status!=-1){
                // double-check that the request hasn't been removed from the queue
                if( (requests.size()==0) || (requests.front().getID()!=request.getID()) ){
                    // this request has been removed from the queue
                    ofLogVerbose("ofURLFileLoader") << "threadedFunction(): request " << requests.front().name
                    << " is missing from the queue, must have been removed/cancelled";
                }
                else{
                    ofLogVerbose("ofURLFileLoader") << "threadedFunction(): got response to request "
                    << requests.front().name << " status " <<response.status;
                    responses.push(response);
                    requests.pop_front();
                }
            }else{
                responses.push(response);
                ofLogVerbose("ofURLFileLoader") << "threadedFunction(): failed getting request " << requests.front().name;
            }
        }else{
            ofLogVerbose("ofURLFileLoader") << "threadedFunction(): stopping on no requests condition";
            condition.wait(mutex);
        }
    }
}

ofHttpResponse ofURLFileLoader::handleRequest(ofHttpRequest request) {
    try {
        URI uri;
        if(request.redirect)
            uri = URI(request.redirecturl);
        else
            uri = URI(request.url);

        std::string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";
        
        string method = "";
        if(request.type == OF_HTTP_GET){
            method = HTTPRequest::HTTP_GET;
        }
        if(request.type == OF_HTTP_PUT){
            method = HTTPRequest::HTTP_PUT;
        }
        if(request.type == OF_HTTP_POST){
            method = HTTPRequest::HTTP_POST;
        }
        if(request.type == OF_HTTP_DELETE){
            method = HTTPRequest::HTTP_DELETE;
        }
        if(method == ""){
            method = HTTPRequest::HTTP_GET;
        }
        
        
        HTTPRequest req(method, path, HTTPMessage::HTTP_1_1);

        bool usesForm = false;
        if(request.header.size() > 0){
            map<string, string>::iterator iter;
            for(iter = request.header.begin(); iter != request.header.end(); iter++){
                req.add(iter->first, iter->second);
            }
        }
        
        if(request.cookies.size() > 0){
            NameValueCollection cookies;
            map<string, string>::iterator cookieIter;
            for(cookieIter = request.cookies.begin(); cookieIter != request.cookies.end(); cookieIter++){
                cookies.add(cookieIter->first, cookieIter->second);
            }
            req.setCookies(cookies);
        }
        
        HTMLForm form;
        if(request.files.size() > 0 || request.data.size() > 0){
            usesForm = true;
            // create the form data to send
            if(request.files.size()>0){
                form.setEncoding("multipart/form-data");
            }else{
                form.setEncoding(HTMLForm::ENCODING_URL);
            }
            map<string, string>::iterator dataIter;
            for(dataIter = request.data.begin(); dataIter != request.data.end(); dataIter++){
                form.add(dataIter->first, dataIter->second);
            }
            map<string, string>::iterator fileIter;
            for(fileIter = request.files.begin(); fileIter!=request.files.end(); fileIter++){
                string fileName = fileIter->second;
                cout<<fileName<<endl;
                form.addPart(fileIter->first,new FilePartSource(fileIter->second));
            }
            
            form.prepareSubmit(req);
        }else{
            usesForm = false;
        }
        
        
        HTTPResponse res;
        ofPtr<HTTPSession> session;
        istream * rs;
        if(uri.getScheme()=="https"){
            //const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "rootcert.pem" ) );
            HTTPSClientSession * httpsSession = new HTTPSClientSession(uri.getHost(), uri.getPort());//,context);
            httpsSession->setTimeout(Poco::Timespan(20,0));
            if(usesForm)
                form.write(httpsSession->sendRequest(req));
            else
                httpsSession->sendRequest(req);
            
            rs = &httpsSession->receiveResponse(res);
            session = ofPtr<HTTPSession>(httpsSession);
        }else{
            HTTPClientSession * httpSession = new HTTPClientSession(uri.getHost(), uri.getPort());
            httpSession->setTimeout(Poco::Timespan(30,0));
            if(usesForm)
                form.write(httpSession->sendRequest(req));
            else
                httpSession->sendRequest(req);
            
            rs = &httpSession->receiveResponse(res);
            session = ofPtr<HTTPSession>(httpSession);
        }

        
        if(res.getStatus() > 300 && res.getStatus() < 400 && request.redirectCount < 10){
            cout<<"REDIRECT "<<endl;
            request.redirecturl = res.get("Location", "");
            request.redirect = true;
            request.redirectCount++;
            cout<<"REDIRECT "<<request.url<<endl;
            return handleRequest(request);
        }
        
        if(!request.saveTo){
            return ofHttpResponse(request,*rs,res.getStatus(),res.getReason());
        }else{
            ofFile saveTo(request.name,ofFile::WriteOnly,true);
            char aux_buffer[1024];
            rs->read(aux_buffer, 1024);
            std::streamsize n = rs->gcount();
            while (n > 0){
                // we resize to size+1 initialized to 0 to have a 0 at the end for strings
                saveTo.write(aux_buffer,n);
                if (rs->good()){
                    rs->read(aux_buffer, 1024);
                    n = rs->gcount();
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

void ofURLFileLoader::update(ofEventArgs & args){
    lock();
    while(!responses.empty()){
        ofHttpResponse response(responses.front());
        ofLogVerbose("ofURLLoader") << "update(): new response " << response.request.name;
        responses.pop();
        unlock();
        ofNotifyEvent(ofURLResponseEvent(),response);
        lock();
    }
    unlock();
    
}

bool initialized = false;
static ofURLFileLoader & getFileLoader(){
	static ofURLFileLoader * fileLoader = new ofURLFileLoader;
	initialized = true;
	return *fileLoader;

}

ofHttpResponse ofLoadURL(string url){
    return getFileLoader().get(url);
}

int ofLoadURLAsync(string url, string name){
    return getFileLoader().getAsync(url,name);
}

int ofLoadRequestAsync(ofHttpRequest request){
    return getFileLoader().getAsync(request);
}

ofHttpResponse ofLoadRequest(ofHttpRequest request){
    return getFileLoader().get(request);
}

ofHttpResponse ofSaveURLTo(string url, string path){
    return getFileLoader().saveTo(url,path);
}

int ofSaveURLAsync(string url, string path){
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
        Poco::Net::uninitializeSSL();
	}
}

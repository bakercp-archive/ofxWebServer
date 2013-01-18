#include "ofxWebServer.h"

//------------------------------------------------------------------------------
ofxWebServer::Settings::Settings() {
    
    host = "http://127.0.0.1";
    port = 8080;
    
    maxQueued            = 64;
    maxThreads           = 64;
    keepAlive            = true;
    maxKeepAliveRequests = 0; // 0 is unlimited
    keepAliveTimeout     = Timespan(10,0); // 10 seconds
    name                 = ""; // empty, will be auto generated
    softwareVersion      = "ofxWebServer/1.0";
    timeout              = Timespan(60,0); // 60 seconds
    threadIdleTime       = Timespan(10,0);
    threadPriority       = Thread::PRIO_NORMAL;
    
}

//------------------------------------------------------------------------------
ofxWebServer::ofxWebServer(Settings _settings) {
    
    settings = _settings;
    
    previousErrorHandler = NULL;
    errorHandler.setName(settings.name);
    
    ofAddListener(ofEvents().exit,this,&ofxWebServer::exit);
    
    server = NULL;
}

//------------------------------------------------------------------------------
ofxWebServer::~ofxWebServer() {
    if(server != NULL) {
        stop();
    }
    ofLogVerbose("ofxWebServer::~ofxWebServer") << "Server destroyed.";
}

void ofxWebServer::exit(ofEventArgs& args) {
    ofLogVerbose("ofxWebServer::exit") << "Waiting for serverhrom thread cleanup.";

    if(server != NULL) {
        stop();
    }
    // it is ok to unregister an item that is not currently registered
    // POCO's internal loop won't complain or return errors
    // POCO stores the delegates in a std::vector and iterates through
    // deleting and returning on match, and doing nothing on a no-match condition
    ofRemoveListener(ofEvents().exit,this,&ofxWebServer::exit);
    ofLogVerbose("ofxWebServer::exit") << "Exiting server.";
    
}

//------------------------------------------------------------------------------
void ofxWebServer::start(){
    if(server != NULL) {
        ofLogWarning("ofxWebServer::start") << "Server is already running.  Call stop() to stop.";
        return;
    }
    
    // all of these params are an attempt to make the server shut down VERY quickly.
    
    string serverName = settings.name.empty() ? settings.host + ":" + ofToString(settings.port) : settings.name;
    
    HTTPServerParams* serverParams = new HTTPServerParams();
    serverParams->setMaxQueued(settings.maxQueued);
    serverParams->setMaxThreads(settings.maxThreads);
    serverParams->setKeepAlive(settings.keepAlive);
    serverParams->setMaxKeepAliveRequests(settings.maxKeepAliveRequests);
    serverParams->setKeepAliveTimeout(settings.keepAliveTimeout);
    serverParams->setServerName(serverName);
    serverParams->setTimeout(settings.timeout);
    serverParams->setThreadIdleTime(settings.threadIdleTime);
    serverParams->setThreadPriority(settings.threadPriority);
    serverParams->setSoftwareVersion(settings.softwareVersion);
    
    // we use the default thread pool
    server = new HTTPServer(new ofxWebServerRouteManager(routes),
                            ServerSocket(settings.port),
                            serverParams);

    
    errorHandler.setName(serverName);
    previousErrorHandler = ErrorHandler::set(&errorHandler);
    
    // start the http server
    server->start();

}

//------------------------------------------------------------------------------
void ofxWebServer::stop() {
    if(server == NULL) {
        ofLogWarning("ofxWebServer::stop") << "Server is not running.  Call start() to start.";
        return;
    }

    server->stop();
    
    // wait for all threads in the thread pool
    ThreadPool::defaultPool().joinAll(); // we gotta wait for all of them ... ugh.
    
    ErrorHandler::set(previousErrorHandler);
    
    ofLogVerbose("ofxWebServer::stop") << "Server successfully shut down.";
    
    server = NULL;
}

//------------------------------------------------------------------------------
string ofxWebServer::getURL() const {
    stringstream ss;
    ss << settings.host << ":" << settings.port << "/";
    return ss.str();
}

//------------------------------------------------------------------------------
int ofxWebServer::getPort() const {
    return settings.port;
}

//------------------------------------------------------------------------------
void ofxWebServer::clearRoutes() {
    routes.clear();
}

//------------------------------------------------------------------------------
void ofxWebServer::addRoute(ofPtr<HTTPRequestHandlerFactory> handler) {
    routes.push_back(handler);
}

//------------------------------------------------------------------------------
void ofxWebServer::removeRoute(ofPtr<HTTPRequestHandlerFactory> handler) {
    vector< ofPtr<HTTPRequestHandlerFactory> >::iterator iter = routes.begin();
    while(iter != routes.end()) {
        if(*iter == handler) {
            routes.erase(iter);
        }
    }
}
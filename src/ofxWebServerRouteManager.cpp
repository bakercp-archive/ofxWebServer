#include "ofxWebServerRouteManager.h"

//------------------------------------------------------------------------------
ofxWebServerRouteManager::ofxWebServerRouteManager(vector< ofPtr<HTTPRequestHandlerFactory> >& _factories) : factories(_factories) { }

//------------------------------------------------------------------------------
ofxWebServerRouteManager::~ofxWebServerRouteManager() { }
    
//------------------------------------------------------------------------------
HTTPRequestHandler* ofxWebServerRouteManager::createRequestHandler(const HTTPServerRequest& request) {
    if(!factories.empty()) {
        vector< ofPtr<HTTPRequestHandlerFactory> >::reverse_iterator iter = factories.rbegin();
        // We start with the last factory that was added.
        while(iter != factories.rend()) {
            HTTPRequestHandler* handler = (*iter)->createRequestHandler(request);
            if(handler != NULL) {
                // If we were given a valid handler pointer, we can
                // return it (the server will own it and destroy it).
                return handler;
            } else {
                ++iter;
            }
        }
    } else {
        ofLogError("ofxWebServerRequestHandlerFactory::createRequestHandler") << "The handler factories list was empty.";
    }
    
    return new ofxWebServerFailedRouteHandler(); // if we get to this point, we got a fail.
}
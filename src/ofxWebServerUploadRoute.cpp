#include "ofxWebServerUploadRoute.h"

//------------------------------------------------------------------------------
ofxWebServerUploadRoute::ofxWebServerUploadRoute(const ofxWebServerUploadRouteHandler::Settings& _settings) : settings(_settings) {
    ofDirectory uploadsDirectory(settings.uploadFolder);
    if(!uploadsDirectory.exists()) {
        ofLogNotice("ofxWebServerUploadRoute::ofxWebServerUploadRoute") << "Uploads directory did not exist.  Creating: " << uploadsDirectory.getAbsolutePath();
        uploadsDirectory.create();
    }
}
    
//------------------------------------------------------------------------------
ofxWebServerUploadRoute::~ofxWebServerUploadRoute() { }
    
//------------------------------------------------------------------------------
HTTPRequestHandler* ofxWebServerUploadRoute::createRequestHandler(const HTTPServerRequest& request) {
    // this will respond to anything
    if(ofxWebServerUploadRouteHandler::matchRoute(URI(request.getURI()), settings.route)) {
        return new ofxWebServerUploadRouteHandler(settings);
    } else {
        return NULL;
    }
}

//------------------------------------------------------------------------------
ofxWebServerUploadRoute* ofxWebServerUploadRoute::Instance(const ofxWebServerUploadRouteHandler::Settings& settings) {
    return new ofxWebServerUploadRoute(settings);
}
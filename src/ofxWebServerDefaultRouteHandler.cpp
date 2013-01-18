#include "ofxWebServerDefaultRouteHandler.h"

//------------------------------------------------------------------------------
ofxWebServerDefaultRouteHandler::Settings::Settings() {
    defaultIndex = "index.html";
    route.path   = "/.*"; // regex
    
}

//------------------------------------------------------------------------------
ofxWebServerDefaultRouteHandler::ofxWebServerDefaultRouteHandler(const Settings& _settings) : settings(_settings) { }

//------------------------------------------------------------------------------
ofxWebServerDefaultRouteHandler::~ofxWebServerDefaultRouteHandler() { }

//------------------------------------------------------------------------------
void ofxWebServerDefaultRouteHandler::handleRequest(HTTPServerRequest& request,
                                                    HTTPServerResponse& response) {
    
    if(isValidRequest(settings.route, request, response)) {
        
        URI uri(request.getURI());
        
        string path = uri.getPath(); // just get the path
        
        ofLogVerbose("ofxWebServerDefaultRouteHandler::handleRequest") << "request: " << request.getURI();
        
        if(path == "/") { path = "/" + settings.defaultIndex;} // default index
        
        ofFile file(settings.route.documentRoot + path);
        string absolutePath = file.getAbsolutePath();
        string fileExtension = file.getExtension();
        
        try {
            MediaType mediaType = ofxWebServerGetMimeType(fileExtension);
            response.sendFile(absolutePath, mediaType.toString());
        } catch (const Poco::FileNotFoundException& ex) {
            response.setStatusAndReason(HTTPResponse::HTTP_NOT_FOUND);
            ofLogError("ofxWebServerDefaultRouteHandler::handleRequest") << ex.displayText();
        } catch (const Poco::OpenFileException& ex) {
            response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            ofLogError("ofxWebServerDefaultRouteHandler::handleRequest") << ex.displayText();
        } catch (const std::exception& ex) {
            response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            ofLogError("ofxWebServerDefaultRouteHandler::handleRequest") << "Unknown server error: " << ex.what();
        }
        
        if(response.getStatus() != HTTPResponse::HTTP_OK) {
            sendErrorResponse(settings.route,response);
        }
    }
    
}
#include "ofxWebServerDefaultRouteHandler.h"

//------------------------------------------------------------------------------
ofxWebServerDefaultRouteHandler::Settings::Settings() {
    defaultIndex = "index.html";
    documentRoot = "DocumentRoot";
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
        
        ofFile file(settings.documentRoot + path); // use it to parse file name parts

        try {
            MediaType mediaType = ofxWebServerGetMimeType(file.getExtension());
            response.sendFile(file.getAbsolutePath(), mediaType.toString());
        } catch (const Poco::FileNotFoundException& ex) {
            ofLogError("ofxWebServerDefaultRouteHandler::handleRequest") << ex.displayText();
            response.setStatusAndReason(HTTPResponse::HTTP_NOT_FOUND);
            sendErrorResponse(response);
        } catch (const Poco::OpenFileException& ex) {
            ofLogError("ofxWebServerDefaultRouteHandler::handleRequest") << ex.displayText();
            response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            sendErrorResponse(response);
        } catch (const std::exception& ex) {
            ofLogError("ofxWebServerDefaultRouteHandler::handleRequest") << "Unknown server error: " << ex.what();
            response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            sendErrorResponse(response);

        }
    } else {
        return; // isValidRequest took care of the response
    }
}

//------------------------------------------------------------------------------
void ofxWebServerDefaultRouteHandler::sendErrorResponse(HTTPServerResponse& response) {
    // now check to see if the status was set something other than 200 by an exception

    HTTPResponse::HTTPStatus responseStatus = response.getStatus();
    // see if we have an html file with that error code
    ofFile errorFile(settings.documentRoot + "/" + ofToString(responseStatus) + ".html");
    if(errorFile.exists()) {
        try {
            response.sendFile(errorFile.getAbsolutePath(),"text/html");
        } catch(const FileNotFoundException& exc) {
            ofxWebServerBaseRouteHandler::sendErrorResponse(response);
        } catch(const OpenFileException& exc) {
            ofxWebServerBaseRouteHandler::sendErrorResponse(response);
        }
    } else {
        // we didn't have one in the DocumentRoot, so generate one
        ofxWebServerBaseRouteHandler::sendErrorResponse(response);
    }
}


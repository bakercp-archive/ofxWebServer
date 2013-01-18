#include "ofxWebServerFailedRouteHandler.h"

//------------------------------------------------------------------------------
ofxWebServerFailedRouteHandler::ofxWebServerFailedRouteHandler() { }

//------------------------------------------------------------------------------
ofxWebServerFailedRouteHandler::~ofxWebServerFailedRouteHandler() { }

//------------------------------------------------------------------------------
void ofxWebServerFailedRouteHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");
    response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    string statusAndReason = ofToString(response.getStatus()) + " - " + response.getReason();
    string s;
    s += "<html>";
    s += "<head><title>" + statusAndReason + "</title></head>";
    s += "<body>";
    s += "<h1>" + statusAndReason + "</h1>";
    s += "<h2>There were no request handlers defined for ofxWebServer.</h2>";
    s += "</body>";
    s += "<html>";
    std::ostream& ostr = response.send(); // get output stream
    ostr << s;
}

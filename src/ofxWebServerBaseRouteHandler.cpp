#include "ofxWebServerBaseRouteHandler.h"

//------------------------------------------------------------------------------
ofxWebServerBaseRouteHandler::Settings::Settings() {
    path = "/";
    
    documentRoot = "DocumentRoot";
    
    sessionCookieName = "session";
    
    requireAuthentication = false;
    
    realm = "Default Realm";
    username = "username";
    password = "password";

}

//------------------------------------------------------------------------------
ofxWebServerBaseRouteHandler::ofxWebServerBaseRouteHandler() { }

//------------------------------------------------------------------------------
ofxWebServerBaseRouteHandler::~ofxWebServerBaseRouteHandler() { }

//------------------------------------------------------------------------------
bool ofxWebServerBaseRouteHandler::matchRoute(const URI& uri, const Settings& settings) {
    return RegularExpression(settings.path).match(uri.getPath());
}

//------------------------------------------------------------------------------
bool ofxWebServerBaseRouteHandler::isValidRequest(const Settings& settings,
                                                  HTTPServerRequest& request,
                                                  HTTPServerResponse& response) {
    
    string sessionId = "";
    
    // extract cookie from request
    NameValueCollection cookies;
    request.getCookies(cookies);
    NameValueCollection::ConstIterator it = cookies.find(settings.sessionCookieName);
    if (it != cookies.end()) {
        sessionId = it->second;
    } else {
        sessionId = ofxWebServerSessionManager::generateSessionKey(request);
        HTTPCookie cookie(settings.sessionCookieName,sessionId);
        cookie.setPath("/");
        // set no age, so it expires @ end of session
        response.addCookie(cookie);
    }
    
    // TODO: update session manager
    
    URI uri(request.getURI());
    
    string path = uri.getPath(); // just get the path

    if(settings.requireAuthentication) {
        if(request.hasCredentials()) {
            HTTPBasicCredentials credentials(request);
            const std::string& user = credentials.getUsername();
            const std::string& pwd = credentials.getPassword();
            
            if(settings.username == credentials.getUsername() &&
               settings.password == credentials.getPassword()) {
                // add an authentication cookie?
                return true;
            } else {
                response.setStatusAndReason(HTTPResponse::HTTP_UNAUTHORIZED);
                sendErrorResponse(settings, response);
                return false;
            }
            
        } else {
            response.requireAuthentication(settings.realm);
            response.setContentLength(0);
            response.send();
            return false;
        }
    } else {
        return true;
    }
}

void ofxWebServerBaseRouteHandler::sendErrorResponse(const Settings& settings,
                                                     HTTPServerResponse& response) {
    // now check to see if the status was set something other than 200 by an exception
    
    if(response.getStatus() == HTTPResponse::HTTP_OK) {
        response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        ofLogError("ofxWebServerBaseRouteHandler::sendErrorResponse") << "Error response requested, but no error found. Sending 500.";
    }
    
    HTTPResponse::HTTPStatus responseStatus = response.getStatus();
    string responseStatusReason = response.getReason();
    
    // see if the user has created a file with that error code
    ofFile errorFile(settings.documentRoot + "/" + ofToString(responseStatus) + "." + "html");
    if(errorFile.exists()) {
        response.sendFile(errorFile.getAbsolutePath(),"text/html");
    } else {
        response.setContentType("text/html");
        string statusAndReason = ofToString(responseStatus) + " - " + responseStatusReason;
        string s;
        s += "<html>";
        s += "<head><title>" + statusAndReason + "</title></head>";
        s += "<body>";
        s += "<h1>" + statusAndReason + "</h1>";
        s += "</body>";
        s += "<html>";
        std::ostream& ostr = response.send(); // get output stream
        ostr << s;
    }
}


#include "ofxWebServerBaseRouteHandler.h"

//------------------------------------------------------------------------------
ofxWebServerBaseRouteHandler::Settings::Settings() {
    path = "/";
    
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
    
    const string path = uri.getPath(); // just get the path

    if(settings.requireAuthentication) {
        if(request.hasCredentials()) {
            HTTPBasicCredentials credentials(request);
            const string& user = credentials.getUsername();
            const string& pwd = credentials.getPassword();
            
            if(settings.username == credentials.getUsername() &&
               settings.password == credentials.getPassword()) {
                // add an authentication cookie?
                return true;
            } else {
                response.setStatusAndReason(HTTPResponse::HTTP_UNAUTHORIZED);
                sendErrorResponse(response);
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

void ofxWebServerBaseRouteHandler::sendErrorResponse(HTTPServerResponse& response) {
    // we will assume that the sender has set the status and
    // reason appropriately before calling the sendErrorResponse()
    
    HTTPResponse::HTTPStatus status = response.getStatus();
    string reason = response.getReason();
    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");
    std::ostream& ostr = response.send(); // get output stream
    ostr << "<html>";
    ostr << "<head><title>" << status << "-" << reason << "</title></head>";
    ostr << "<body>";
    ostr << "<h1>" << status << "-" << reason << "</h1>";
    ostr << "</body>";
    ostr << "<html>";
}


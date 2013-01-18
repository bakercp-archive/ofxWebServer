/*==============================================================================
 
 Copyright (c) 2010-2013 Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ==============================================================================*/

#pragma once

#include "ofLog.h"

#include <string>

#include "Poco/URI.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/NameValueCollection.h"

#include "ofUtils.h"
#include "ofxHttpUtils.h"

#include "ofxWebServerSessionManager.h"
#include "ofxWebServerMimeTypes.h"

using std::string;

using Poco::URI;
using Poco::Net::HTTPCookie;
using Poco::Net::HTTPBasicCredentials;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;


//------------------------------------------------------------------------------
class ofxWebServerBaseRouteHandler : public HTTPRequestHandler {
public:
    struct Settings;
    
    ofxWebServerBaseRouteHandler();
    virtual ~ofxWebServerBaseRouteHandler();
    
    static bool matchRoute(const URI& uri, const Settings& settings);
    
    virtual bool isValidRequest(const Settings& settings,
                                HTTPServerRequest& request,
                                HTTPServerResponse& response);
    
    virtual void sendErrorResponse(const Settings& settings,
                                   HTTPServerResponse& response);
    
    
    
    struct Settings {
        string path;
        
        string documentRoot;
        
        string sessionCookieName;
        
        bool   requireAuthentication;
        
        string realm;
        string username;
        string password;
        
        Settings();
    };
    
};

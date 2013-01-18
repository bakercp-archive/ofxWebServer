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

#include "ofxWebServerBaseRoute.h"
#include "ofxWebServerDefaultRouteHandler.h"

//------------------------------------------------------------------------------
class ofxWebServerDefaultRoute : public ofxWebServerBaseRoute {
public:
    
    ofxWebServerDefaultRoute(const ofxWebServerDefaultRouteHandler::Settings& _settings = ofxWebServerDefaultRouteHandler::Settings() ) {
        ofDirectory documentRootDirectory(settings.route.documentRoot);
        if(!documentRootDirectory.exists()) {
            ofLogNotice("ofxWebServerDefaultRoute::ofxWebServerDefaultRoute") << "Document Root directory did not exist.  Creating: " << documentRootDirectory.getAbsolutePath();
            documentRootDirectory.create();
        }
    }
    virtual ~ofxWebServerDefaultRoute() { }

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) {
        if(ofxWebServerDefaultRouteHandler::matchRoute(URI(request.getURI()), settings.route)) {
            return new ofxWebServerDefaultRouteHandler(settings);
        } else {
            return NULL;
        }
    }

    static ofPtr<ofxWebServerDefaultRoute> Instance(const ofxWebServerDefaultRouteHandler::Settings& settings = ofxWebServerDefaultRouteHandler::Settings()) {
        return ofPtr<ofxWebServerDefaultRoute>(new ofxWebServerDefaultRoute(settings));
    }
    
protected:
    
    ofxWebServerDefaultRouteHandler::Settings settings;
    
};




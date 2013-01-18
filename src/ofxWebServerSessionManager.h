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

#include <string>
#include <map>

#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "ofUtils.h"

using std::string;
using std::map;

using Poco::DigestOutputStream;
using Poco::DigestEngine;
using Poco::MD5Engine;
using Poco::Net::HTTPServerRequest;

class ofxWebServerSession {
public:
    ofxWebServerSession() {}
    virtual ~ofxWebServerSession() {}
    
    string sessionId;
    unsigned long long lastConnected;
};


class ofxWebServerSessionManager {
public:
    ofxWebServerSessionManager();
    virtual ~ofxWebServerSessionManager();
    
    static string generateSessionKey(const HTTPServerRequest& request);

protected:
    map<string,ofxWebServerSession> sessions;
    
};

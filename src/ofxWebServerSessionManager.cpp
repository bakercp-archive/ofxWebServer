#include "ofxWebServerSessionManager.h"

//------------------------------------------------------------------------------
ofxWebServerSessionManager::ofxWebServerSessionManager() { }

//------------------------------------------------------------------------------
ofxWebServerSessionManager::~ofxWebServerSessionManager() { }

//------------------------------------------------------------------------------
string ofxWebServerSessionManager::generateSessionKey(const HTTPServerRequest& request) {
    MD5Engine md5;
    DigestOutputStream ostr(md5);
    // md5(micros + client address)
    ostr << ofGetSystemTimeMicros() << request.clientAddress().host().toString();
    ostr.flush(); // Ensure everything gets passed to the digest engine
    const DigestEngine::Digest& digest = md5.digest(); // obtain result
    return DigestEngine::digestToHex(digest);
}

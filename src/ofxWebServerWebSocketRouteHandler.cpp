#include "ofxWebServerWebSocketRouteHandler.h"

//------------------------------------------------------------------------------
ofxWebServerWebSocketRouteHandler::Settings::Settings() {
    route.path = "/ws";
};

//------------------------------------------------------------------------------
ofxWebServerWebSocketRouteHandler::ofxWebServerWebSocketRouteHandler(const Settings& _settings) : settings(_settings) { }

//------------------------------------------------------------------------------
ofxWebServerWebSocketRouteHandler::~ofxWebServerWebSocketRouteHandler() { }

//------------------------------------------------------------------------------
void ofxWebServerWebSocketRouteHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
    if(isValidRequest(settings.route, request, response)) {
		try {
			WebSocket ws(request, response);
            ofLogNotice("ofxWebServerWebSocketRouteHandler::handleRequest") << "WebSocket connection established.";
			char buffer[1024];
			int flags;
			int n;
			do {
				n = ws.receiveFrame(buffer, sizeof(buffer), flags);
                ofLogNotice("ofxWebServerWebSocketRouteHandler::handleRequest") << "Frame received length=" << n << " flags=" << unsigned(flags);
				ws.sendFrame(buffer, n, flags);
			} while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
			ofLogNotice("ofxWebServerWebSocketRouteHandler::handleRequest") << "WebSocket connection closed.";
		} catch (const WebSocketException& exc) {
			switch (exc.code())
			{
                case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                    ofLogError("ofxWebServerWebSocketRouteHandler::handleRequest") << "Error: WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION";
                    response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
                    response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                    response.setContentLength(0);
                    response.send();
                    break;
                    // fallthrough
                case WebSocket::WS_ERR_NO_HANDSHAKE:
                    ofLogError("ofxWebServerWebSocketRouteHandler::handleRequest") << "Error: WS_ERR_NO_HANDSHAKE";
                    response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                    response.setContentLength(0);
                    response.send();
                    break;
                case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
                    ofLogError("ofxWebServerWebSocketRouteHandler::handleRequest") << "Error: WS_ERR_HANDSHAKE_NO_VERSION";
                    response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                    response.setContentLength(0);
                    response.send();
                    break;
                case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                    ofLogError("ofxWebServerWebSocketRouteHandler::handleRequest") << "Error: WS_ERR_HANDSHAKE_NO_KEY";
                    response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                    response.setContentLength(0);
                    response.send();
                    break;
			}
		}
    } else {
        return; // isValidRequest took care of the response
    }
}
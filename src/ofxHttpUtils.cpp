//
//  ofHttpUtils.cpp
//  example
//
//  Created by Christopher P. Baker on 1/16/13.
//
//

#include "ofxHttpUtils.h"

NameValueCollection ofGetQueryMap(const URI& uri) {
    NameValueCollection nvc;
    
    if(uri.empty()) return nvc;

    string query = uri.getQuery();

    if(query.empty()) return nvc;

    vector<string> arguments = ofSplitString(query,"&",true);
    
    vector<string>::const_iterator iter = arguments.begin();
    
    while(iter != arguments.end()) {
        vector<string> tokens = ofSplitString(*iter,"=",true);
        if(tokens.size() > 0) {
            string key   = tokens[0];
            string value = "";
            if(tokens.size() > 1) {
                value = tokens[1];
            }
            nvc.add(key,value);
        }
        ++iter;
    }
    return nvc;
}

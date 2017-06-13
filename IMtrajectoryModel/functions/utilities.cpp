//
//  utilities.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 13.06.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#include "utilities.hpp"

vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;
    
    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }
    
    return internal;
}

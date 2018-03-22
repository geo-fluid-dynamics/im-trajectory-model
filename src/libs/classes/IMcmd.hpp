//
//  IMcmd.hpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 19.03.18.
//  Copyright © 2018 Kai Schüller. All rights reserved.
//

#ifndef IMcmd_hpp
#define IMcmd_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include "../functions/runTests.hpp"

class IMcmd{
public:
    unsigned int flag_returnValue;
    unsigned int flag_return;
    string logfileName;
    string outputName;
    string inputFileName;
    bool simpleLogfile;
    bool flagCalcDistance;
    bool extendLogfile;
    unsigned int extendSubsteps;
    IMcmd(int argc, const char *argv[]);
};

#endif /* IMcmd_hpp */

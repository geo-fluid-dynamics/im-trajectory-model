//
//  IMlogfile.hpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 25.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#ifndef IMlogfile_hpp
#define IMlogfile_hpp

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../functions/utilities.hpp"

using namespace std;

/// Reads an IceMole logfile and exports the heater states and times.
/** Example:
    IMlogfile logfileObj("logfile.log");
 */
class IMlogfile {
    int length;
    vector <float> time;
    
public:
    string filename;                // name of the file to read
    unsigned int numberOfLines;     // number of lines in logfile
    string *content;                // contains each line of logfile as a string
    bool *dataLines;                // true for lines with valid data
    unsigned int numberOfDataLines; // number of lines with valid data
    bool **heaterStates;            // 2d array of the heater states (numberOfDataLines x 24)
    unsigned int *timeInSeconds;    // time in seconds for each data line (beginning from 0 s)
    bool isValidLogfile;            // to check if the logfile is valid
    IMlogfile(string);
    ~IMlogfile();
};

#endif /* IMlogfile_hpp */

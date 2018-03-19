//
//  IMcmd.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 19.03.18.
//  Copyright © 2018 Kai Schüller. All rights reserved.
//

#include "IMcmd.hpp"

// Purpose: Display command line options
static void show_usage(void){
    std::cerr << "Usage:" << endl
    << "Options:\n"
    << "\t-h,--help\t\tShow this help message\n"
    << "\t-l,--logfile\t\tSpecify the logile name\n"
    << "\t-sl,--simplelogfile\tSpecify the simple logile name\n"
    << "\t-i,--input\t\tSpecify the input file name (*.ini)\n"
    << "\t-d,--distance\t\tDecide if the total distance should be calculated (0: no, 1: yes)\n"
    << "\t-t,--test\t\tRun tests\n"
    << std::endl;
}

IMcmd::IMcmd(int argc, const char *argv[]){
    // Check command line arguments
    
    string testName;
    int testNumber=0;
    int flag_notPassed;
    
    flag_returnValue=0;
    flag_return=0;
    logfileName="";
    outputName="trajectory.log";
    inputFileName="";
    simpleLogfile=false;
    flagCalcDistance=true;
    
    if (argc > 0) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if ((arg == "-h") || (arg == "--help")) {
                
                show_usage();
                flag_returnValue=0;
                flag_return=1;
                
            } else if ((arg == "-l") || (arg == "--logfile")) {
                
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    logfileName = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                    simpleLogfile=false;
                } else { // there was no argument to the destination option.
                    std::cerr << "--logfile option requires one argument." << std::endl;
                    flag_returnValue=1;
                    flag_return=1;
                }
            } else if ((arg == "-sl") || (arg == "--simplelogfile")) {
                
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    logfileName = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                    simpleLogfile=true;
                } else { // there was no argument to the destination option.
                    std::cerr << "--simplelogfile option requires one argument." << std::endl;
                    flag_returnValue=1;
                    flag_return=1;
                }
            }else if((arg == "-o") || (arg == "--output")){
                
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    outputName = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                } else { // there was no argument to the destination option.
                    std::cerr << "--output option requires one argument." << std::endl;
                    flag_returnValue=1;
                    flag_return=1;
                }
                
            }else if ((arg == "-t") || (arg == "--test")) {
                if (i + 2 < argc) { // Make sure we aren't at the end of argv!
                    testName = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                    testNumber = atoi(argv[++i]);
                } else { // there was no argument to the destination option.
                    std::cerr << "--test option requires two arguments." << std::endl;
                    flag_returnValue=1;
                    flag_return=1;
                }
                
                flag_notPassed=runTests(testName,testNumber);
                flag_returnValue=flag_notPassed;
                flag_return=1;
                
            }else if ((arg == "-i") || (arg == "--input")) {
                
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    inputFileName = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                } else { // there was no argument to the destination option.
                    std::cerr << "--input file option requires one argument." << std::endl;
                    flag_returnValue=1;
                    flag_return=1;
                }
                
            }else if ((arg == "-d") || (arg == "--distance")) {
                
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    flagCalcDistance = atoi(argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
                } else { // there was no argument to the destination option.
                    std::cerr << "--distance option requires one argument." << std::endl;
                    flag_returnValue=1;
                    flag_return=1;
                }
                
            } else {
                
                std::cerr << "Not a valid input. Use --help to see valid inputs." << std::endl;
                flag_returnValue=1;
                flag_return=1;
            }
        }
    }
    
}

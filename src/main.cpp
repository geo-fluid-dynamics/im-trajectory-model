//
//  main.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 25.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//
//  Purpose: Reads command line inputs, logfile and thermophysical properties
//           to calculate a trajectory from heater states

#include <iostream>
#include "libs/classes/IMlogfile.hpp"
#include "libs/functions/IMinterpreter.hpp"
#include "libs/classes/IMmodel.hpp"
#include "libs/classes/IMtrajectory.hpp"
#include "libs/functions/runTests.hpp"
#include "libs/classes/IMinputFileParser.hpp"

// Purpose: Display command line options
static void show_usage(void)
{
    std::cerr << "Usage:" << endl
    << "Options:\n"
    << "\t-h,--help\t\tShow this help message\n"
    << "\t-l,--logfile\t\tSpecify the logile name\n"
    << "\t-sl,--simplelogfile\t\tSpecify the simple logile name\n"
    << "\t-i,--input\t\tSpecify the input file name (*.ini)\n"
    << "\t-t,--test\t\tRun tests\n"
    << std::endl;
}

int main(int argc, const char * argv[]) {
    
    string logfileName="";
    string outputName="trajectory.log";
    string inputFileName="";
    bool simpleLogfile=false;
    
    // Check command line arguments
    if (argc > 0) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if ((arg == "-h") || (arg == "--help")) {
            
                show_usage();
                return 0;
            
            } else if ((arg == "-l") || (arg == "--logfile")) {
                
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    logfileName = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                    simpleLogfile=false;
                } else { // there was no argument to the destination option.
                    std::cerr << "--logfile option requires one argument." << std::endl;
                    return 1;
                }
            } else if ((arg == "-sl") || (arg == "--simplelogfile")) {
                
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    logfileName = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                    simpleLogfile=true;
                } else { // there was no argument to the destination option.
                    std::cerr << "--simplelogfile option requires one argument." << std::endl;
                    return 1;
                }
            }else if((arg == "-o") || (arg == "--output")){
                
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    outputName = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                } else { // there was no argument to the destination option.
                    std::cerr << "--output option requires one argument." << std::endl;
                    return 1;
                }
            
            }else if ((arg == "-t") || (arg == "--test")) {
            
                int flag_notPassed=runTests();
                return flag_notPassed;
            
            }else if ((arg == "-i") || (arg == "--input")) {
            
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    inputFileName = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                } else { // there was no argument to the destination option.
                    std::cerr << "--input file option requires one argument." << std::endl;
                    return 1;
                }
            
            } else {
            
                std::cerr << "Not a valid input. Use --help to see valid inputs." << std::endl;
                return 1;
            
            }
        }
    }
    
    // We can only execute the program if an appropriate logfile is provided
    if(!logfileName.empty()){
        IMlogfile A(logfileName,simpleLogfile);
        
        float P_H=0.;
        float P_W=0.;
        
        IMmodel myIMmodel;
        //myIMmodel.straightMeltingModel=0;
        //myIMmodel.curvilinearMeltingModel=0;
        
        if (!inputFileName.empty()) {
            
            // read the input file by creating an IMinputFileParser object
            IMinputFileParser inputFileObj(inputFileName);
            inputFileObj.parseToModel(myIMmodel);
        }
        
        double r_cDirection[2];
        r_cDirection[0]=1;
        r_cDirection[1]=0;
        IMtrajectory trajectory(myIMmodel.p_0,myIMmodel.t_0,myIMmodel.n_0,A.numberOfDataLines);
        trajectory.temporalDiscretization=myIMmodel.temporalDiscretization;
        trajectory.subSteps=myIMmodel.subSteps;
        
        for (unsigned int i=0; i<A.numberOfDataLines-1; i++) {
            myIMmodel.meltingMode=IMinterpreter(A.heaterStates[i], &P_H, &P_W);
            
            // parse the power values and calculate the melting velocity and curve radius
            myIMmodel.P_W=P_W;
            myIMmodel.P_H=P_H;
            //cout << "P_H=" << P_H << " " << "P_W=" << P_W << endl;
            myIMmodel.solve();
            
            trajectory.add(A.timeInSeconds[i+1]-A.timeInSeconds[i],myIMmodel.U_0,myIMmodel.r_c,myIMmodel.tau,myIMmodel.r_cDirection);
            
        }
    
        trajectory.writeToDisk(outputName);
    }
    
    return 0;
}



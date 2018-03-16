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
    << "\t-sl,--simplelogfile\tSpecify the simple logile name\n"
    << "\t-i,--input\t\tSpecify the input file name (*.ini)\n"
    << "\t-d,--distance\t\tDecide if the total distance should be calculated (0: no, 1: yes)\n"
    << "\t-t,--test\t\tRun tests\n"
    << std::endl;
}

int main(int argc, const char * argv[]) {
    
    string logfileName="";
    string outputName="trajectory.log";
    string inputFileName="";
    bool simpleLogfile=false;
    bool flagCalcDistance=true;
    
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
                string testName;
                int testNumber;
                if (i + 2 < argc) { // Make sure we aren't at the end of argv!
                    testName = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                    testNumber = atoi(argv[++i]);
                } else { // there was no argument to the destination option.
                    std::cerr << "--test option requires two arguments." << std::endl;
                    return 1;
                }
                
                int flag_notPassed=runTests(testName,testNumber);
                return flag_notPassed;
            
            }else if ((arg == "-i") || (arg == "--input")) {
            
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    inputFileName = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                } else { // there was no argument to the destination option.
                    std::cerr << "--input file option requires one argument." << std::endl;
                    return 1;
                }
            
            }else if ((arg == "-d") || (arg == "--distance")) {
                
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    flagCalcDistance = atoi(argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
                } else { // there was no argument to the destination option.
                    std::cerr << "--distance option requires one argument." << std::endl;
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
        
        if (!A.isValidLogfile){
            cout << A.isValidLogfile << endl;
            cout << "Unable to open the file " << logfileName << endl;
            return 1;
        }
        
        float P_H=0.;
        float P_W=0.;
        
        IMmodel myIMmodel;
        //myIMmodel.straightMeltingModel=0;
        //myIMmodel.curvilinearMeltingModel=0;
        
        if (!inputFileName.empty()) {
            
            // read the input file by creating an IMinputFileParser object
            IMinputFileParser inputFileObj(inputFileName);
            if (!inputFileObj.isValidInputFile) {
                cout << "Unable to open the file " << inputFileName;
                return 1;
            }
            inputFileObj.parseToModel(myIMmodel);
        }
        
        double r_cDirection[2];
        r_cDirection[0]=1;
        r_cDirection[1]=0;
        IMtrajectory trajectory(myIMmodel.p_0,myIMmodel.t_0,myIMmodel.n_0,A.numberOfDataLines,myIMmodel.gravity_vector);
        trajectory.temporalDiscretization=myIMmodel.temporalDiscretization;
        trajectory.subSteps=myIMmodel.subSteps;
        trajectory.flagCalcDistance=flagCalcDistance;
        
        for (unsigned int i=0; i<A.numberOfDataLines-1; i++) {
            myIMmodel.meltingMode=IMinterpreter(A.heaterStates[i], &P_H, &P_W);
            
            // parse the power values and calculate the melting velocity and curve radius
            myIMmodel.P_W=P_W;
            myIMmodel.P_H=P_H;
            myIMmodel.current_t_vector[0]=trajectory.t[i][0];
            myIMmodel.current_t_vector[1]=trajectory.t[i][1];
            myIMmodel.current_t_vector[2]=trajectory.t[i][2];
            
            myIMmodel.solve();
            
            // if the mode is straight melting, we don't need more than one substeps
            if (myIMmodel.meltingMode==0) {
                trajectory.subSteps=1;
            }else{
                trajectory.subSteps=myIMmodel.subSteps;
            }
            
            trajectory.add(A.timeInSeconds[i+1]-A.timeInSeconds[i],&myIMmodel);
            
        }
    
        trajectory.writeToDisk(outputName);
    }
    
    return 0;
}



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
#include "libs/classes/IMinputFileParser.hpp"
#include "libs/classes/IMcmd.hpp"

int main(int argc, const char * argv[]) {
    
    float P_H=0.;
    float P_W=0.;
    
    // Check the command line arguments
    IMcmd myIMcmd(argc, argv);
    if (myIMcmd.flag_return) {
        return myIMcmd.flag_returnValue;
    }
    
    // We can only execute the program if an appropriate logfile is provided
    if(!myIMcmd.logfileName.empty()){
        IMlogfile myIMlogfile(myIMcmd.logfileName,myIMcmd.simpleLogfile);
        
        if (!myIMlogfile.isValidLogfile){
            cout << myIMlogfile.isValidLogfile << endl;
            cout << "Unable to open the file " << myIMcmd.logfileName << endl;
            return 1;
        }
        
        IMmodel myIMmodel;
        
        if (!myIMcmd.inputFileName.empty()) {
            
            // read the input file by creating an IMinputFileParser object
            IMinputFileParser inputFileObj(myIMcmd.inputFileName);
            if (!inputFileObj.isValidInputFile) {
                cout << "Unable to open the file " << myIMcmd.inputFileName;
                return 1;
            }
            inputFileObj.parseToModel(myIMmodel);
        }
        
        IMtrajectory trajectory(&myIMmodel,myIMlogfile.numberOfDataLines);
        trajectory.temporalDiscretization=myIMmodel.temporalDiscretization;
        trajectory.subSteps=myIMmodel.subSteps;
        trajectory.flagCalcDistance=myIMcmd.flagCalcDistance;
        
        for (unsigned int i=0; i<myIMlogfile.numberOfDataLines-1; i++) {
            
            // determine melting probe and calculate the melting head power P_H and the wall power P_W
            myIMmodel.meltingMode=IMinterpreter(myIMlogfile.heaterStates[i], &P_H, &P_W);
            
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
            
            // incremental time integration of Frenet-Serret equations
            trajectory.add(myIMlogfile.timeInSeconds[i+1]-myIMlogfile.timeInSeconds[i],&myIMmodel);
            
        }
    
        trajectory.writeToDisk(myIMcmd.outputName);
    }
    
    return 0;
}



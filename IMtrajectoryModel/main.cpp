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
#include "classes/IMlogfile.hpp"
#include "functions/IMinterpreter.hpp"
#include "classes/IMmodel.hpp"
#include "classes/IMtrajectory.hpp"
#include "functions/runTests.hpp"
#include "classes/IMinputFileParser.hpp"

// Purpose: Display command line options
static void show_usage(void)
{
    std::cerr << "Usage:" << endl
    << "Options:\n"
    << "\t-h,--help\t\tShow this help message\n"
    << "\t-l,--logfile\t\tSpecify the logile name\n"
    << "\t-i,--input\t\tSpecify the input file name (*.ini)\n"
    << "\t-t,--test\t\tRun tests\n"
    << std::endl;
}

// Purpose: Add a value of a IMinputFileParser object to an IMmodel object
void useDataFromInputFile(bool flag, double* inputFileValue, double* modelValue,string filename, string variablename){
    if (flag) {
        *modelValue=*inputFileValue;
    }else{
        cout << "WARNING: " << variablename << " was not found in " << filename << ". Using the default value " << *modelValue << " instead." << endl;
    }
}

int main(int argc, const char * argv[]) {
    
    string logfileName="";
    string outputName="./";
    string inputFileName="";
    
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
                } else { // there was no argument to the destination option.
                    std::cerr << "--logfile option requires one argument." << std::endl;
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
            
                int flag=runTests();
                return flag;
            
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
    
    // polynomial coefficients for linear equation for tau
    double c1_tau=0;
    double c2_tau=0;
    
    // We can only execute the program if an appropriate logfile is provided
    if(!logfileName.empty()){
        IMlogfile A(logfileName);
        
        float P_H=0.;
        float P_W=0.;
        vector<float> U_0(A.numberOfDataLines);  // melting velocity
        vector<vector<float> > r_cDirection(A.numberOfDataLines,vector<float>(3));  // curve radius direction
        vector<float> r_c(A.numberOfDataLines);
        vector<float> P_Ws(A.numberOfDataLines);
        vector<float> P_Hs(A.numberOfDataLines);
        vector<int> mMode(A.numberOfDataLines);
        
        IMmodel myIMmodel;
        myIMmodel.straightMeltingModel=0;
        myIMmodel.curvilinearMeltingModel=0;

        if (!inputFileName.empty()) {
            
            // read the input file by creating an IMinputFileParser object
            IMinputFileParser inputFileObj(inputFileName);
            
            // now print a waring if variables has not been defined in the input file
            useDataFromInputFile(inputFileObj.flag_rho_S, &inputFileObj.rho_S, &myIMmodel.rho_S,inputFileName,"rho_S");
            useDataFromInputFile(inputFileObj.flag_rho_L, &inputFileObj.rho_L, &myIMmodel.rho_L,inputFileName,"rho_L");
            useDataFromInputFile(inputFileObj.flag_h_m, &inputFileObj.h_m, &myIMmodel.h_m,inputFileName,"h_m");
            useDataFromInputFile(inputFileObj.flag_T_m, &inputFileObj.T_m, &myIMmodel.T_m,inputFileName,"T_m");
            useDataFromInputFile(inputFileObj.flag_T_S, &inputFileObj.T_S, &myIMmodel.T_S,inputFileName,"T_S");
            useDataFromInputFile(inputFileObj.flag_c_p_S, &inputFileObj.c_p_S, &myIMmodel.c_p_S,inputFileName,"c_p_S");
            useDataFromInputFile(inputFileObj.flag_c_p_L, &inputFileObj.c_p_L, &myIMmodel.c_p_L,inputFileName,"c_p_S");
            useDataFromInputFile(inputFileObj.flag_k_L, &inputFileObj.k_L, &myIMmodel.k_L,inputFileName,"k_L");
            useDataFromInputFile(inputFileObj.flag_k_S, &inputFileObj.k_S, &myIMmodel.k_S,inputFileName,"k_S");
            useDataFromInputFile(inputFileObj.flag_L, &inputFileObj.L, &myIMmodel.L,inputFileName,"L");
            useDataFromInputFile(inputFileObj.flag_H, &inputFileObj.H, &myIMmodel.H,inputFileName,"H");
            
            if (inputFileObj.flag_c1_tau) {
                c1_tau=inputFileObj.c1_tau;
            }else{
                cout << "WARNING: c1_tau was not found in " << inputFileName << ". Using the default value " << c1_tau << " instead." << endl;
            }
            
            if (inputFileObj.flag_c2_tau) {
                c2_tau=inputFileObj.c2_tau;
            }else{
                cout << "WARNING: c2_tau was not found in " << inputFileName << ". Using the default value " << c2_tau << " instead." << endl;
            }
        }
        
        double p_0[3]={0,0,0};
        double t_0[3]={0,0,-1};
        double n_0[3]={1,0,0};
        
        double tau=0;
        
        // here we set r_cDirection equal to n_0 for simplicity
        IMtrajectory trajectory(p_0,t_0,n_0,n_0,A.numberOfDataLines+1);
        trajectory.subSteps=0;
        
        for (unsigned int i=0; i<A.numberOfDataLines; i++) {
            mMode[i]=IMinterpreter(A.heaterStates[i], &P_H, &P_W);
            P_Ws[i]=P_W;
            P_Hs[i]=P_H;
            
            // parse the power values and calculate the melting velocity and curve radius
            myIMmodel.P_W=P_W;
            myIMmodel.P_H=P_H;
            myIMmodel.meltingMode=mMode[i];
            myIMmodel.solve();
            
            r_cDirection[i][0]=myIMmodel.r_cDirection[0];
            r_cDirection[i][1]=myIMmodel.r_cDirection[1];
            r_cDirection[i][2]=myIMmodel.r_cDirection[2];
            r_c[i]=myIMmodel.r_c;
            U_0[i]=myIMmodel.U_0;
            
            // we assume that tau can be a linear function of the meling velocity
            tau=U_0[i]*c1_tau+c2_tau;
            
            trajectory.add(A.timeInSeconds[i+1]-A.timeInSeconds[i],U_0[i],r_c[i],tau,myIMmodel.r_cDirection);
            
        }
    
        trajectory.writeToDisk("trajectory.log");
    }
    
    return 0;
}



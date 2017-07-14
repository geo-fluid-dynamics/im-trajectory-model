//
//  runTests.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 29.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#include "runTests.hpp"

float isNearlyEqual(float a, float b){
    return fabsf( a - b ) < 1e-10;
}

void testerInfo(int condition,string moduleName,int testNumber){
    cout << moduleName << " Test " << testNumber << ": ";
    if(condition){
        cout << "passed";
    }else{
        cout << "failed";
    }
    cout << endl;
}

int testLogfileReading(string filename){
    
    IMlogfile logfile(filename,false);
    
    return logfile.isValidLogfile;
}

int runTests(void){
    
    // Logfile Reading
    // Test 1
    system("touch emptyfile.log");
    testerInfo(!testLogfileReading("emptyfile.log"),"IMlogfile",1);
    system("rm emptyfile.log");
    
    // IMinterpreter
    // Test 1
    bool testHeaterStates1[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    float P_W1,P_H1;
    testerInfo(IMinterpreter(testHeaterStates1, &P_H1, &P_W1)==-1,"IMinterpreter",1);
    
    // Test 2
    bool testHeaterStates2[24]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0};
    float P_W2,P_H2;
    int mMode2=IMinterpreter(testHeaterStates2, &P_H2, &P_W2);
    testerInfo(mMode2==0 && isNearlyEqual( P_W2, 0.0) && isNearlyEqual( P_H2, 2880.0),"IMinterpreter",2);
    
    // Test 3
    bool testHeaterStates3[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};
    float P_W3,P_H3;
    int mMode3=IMinterpreter(testHeaterStates3, &P_H3, &P_W3);
    testerInfo(mMode3==-1 && isNearlyEqual( P_H3, 0.0) && isNearlyEqual( P_W3, 0.0),"IMinterpreter",3);
    
    // Test 4
    // Since no wall heaters are active -> straight melting
    bool testHeaterStates4[24]={1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    float P_W4,P_H4;
    int mMode4=IMinterpreter(testHeaterStates4, &P_H4, &P_W4);
    testerInfo(mMode4==0 && isNearlyEqual( P_H4, 1440.0) && isNearlyEqual( P_W4, 0.0),"IMinterpreter",4);
    
    // Test 5
    // Since the wrong wall heaters are active -> straight melting
    bool testHeaterStates5[24]={1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1};
    float P_W5,P_H5;
    int mMode5=IMinterpreter(testHeaterStates5, &P_H5, &P_W5);
    testerInfo(mMode5==0 && isNearlyEqual( P_H5, 1440.0) && isNearlyEqual( P_W5, 0.0),"IMinterpreter",5);
    
    // Test 6
    bool testHeaterStates6[24]={1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0};
    float P_W6,P_H6;
    int mMode6=IMinterpreter(testHeaterStates6, &P_H6, &P_W6);
    testerInfo(mMode6==2 && isNearlyEqual( P_H6, 1440.0) && isNearlyEqual( P_W6, 2000.0),"IMinterpreter",6);
    
    // Test 7
    bool testHeaterStates7[24]={0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0};
    float P_W7,P_H7;
    int mMode7=IMinterpreter(testHeaterStates7, &P_H7, &P_W7);
    testerInfo(mMode7==1 && isNearlyEqual( P_H7, 1440.0) && isNearlyEqual( P_W7, 2000.0),"IMinterpreter",7);
    
    // Test 8
    bool testHeaterStates8[24]={0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0};
    float P_W8,P_H8;
    int mMode8=IMinterpreter(testHeaterStates8, &P_H8, &P_W8);
    testerInfo(mMode8==3 && isNearlyEqual( P_H8, 1440.0) && isNearlyEqual( P_W8, 2000.0),"IMinterpreter",8);
    
    // Test 9
    bool testHeaterStates9[24]={1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1};
    float P_W9,P_H9;
    int mMode9=IMinterpreter(testHeaterStates9, &P_H9, &P_W9);
    testerInfo(mMode9==4 && isNearlyEqual( P_H9, 1440.0) && isNearlyEqual( P_W9, 2000.0),"IMinterpreter",9);
    
    // IMmodel
    IMmodel testIMmodel;
    testIMmodel.straightMeltingModel=0;
    testIMmodel.curvilinearMeltingModel=0;
    
    // Test 1
    // simplified curve radius calculation
    testIMmodel.P_W=4;
    testIMmodel.P_H=0.3;
    testIMmodel.meltingMode=1;
    testIMmodel.solve();
    testerInfo(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[2], 0.0) && isNearlyEqual( testIMmodel.r_cDirection[1], 1.0) && isNearlyEqual( testIMmodel.r_cDirection[0], 0.0),"IMmodel",1);
    
    // Test 2
    testIMmodel.meltingMode=2;
    testIMmodel.solve();
    testerInfo(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[2], 0.0) && isNearlyEqual( testIMmodel.r_cDirection[1], -1.0) && isNearlyEqual( testIMmodel.r_cDirection[0], 0.0),"IMmodel",2);
    
    // Test 3
    testIMmodel.meltingMode=3;
    testIMmodel.solve();
    testerInfo(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[2], -1.0) && isNearlyEqual( testIMmodel.r_cDirection[1], 0.0) && isNearlyEqual( testIMmodel.r_cDirection[0], 0.0),"IMmodel",3);
    
    // Test 4
    testIMmodel.meltingMode=4;
    testIMmodel.solve();
    testerInfo(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[2], 1.0) && isNearlyEqual( testIMmodel.r_cDirection[1], 0.0) && isNearlyEqual( testIMmodel.r_cDirection[0], 0.0),"IMmodel",4);
    
    // Test 5
    testIMmodel.c_p_S=2049.41;
    testIMmodel.h_m=333700;
    testIMmodel.rho_S=920;
    testIMmodel.T_m=0;
    testIMmodel.T_S=-20;
    testIMmodel.P_H=1000;
    testIMmodel.H=0.15;
    testIMmodel.solve();
    testerInfo(abs(testIMmodel.U_0-0.000128932)<0.000000003,"IMmodel",5);
    
    // IMtrajectory
    
    // Test 1
    IMmodel testIMmodel2;
    testIMmodel2.straightMeltingModel=0;
    testIMmodel2.curvilinearMeltingModel=0;
    double p_0[3]={0,0,0};
    double t_0[3]={0,0,-1};
    double n_0[3]={1,0,0};
    double r_cDirection[3]={1,0,0};
    IMtrajectory trajectory(p_0,t_0,n_0,n_0,5);
    trajectory.subSteps=0;
    trajectory.add(10.,0.1,100000,0,r_cDirection);
    // cout << trajectory.p[0][0] << " " << trajectory.p[0][1] << " " << trajectory.p[0][2] << endl;
    // cout << trajectory.p[1][0] << " " << trajectory.p[1][1] << " " << trajectory.p[1][2] << endl;
    testerInfo(trajectory.p[1][0]==0 && trajectory.p[1][1]==0 && trajectory.p[1][2]==-1,"IMtrajectory",1);
    
    // Test 2
    trajectory.reset();
    trajectory.subSteps=100;
    trajectory.add(10.,0.1,100000,0,r_cDirection);
    //cout << trajectory.p[0][0] << " " << trajectory.p[0][1] << " " << trajectory.p[0][2] << endl;
    //cout << trajectory.p[1][0] << " " << trajectory.p[1][1] << " " << trajectory.p[1][2] << endl;
    testerInfo(trajectory.p[1][0]<4.9506e-6 && trajectory.p[1][1]==0 && abs(trajectory.p[1][2]-(-1))<1e-6,"IMtrajectory",2);
    
    // Test 3
    // test backward Euler method
    trajectory.reset();
    trajectory.method=1;
    trajectory.subSteps=100;
    trajectory.add(10.,0.1,100000,0,r_cDirection);
    //cout << trajectory.p[0][0] << " " << trajectory.p[0][1] << " " << trajectory.p[0][2] << endl;
    //cout << trajectory.p[1][0] << " " << trajectory.p[1][1] << " " << trajectory.p[1][2] << endl;
    testerInfo(trajectory.p[1][0]<5.1496e-6 && trajectory.p[1][1]==0 && abs(trajectory.p[1][2]-(-1))<1e-6,"IMtrajectory",3);
    
    // Test 4
    // forward euler: a quarter curve, the endpoint must be at p=(1,0,-1) when subSteps go to infinity
    trajectory.reset();
    trajectory.method=0;
    trajectory.subSteps=1000;
    const double PI = std::atan(1.0)*4;
    double r_c=1.0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //cout << trajectory.p[0][0] << " " << trajectory.p[0][1] << " " << trajectory.p[0][2] << endl;
    //cout << trajectory.p[1][0] << " " << trajectory.p[1][1] << " " << trajectory.p[1][2] << endl;
    testerInfo(abs(trajectory.p[1][0]-1.0)<0.01 && abs(trajectory.p[1][1]-0.0)<0.01 && abs(trajectory.p[1][2]-(-1.0))<0.01,"IMtrajectory",4);
    
    // Test 5
    // backward euler: a quarter curve, the endpoint must be at p=(1,0,-1) when subSteps go to infinity
    trajectory.reset();
    trajectory.method=1;
    trajectory.subSteps=1000;
    r_c=1.0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //cout << trajectory.p[0][0] << " " << trajectory.p[0][1] << " " << trajectory.p[0][2] << endl;
    //cout << trajectory.p[1][0] << " " << trajectory.p[1][1] << " " << trajectory.p[1][2] << endl;
    testerInfo(abs(trajectory.p[1][0]-1.0)<0.01 && abs(trajectory.p[1][1]-0.0)<0.01 && abs(trajectory.p[1][2]-(-1.0))<0.01,"IMtrajectory",5);
    
    // Test 6
    // forward euler: a quarter curve with a different curve radius direction, the endpoint must be at p=(0,-1,-1) when subSteps go to infinity
    trajectory.reset();
    trajectory.method=0;
    trajectory.subSteps=1000;
    r_c=1.0;
    r_cDirection[0]=0;
    r_cDirection[1]=-1;
    r_cDirection[2]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //cout << trajectory.p[0][0] << " " << trajectory.p[0][1] << " " << trajectory.p[0][2] << endl;
    //cout << trajectory.p[1][0] << " " << trajectory.p[1][1] << " " << trajectory.p[1][2] << endl;
    testerInfo(abs(trajectory.p[1][0]-0.0)<0.01 && abs(trajectory.p[1][1]-(-1.0))<0.01 && abs(trajectory.p[1][2]-(-1.0))<0.01,"IMtrajectory",6);
    
    // Test 7
    // forward euler: test 3d motion
    trajectory.reset();
    trajectory.method=0;
    trajectory.subSteps=1000;
    r_c=1.0;
    r_cDirection[0]=0;
    r_cDirection[1]=-1;
    r_cDirection[2]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=1;
    r_cDirection[1]=0;
    r_cDirection[2]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //cout << trajectory.p[0][0] << " " << trajectory.p[0][1] << " " << trajectory.p[0][2] << endl;
    //cout << trajectory.p[1][0] << " " << trajectory.p[1][1] << " " << trajectory.p[1][2] << endl;
    //cout << trajectory.p[2][0] << " " << trajectory.p[2][1] << " " << trajectory.p[2][2] << endl;
    testerInfo(abs(trajectory.p[2][0]-1.0)<0.01 && abs(trajectory.p[2][1]-(-2.0))<0.01 && abs(trajectory.p[1][2]-(-1.0))<0.01,"IMtrajectory",7);
    
    return 0;
}

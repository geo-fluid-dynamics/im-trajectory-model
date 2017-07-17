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

int testerInfo(int condition,string moduleName,int testNumber){
    int flag_notPassed=0;
    cout << moduleName << " Test " << testNumber << ": ";
    if(condition){
        cout << "passed";
        flag_notPassed=0;
    }else{
        cout << "failed";
        flag_notPassed=1;
    }
    cout << endl;
    return flag_notPassed;
}

int testLogfileReading(string filename){
    
    IMlogfile logfile(filename,false);
    
    return logfile.isValidLogfile;
}

int runTests(void){
    
    int flag_notPassed=0;
    
    // Logfile Reading
    // Test 1
    system("touch emptyfile.log");
    flag_notPassed=testerInfo(!testLogfileReading("emptyfile.log"),"IMlogfile",1);
    system("rm emptyfile.log");
    
    // Test 2
    // straight melting with a melting velocity of 1.50096 m/h
    IMlogfile A("test/simplifiedLogfile/test_sl1.csv",true);
    IMinputFileParser inputFileObj("test/simplifiedLogfile/test_sl1.ini");
    IMmodel myIMmodel;
    inputFileObj.parseToModel(myIMmodel);
    IMtrajectory trajectory0(myIMmodel.p_0,myIMmodel.t_0,myIMmodel.n_0,A.numberOfDataLines);
    trajectory0.subSteps=myIMmodel.subSteps;
    float P_H=0.;
    float P_W=0.;
    for (unsigned int i=0; i<A.numberOfDataLines-1; i++) {
        myIMmodel.meltingMode=IMinterpreter(A.heaterStates[i], &P_H, &P_W);
        
        // parse the power values and calculate the melting velocity and curve radius
        myIMmodel.P_W=P_W;
        myIMmodel.P_H=P_H;
        myIMmodel.solve();
        
        trajectory0.add(A.timeInSeconds[i+1]-A.timeInSeconds[i],myIMmodel.U_0,myIMmodel.r_c,myIMmodel.tau,myIMmodel.r_cDirection);
    }
    flag_notPassed=testerInfo(abs(trajectory0.p[trajectory0.length-1][2]+1.50096)<0.00001,"IMLogfile",2);
    
    // Test 3
    // straight melting with a melting velocity of 1.50096 m/h
    IMlogfile A3("test/simplifiedLogfile/test_sl2.csv",true);
    IMinputFileParser inputFileObj3("test/simplifiedLogfile/test_sl2.ini");
    IMmodel myIMmodel3;
    inputFileObj3.parseToModel(myIMmodel3);
    IMtrajectory trajectory3(myIMmodel3.p_0,myIMmodel3.t_0,myIMmodel3.n_0,A3.numberOfDataLines);
    trajectory3.subSteps=myIMmodel3.subSteps;
    P_H=0.;
    P_W=0.;
    for (unsigned int i=0; i<A3.numberOfDataLines-1; i++) {
        myIMmodel3.meltingMode=IMinterpreter(A3.heaterStates[i], &P_H, &P_W);
        
        // parse the power values and calculate the melting velocity and curve radius
        myIMmodel3.P_W=P_W;
        myIMmodel3.P_H=P_H;
        myIMmodel3.solve();
        
        trajectory3.add(A3.timeInSeconds[i+1]-A3.timeInSeconds[i],myIMmodel3.U_0,myIMmodel3.r_c,myIMmodel3.tau,myIMmodel3.r_cDirection);
    }
    flag_notPassed=testerInfo(abs(trajectory3.p[trajectory3.length-1][1]-19.2)<0.001,"IMLogfile",3);

    // Test 4
    // straight melting with a melting velocity of 1.50096 m/h
    IMlogfile A4("test/simplifiedLogfile/test_sl3.csv",true);
    IMinputFileParser inputFileObj4("test/simplifiedLogfile/test_sl3.ini");
    IMmodel myIMmodel4;
    inputFileObj4.parseToModel(myIMmodel4);
    IMtrajectory trajectory4(myIMmodel4.p_0,myIMmodel4.t_0,myIMmodel4.n_0,A4.numberOfDataLines);
    trajectory4.subSteps=myIMmodel4.subSteps;
    P_H=0.;
    P_W=0.;
    for (unsigned int i=0; i<A4.numberOfDataLines-1; i++) {
        myIMmodel4.meltingMode=IMinterpreter(A4.heaterStates[i], &P_H, &P_W);
        
        // parse the power values and calculate the melting velocity and curve radius
        myIMmodel4.P_W=P_W;
        myIMmodel4.P_H=P_H;
        myIMmodel4.solve();
        
        trajectory4.add(A4.timeInSeconds[i+1]-A4.timeInSeconds[i],myIMmodel4.U_0,myIMmodel4.r_c,myIMmodel4.tau,myIMmodel4.r_cDirection);
    }
    flag_notPassed=testerInfo(abs(trajectory4.p[trajectory4.length-1][0]-(-19.2))<0.001,"IMLogfile",4);
    
    // IMinterpreter
    // Test 1
    bool testHeaterStates1[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    float P_W1,P_H1;
    flag_notPassed=testerInfo(IMinterpreter(testHeaterStates1, &P_H1, &P_W1)==-1,"IMinterpreter",1);
    
    // Test 2
    bool testHeaterStates2[24]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0};
    float P_W2,P_H2;
    int mMode2=IMinterpreter(testHeaterStates2, &P_H2, &P_W2);
    flag_notPassed=testerInfo(mMode2==0 && isNearlyEqual( P_W2, 0.0) && isNearlyEqual( P_H2, 2880.0),"IMinterpreter",2);
    
    // Test 3
    bool testHeaterStates3[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};
    float P_W3,P_H3;
    int mMode3=IMinterpreter(testHeaterStates3, &P_H3, &P_W3);
    flag_notPassed=testerInfo(mMode3==-1 && isNearlyEqual( P_H3, 0.0) && isNearlyEqual( P_W3, 0.0),"IMinterpreter",3);
    
    // Test 4
    // Since no wall heaters are active -> straight melting
    bool testHeaterStates4[24]={1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    float P_W4,P_H4;
    int mMode4=IMinterpreter(testHeaterStates4, &P_H4, &P_W4);
    flag_notPassed=testerInfo(mMode4==0 && isNearlyEqual( P_H4, 1440.0) && isNearlyEqual( P_W4, 0.0),"IMinterpreter",4);
    
    // Test 5
    // Since the wrong wall heaters are active -> straight melting
    bool testHeaterStates5[24]={1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1};
    float P_W5,P_H5;
    int mMode5=IMinterpreter(testHeaterStates5, &P_H5, &P_W5);
    flag_notPassed=testerInfo(mMode5==0 && isNearlyEqual( P_H5, 1440.0) && isNearlyEqual( P_W5, 0.0),"IMinterpreter",5);
    
    // Test 6
    bool testHeaterStates6[24]={1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0};
    float P_W6,P_H6;
    int mMode6=IMinterpreter(testHeaterStates6, &P_H6, &P_W6);
    flag_notPassed=testerInfo(mMode6==2 && isNearlyEqual( P_H6, 1440.0) && isNearlyEqual( P_W6, 2000.0),"IMinterpreter",6);
    
    // Test 7
    bool testHeaterStates7[24]={0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0};
    float P_W7,P_H7;
    int mMode7=IMinterpreter(testHeaterStates7, &P_H7, &P_W7);
    flag_notPassed=testerInfo(mMode7==1 && isNearlyEqual( P_H7, 1440.0) && isNearlyEqual( P_W7, 2000.0),"IMinterpreter",7);
    
    // Test 8
    bool testHeaterStates8[24]={0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0};
    float P_W8,P_H8;
    int mMode8=IMinterpreter(testHeaterStates8, &P_H8, &P_W8);
    flag_notPassed=testerInfo(mMode8==3 && isNearlyEqual( P_H8, 1440.0) && isNearlyEqual( P_W8, 2000.0),"IMinterpreter",8);
    
    // Test 9
    bool testHeaterStates9[24]={1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1};
    float P_W9,P_H9;
    int mMode9=IMinterpreter(testHeaterStates9, &P_H9, &P_W9);
    flag_notPassed=testerInfo(mMode9==4 && isNearlyEqual( P_H9, 1440.0) && isNearlyEqual( P_W9, 2000.0),"IMinterpreter",9);
    
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
    flag_notPassed=testerInfo(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[1], 0.0) && isNearlyEqual( testIMmodel.r_cDirection[0], 1.0),"IMmodel",1);
    
    // Test 2
    testIMmodel.meltingMode=2;
    testIMmodel.solve();
    flag_notPassed=testerInfo(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[1], 0.0) && isNearlyEqual( testIMmodel.r_cDirection[0], -1.0),"IMmodel",2);
    
    // Test 3
    testIMmodel.meltingMode=3;
    testIMmodel.solve();
    flag_notPassed=testerInfo(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[1], -1.0) && isNearlyEqual( testIMmodel.r_cDirection[0], 0.0),"IMmodel",3);
    
    // Test 4
    testIMmodel.meltingMode=4;
    testIMmodel.solve();
    flag_notPassed=testerInfo(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[1], 1.0) && isNearlyEqual( testIMmodel.r_cDirection[0], 0.0),"IMmodel",4);
    
    // Test 5
    testIMmodel.c_p_S=2049.41;
    testIMmodel.h_m=333700;
    testIMmodel.rho_S=920;
    testIMmodel.T_m=0;
    testIMmodel.T_S=-20;
    testIMmodel.P_H=1000;
    testIMmodel.H=0.15;
    testIMmodel.solve();
    flag_notPassed=testerInfo(abs(testIMmodel.U_0-0.000128932)<0.000000003,"IMmodel",5);
    
    // IMtrajectory
    
    // Test 1
    IMmodel testIMmodel2;
    testIMmodel2.straightMeltingModel=0;
    testIMmodel2.curvilinearMeltingModel=0;
    double p_0[3]={0,0,0};
    double t_0[3]={0,0,-1};
    double n_0[3]={1,0,0};
    double r_cDirection[2]={1,0};
    IMtrajectory trajectory(p_0,t_0,n_0,5);
    trajectory.subSteps=0;
    trajectory.add(10.,0.1,100000,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(trajectory.p[1][0]==0 && trajectory.p[1][1]==0 && trajectory.p[1][2]==-1,"IMtrajectory",1);
    
    // Test 2
    trajectory.reset();
    trajectory.subSteps=100;
    trajectory.add(10.,0.1,100000,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(trajectory.p[1][0]<4.9506e-6 && trajectory.p[1][1]==0 && abs(trajectory.p[1][2]-(-1))<1e-6,"IMtrajectory",2);
    
    // Test 3
    // test backward Euler method
    trajectory.reset();
    trajectory.temporalDiscretization=1;
    trajectory.subSteps=100;
    trajectory.add(10.,0.1,100000,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(trajectory.p[1][0]<5.1496e-6 && trajectory.p[1][1]==0 && abs(trajectory.p[1][2]-(-1))<1e-6,"IMtrajectory",3);
    
    // Test 4
    // forward euler: a quarter curve, the endpoint must be at p=(1,0,-1) when subSteps go to infinity
    trajectory.reset();
    trajectory.temporalDiscretization=0;
    trajectory.subSteps=1000;
    const double PI = std::atan(1.0)*4;
    double r_c=1.0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    ///trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[1][0]-1.0)<0.01 && abs(trajectory.p[1][1]-0.0)<0.01 && abs(trajectory.p[1][2]-(-1.0))<0.01,"IMtrajectory",4);
    
    // Test 5
    // backward euler: a quarter curve, the endpoint must be at p=(1,0,-1) when subSteps go to infinity
    trajectory.reset();
    trajectory.temporalDiscretization=1;
    trajectory.subSteps=1000;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[1][0]-1.0)<0.01 && abs(trajectory.p[1][1]-0.0)<0.01 && abs(trajectory.p[1][2]-(-1.0))<0.01,"IMtrajectory",5);
    
    // Test 6
    // forward euler: a quarter curve with a different curve radius direction, the endpoint must be at p=(0,-1,-1) when subSteps go to infinity
    trajectory.reset();
    trajectory.temporalDiscretization=0;
    trajectory.subSteps=1000;
    r_cDirection[0]=-1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[1][1]-0.0)<0.01 && abs(trajectory.p[1][0]-(-1.0))<0.01 && abs(trajectory.p[1][2]-(-1.0))<0.01,"IMtrajectory",6);
    
    // Test 7
    // forward euler: test 3d motion
    trajectory.reset();
    trajectory.temporalDiscretization=0;
    trajectory.subSteps=1000;
    r_cDirection[0]=-1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=0;
    r_cDirection[1]=1;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[2][1]-1.0)<0.01 && abs(trajectory.p[2][0]-(-2.0))<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01,"IMtrajectory",7);
    
    // Test 8
    // forward euler: test 3d motion
    trajectory.reset();
    trajectory.temporalDiscretization=0;
    trajectory.subSteps=1000;
    r_cDirection[0]=-1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=0;
    r_cDirection[1]=-1;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[2][1]-(-1.0))<0.01 && abs(trajectory.p[2][0]-(-2.0))<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01,"IMtrajectory",8);
 
    // Test 9
    // forward euler: test 3d motion
    trajectory.reset();
    trajectory.temporalDiscretization=0;
    trajectory.subSteps=1000;
    r_cDirection[0]=1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=0;
    r_cDirection[1]=1;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[2][1]-1.0)<0.01 && abs(trajectory.p[2][0]-2.0)<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01,"IMtrajectory",9);
    
    // Test 10
    // forward euler: test 3d motion
    trajectory.reset();
    trajectory.temporalDiscretization=0;
    trajectory.subSteps=1000;
    r_cDirection[0]=1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=0;
    r_cDirection[1]=-1;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[2][1]-(-1.0))<0.01 && abs(trajectory.p[2][0]-2.0)<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01,"IMtrajectory",10);

    // Test 11
    // forward euler: test 3d motion
    trajectory.reset();
    trajectory.temporalDiscretization=0;
    trajectory.subSteps=1000;
    r_cDirection[0]=0;
    r_cDirection[1]=-1;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[2][1]-(-2.0))<0.01 && abs(trajectory.p[2][0]-1.0)<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01,"IMtrajectory",11);

    // Test 12
    // forward euler: test 3d motion
    trajectory.reset();
    trajectory.temporalDiscretization=0;
    trajectory.subSteps=1000;
    r_cDirection[0]=0;
    r_cDirection[1]=-1;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=-1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[2][1]-(-2.0))<0.01 && abs(trajectory.p[2][0]-(-1.0))<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01,"IMtrajectory",12);

    // Test 13
    // forward euler: test 3d motion
    trajectory.reset();
    trajectory.temporalDiscretization=0;
    trajectory.subSteps=1000;
    r_cDirection[0]=0;
    r_cDirection[1]=1;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[2][1]-2.0)<0.01 && abs(trajectory.p[2][0]-1.0)<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01,"IMtrajectory",13);

    // Test 14
    // forward euler: test 3d motion
    trajectory.reset();
    trajectory.temporalDiscretization=0;
    trajectory.subSteps=1000;
    r_cDirection[0]=0;
    r_cDirection[1]=1;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=-1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[2][1]-2.0)<0.01 && abs(trajectory.p[2][0]-(-1.0))<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01,"IMtrajectory",14);
    
    // Test 15
    // forward euler: test 3d motion; change n_0 direction
    n_0[0]=-1;
    n_0[1]=0;
    n_0[2]=0;
    trajectory.reinitialize(p_0, t_0, n_0, 3);
    r_cDirection[0]=-1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=0;
    r_cDirection[1]=1;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[2][1]-(-1.0))<0.01 && abs(trajectory.p[2][0]-2.0)<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01,"IMtrajectory",15);

    // Test 16
    // forward euler: test 3d motion; change n_0 direction
    n_0[0]=0;
    n_0[1]=1;
    n_0[2]=0;
    trajectory.reinitialize(p_0, t_0, n_0, 3);
    r_cDirection[0]=-1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=0;
    r_cDirection[1]=1;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[2][1]-(-2.0))<0.01 && abs(trajectory.p[2][0]-(-1.0))<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01,"IMtrajectory",16);
    
    // Test 17
    // forward euler: test 3d motion; change n_0 direction
    t_0[0]=1;
    t_0[1]=0;
    t_0[2]=0;
    n_0[0]=0;
    n_0[1]=0;
    n_0[2]=1;
    trajectory.reinitialize(p_0, t_0, n_0, 3);
    r_cDirection[0]=-1;
    r_cDirection[1]=0;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    r_cDirection[0]=0;
    r_cDirection[1]=1;
    trajectory.add(10.,PI/2*r_c/10.0,r_c,0,r_cDirection);
    //trajectory.print();
    flag_notPassed=testerInfo(abs(trajectory.p[2][1]-1.0)<0.01 && abs(trajectory.p[2][0]-1.0)<0.01 && abs(trajectory.p[2][2]-(-2.0))<0.01,"IMtrajectory",17);

    return flag_notPassed;
}

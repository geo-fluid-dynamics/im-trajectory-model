//
//  runTests.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 29.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#include "runTests.hpp"

bool isNearlyEqual(double a, double b){
    return (fabs( a - b ) < 1e-5);
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

int runTests(string testName,int testNumber){
    
    bool flag_notPassed=0;
    float P_H, P_W;
    int mMode;
    
    if (testName=="IMLogfile") {
        switch (testNumber) {
            /*case 2:
            {
                // Test 2
                // straight melting with a melting velocity of 1.50096 m/h
                IMlogfile A("../test/simplifiedLogfile/test_sl1.csv",true);
                IMinputFileParser inputFileObj("../test/simplifiedLogfile/test_sl1/test_sl1.ini");
                IMmodel myIMmodel;
                inputFileObj.parseToModel(myIMmodel);
                IMtrajectory trajectory0(myIMmodel.p_0,myIMmodel.t_0,myIMmodel.n_0,A.numberOfDataLines);
                trajectory0.subSteps=myIMmodel.subSteps;
                P_H=0.;
                P_W=0.;
                for (unsigned int i=0; i<A.numberOfDataLines-1; i++) {
                    myIMmodel.meltingMode=IMinterpreter(A.heaterStates[i], &P_H, &P_W);
                    
                    // parse the power values and calculate the melting velocity and curve radius
                    myIMmodel.P_W=P_W;
                    myIMmodel.P_H=P_H;
                    myIMmodel.solve();
                    
                    trajectory0.add(A.timeInSeconds[i+1]-A.timeInSeconds[i],myIMmodel.U_0,myIMmodel.r_c,myIMmodel.tau,myIMmodel.r_cDirection);
                }
                flag_notPassed=abs(trajectory0.p[trajectory0.length-1][2]+1.50096)>0.00001;
                
                break;
            }*/
            default:
                break;
        }
    }else if (testName=="IMInterpreter"){
        switch (testNumber) {
            case 1:
            {
                bool testHeaterStates[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                flag_notPassed=!(IMinterpreter(testHeaterStates, &P_H, &P_W)==-1);
                break;
            }
            case 2:
            {
                bool testHeaterStates[24]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0};
                mMode=IMinterpreter(testHeaterStates, &P_H, &P_W);
                flag_notPassed=!(mMode==0 && isNearlyEqual( P_W, 0.0) && isNearlyEqual( P_H, 2880.0));
                break;
            }
            case 3:
            {
                bool testHeaterStates[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};
                mMode=IMinterpreter(testHeaterStates, &P_H, &P_W);
                flag_notPassed=!(mMode==-1 && isNearlyEqual( P_H, 0.0) && isNearlyEqual( P_W, 0.0));
                break;
            }
            case 4:
            {
                bool testHeaterStates[24]={1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                mMode=IMinterpreter(testHeaterStates, &P_H, &P_W);
                flag_notPassed=!(mMode==0 && isNearlyEqual( P_H, 1440.0) && isNearlyEqual( P_W, 0.0));
                break;
            }
            case 5:
            {
                bool testHeaterStates[24]={1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1};
                mMode=IMinterpreter(testHeaterStates, &P_H, &P_W);
                flag_notPassed=!(mMode==0 && isNearlyEqual( P_H, 1440.0) && isNearlyEqual( P_W, 0.0));
                break;
            }
            case 6:
            {
                bool testHeaterStates[24]={1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0};
                mMode=IMinterpreter(testHeaterStates, &P_H, &P_W);
                flag_notPassed=!(mMode==2 && isNearlyEqual( P_H, 1440.0) && isNearlyEqual( P_W, 2000.0));
                break;
            }
            case 7:
            {
                bool testHeaterStates[24]={0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0};
                mMode=IMinterpreter(testHeaterStates, &P_H, &P_W);
                flag_notPassed=!(mMode==1 && isNearlyEqual( P_H, 1440.0) && isNearlyEqual( P_W, 2000.0));
                break;
            }
            case 8:
            {
                bool testHeaterStates[24]={0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0};
                mMode=IMinterpreter(testHeaterStates, &P_H, &P_W);
                flag_notPassed=!(mMode==3 && isNearlyEqual( P_H, 1440.0) && isNearlyEqual( P_W, 2000.0));
                break;
            }
            case 9:
            {
                bool testHeaterStates[24]={1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1};
                mMode=IMinterpreter(testHeaterStates, &P_H, &P_W);
                flag_notPassed=!(mMode==4 && isNearlyEqual( P_H, 1440.0) && isNearlyEqual( P_W, 2000.0));
                break;
            }

            default:
                break;
        }
    }else if (testName=="IMModel"){
        IMmodel testIMmodel;
        testIMmodel.straightMeltingModel=0;
        testIMmodel.curvilinearMeltingModel=0;
        switch (testNumber) {
            case 1:
            {
                testIMmodel.P_W=4;
                testIMmodel.P_H=0.3;
                testIMmodel.meltingMode=1;
                testIMmodel.solve();
                flag_notPassed=!(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[1], 0.0) && isNearlyEqual( testIMmodel.r_cDirection[0], 1.0));
                break;
            }
            case 2:
            {
                testIMmodel.P_W=4;
                testIMmodel.P_H=0.3;
                testIMmodel.meltingMode=2;
                testIMmodel.solve();
                flag_notPassed=!(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[1], 0.0) && isNearlyEqual( testIMmodel.r_cDirection[0], -1.0));
                break;
            }
            case 3:
            {
                testIMmodel.P_W=4;
                testIMmodel.P_H=0.3;
                testIMmodel.meltingMode=3;
                testIMmodel.solve();
                flag_notPassed=!(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[1], -1.0) && isNearlyEqual( testIMmodel.r_cDirection[0], 0.0));
                break;
            }
            case 4:
            {
                testIMmodel.P_W=4;
                testIMmodel.P_H=0.3;
                testIMmodel.meltingMode=4;
                testIMmodel.solve();
                flag_notPassed=!(isNearlyEqual( testIMmodel.r_c, 1.0) && isNearlyEqual( testIMmodel.r_cDirection[1], 1.0) && isNearlyEqual( testIMmodel.r_cDirection[0], 0.0));
                break;
            }
            case 5:
            {
                testIMmodel.P_W=4;
                testIMmodel.P_H=0.3;
                testIMmodel.c_p_S=2049.41;
                testIMmodel.h_m=333700;
                testIMmodel.rho_S=920;
                testIMmodel.T_m=0;
                testIMmodel.T_S=-20;
                testIMmodel.P_H=1000;
                testIMmodel.H=0.15;
                testIMmodel.solve();
                flag_notPassed=!(abs(testIMmodel.U_0-0.000128932)<0.000000003);
                break;
            }
            default:
                break;
        }
    }else if (testName=="IMTrajectory"){
        IMmodel testIMmodel;
        testIMmodel.straightMeltingModel=0;
        testIMmodel.curvilinearMeltingModel=0;
        testIMmodel.p_0[0]=0;
        testIMmodel.p_0[1]=0;
        testIMmodel.p_0[2]=0;
        testIMmodel.t_0[0]=0;
        testIMmodel.t_0[1]=0;
        testIMmodel.t_0[2]=-1;
        testIMmodel.n_0[0]=1;
        testIMmodel.n_0[1]=0;
        testIMmodel.n_0[2]=0;
        double r_cDirection[2]={1,0};
        testIMmodel.r_cDirection[0]=r_cDirection[0];
        testIMmodel.r_cDirection[1]=r_cDirection[1];
        testIMmodel.U_0=0.1;
        testIMmodel.r_c=100000;
        testIMmodel.tau=0;
        testIMmodel.gravity_vector[0]=0;
        testIMmodel.gravity_vector[1]=0;
        testIMmodel.gravity_vector[2]=-1;
        IMtrajectory trajectory(&testIMmodel,5);

        double r_c=1.0;
        
        switch (testNumber) {
            case 1:
            {
                trajectory.subSteps=0;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(isNearlyEqual(trajectory.p[1][0],0) && isNearlyEqual(trajectory.p[1][1],0) && isNearlyEqual(trajectory.p[1][2],-1));
                break;
            }
            case 2:
            {
                trajectory.subSteps=100;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(trajectory.p[1][0]<4.9506e-6 && trajectory.p[1][1]==0 && abs(trajectory.p[1][2]-(-1))<1e-6);
                break;
            }
            case 3:
            {
                trajectory.temporalDiscretization=1;
                trajectory.subSteps=100;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(trajectory.p[1][0]<5.1496e-6 && trajectory.p[1][1]==0 && abs(trajectory.p[1][2]-(-1))<1e-6);
                break;
            }
            case 4:
            {
                trajectory.temporalDiscretization=0;
                trajectory.subSteps=1000;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;

                trajectory.add(10.,&testIMmodel);
                ///trajectory.print();
                flag_notPassed=!(abs(trajectory.p[1][0]-1.0)<0.01 && abs(trajectory.p[1][1]-0.0)<0.01 && abs(trajectory.p[1][2]-(-1.0))<0.01);
                break;
            }
            case 5:
            {
                trajectory.temporalDiscretization=1;
                trajectory.subSteps=1000;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[1][0]-1.0)<0.01 && abs(trajectory.p[1][1]-0.0)<0.01 && abs(trajectory.p[1][2]-(-1.0))<0.01);
                break;
            }
            case 6:
            {
                trajectory.temporalDiscretization=0;
                trajectory.subSteps=1000;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                testIMmodel.r_cDirection[0]=-1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[1][1]-0.0)<0.01 && abs(trajectory.p[1][0]-(-1.0))<0.01 && abs(trajectory.p[1][2]-(-1.0))<0.01);
                break;
            }
            case 7:
            {
                trajectory.temporalDiscretization=0;
                trajectory.subSteps=1000;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                testIMmodel.r_cDirection[0]=-1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                testIMmodel.r_cDirection[0]=0;
                testIMmodel.r_cDirection[1]=1;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[2][1]-1.0)<0.01 && abs(trajectory.p[2][0]-(-2.0))<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01);
                break;
            }
            case 8:
            {
                trajectory.temporalDiscretization=0;
                trajectory.subSteps=1000;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                testIMmodel.r_cDirection[0]=-1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                testIMmodel.r_cDirection[0]=0;
                testIMmodel.r_cDirection[1]=-1;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[2][1]-(-1.0))<0.01 && abs(trajectory.p[2][0]-(-2.0))<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01);
                break;
            }
            case 9:
            {
                trajectory.temporalDiscretization=0;
                trajectory.subSteps=1000;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                testIMmodel.r_cDirection[0]=1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                testIMmodel.r_cDirection[0]=0;
                testIMmodel.r_cDirection[1]=1;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[2][1]-1.0)<0.01 && abs(trajectory.p[2][0]-2.0)<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01);
                break;
            }
            case 10:
            {
                trajectory.temporalDiscretization=0;
                trajectory.subSteps=1000;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                testIMmodel.r_cDirection[0]=1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                testIMmodel.r_cDirection[0]=0;
                testIMmodel.r_cDirection[1]=-1;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[2][1]-(-1.0))<0.01 && abs(trajectory.p[2][0]-2.0)<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01);
                break;
            }
            case 11:
            {
                trajectory.temporalDiscretization=0;
                trajectory.subSteps=1000;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                testIMmodel.r_cDirection[0]=0;
                testIMmodel.r_cDirection[1]=-1;
                trajectory.add(10.,&testIMmodel);
                testIMmodel.r_cDirection[0]=1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[2][1]-(-2.0))<0.01 && abs(trajectory.p[2][0]-1.0)<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01);
                break;
            }
            case 12:
            {
                trajectory.temporalDiscretization=0;
                trajectory.subSteps=1000;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                testIMmodel.r_cDirection[0]=0;
                testIMmodel.r_cDirection[1]=-1;
                trajectory.add(10.,&testIMmodel);
                testIMmodel.r_cDirection[0]=-1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[2][1]-(-2.0))<0.01 && abs(trajectory.p[2][0]-(-1.0))<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01);
                break;
            }
            case 13:
            {
                trajectory.temporalDiscretization=0;
                trajectory.subSteps=1000;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                testIMmodel.r_cDirection[0]=0;
                testIMmodel.r_cDirection[1]=1;
                trajectory.add(10.,&testIMmodel);
                testIMmodel.r_cDirection[0]=1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[2][1]-2.0)<0.01 && abs(trajectory.p[2][0]-1.0)<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01);
                break;
            }
            case 14:
            {
                trajectory.temporalDiscretization=0;
                trajectory.subSteps=1000;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                testIMmodel.r_cDirection[0]=0;
                testIMmodel.r_cDirection[1]=1;
                trajectory.add(10.,&testIMmodel);
                testIMmodel.r_cDirection[0]=-1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[2][1]-2.0)<0.01 && abs(trajectory.p[2][0]-(-1.0))<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01);
                break;
            }
            case 15:
            {
                trajectory.subSteps=1000;
                testIMmodel.n_0[0]=-1;
                testIMmodel.n_0[1]=0;
                testIMmodel.n_0[2]=0;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                trajectory.reinitialize(&testIMmodel, 3);
                testIMmodel.r_cDirection[0]=-1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                testIMmodel.r_cDirection[0]=0;
                testIMmodel.r_cDirection[1]=1;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[2][1]-(-1.0))<0.01 && abs(trajectory.p[2][0]-2.0)<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01);
                break;
            }
            case 16:
            {
                trajectory.subSteps=1000;
                testIMmodel.n_0[0]=0;
                testIMmodel.n_0[1]=1;
                testIMmodel.n_0[2]=0;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                trajectory.reinitialize(&testIMmodel, 3);
                testIMmodel.r_cDirection[0]=-1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                testIMmodel.r_cDirection[0]=0;
                testIMmodel.r_cDirection[1]=1;
                trajectory.add(10.,&testIMmodel);
                //trajectory.print();
                flag_notPassed=!(abs(trajectory.p[2][1]-(-2.0))<0.01 && abs(trajectory.p[2][0]-(-1.0))<0.01 && abs(trajectory.p[2][2]-(-1.0))<0.01);
                break;
            }
            case 17:
            {
                trajectory.subSteps=1000;
                testIMmodel.t_0[0]=1;
                testIMmodel.t_0[1]=0;
                testIMmodel.t_0[2]=0;
                testIMmodel.n_0[0]=0;
                testIMmodel.n_0[1]=0;
                testIMmodel.n_0[2]=1;
                testIMmodel.U_0=M_PI/2*r_c/10.0;
                testIMmodel.r_c=r_c;
                
                trajectory.reinitialize(&testIMmodel, 3);
                testIMmodel.r_cDirection[0]=-1;
                testIMmodel.r_cDirection[1]=0;
                trajectory.add(10.,&testIMmodel);
                testIMmodel.r_cDirection[0]=0;
                testIMmodel.r_cDirection[1]=1;
                trajectory.add(10.,&testIMmodel);
                trajectory.print();
                flag_notPassed=!(abs(trajectory.p[2][1]-1.0)<0.01 && abs(trajectory.p[2][0]-1.0)<0.01 && abs(trajectory.p[2][2]-(-2.0))<0.01);
                break;
            }
                
            default:
                break;
        }
    }else if (testName=="improvedAnalyticalModel"){
        
        double F_H=1;
        float P_H=1000;
        double rho_S=921.3;
        double rho_L=1000;
        double c_p_S=1877;
        double c_p_L=4200;
        double mu_L=0.0013;
        double k_L=0.6;
        double R=0.06;
        double h_m=333700;
        double T_m=0;
        double T_S=-63;
        double U_0_min=0.000001;
        double U_0_max=1;
        double error=0.000000000001;
        double veloResult;
        
        switch (testNumber) {
            case 1:
                veloResult=improvedAnalyticalModel_velocity(F_H,P_H,rho_S,rho_L,c_p_S,c_p_L,mu_L,k_L,R,h_m,T_m,T_S,U_0_min,U_0_max,error);
                //cout << veloResult*3600 << endl;
                flag_notPassed=!(abs(veloResult*3600-0.6657)<0.0001);
            default:
                break;
        }
    }

    return flag_notPassed;
}

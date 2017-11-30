//
//  improvedAnalyticalModel_velocity.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 28.11.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#include "improvedAnalyticalModel_velocity.hpp"

double improvedAnalyticalModel_velocity(double F_H,float P_H,double rho_S,double rho_L,double c_p_S,double c_p_L,double mu_L,double k_L,double R,double h_m,double T_m,double T_S,double U_0_min,double U_0_max,double error){
    // uses secant search to find the root of f_velo
    
    double a,b,c,e,j_0,j_1;
    
    j_0=M_PI*pow(R,2)*pow(rho_S,2)*c_p_L*(h_m+c_p_S*(T_m-T_S))/(2*k_L)*pow(rho_S/rho_L*3/2*M_PI*pow(R,4)*mu_L/F_H,1.0/3);
    j_1=M_PI*pow(R,2)*rho_S*(h_m+c_p_S*(T_m-T_S));
    
    b=U_0_min;
    c=U_0_max;

    e=error;
    do
    {
        a=b;
        b=c;
        c=b-(b-a)/(f_velo(b,P_H,j_0,j_1)-f_velo(a,P_H,j_0,j_1))*f_velo(b,P_H,j_0,j_1);
        //cout<<"\nThe root of the equation is "<<c*3600<<endl;
        if (f_velo(c,P_H,j_0,j_1)==0)
        {
            return c;
        }
    }while(abs(c-b)>=e);    //check if the error is greater than the desired accuracy
    
    return c;
}

double f_velo(double U_0,float P_H, double j_0, double j_1){
    //return pow(U_0,2)-2;
    return j_0*pow(U_0,7.0/3)+j_1*U_0-P_H;
}

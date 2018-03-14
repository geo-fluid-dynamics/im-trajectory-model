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
    
    double a,b,c,e,gammaWithoutVelocity,P_CWithoutVelocity;
    
    double alpha_L=k_L/rho_L/c_p_L;
    
    gammaWithoutVelocity=pow(rho_S*R/rho_L,4./3.)*pow(3*M_PI*mu_L/2.0/F_H, 1./3.)/20.0/alpha_L;
    P_CWithoutVelocity=M_PI*R*R*rho_S*(h_m+c_p_S*(T_m-T_S));
    
    b=U_0_min;
    c=U_0_max;

    e=error;
    do
    {
        a=b;
        b=c;

        c=b-(b-a)/(f_velo(b,P_H,gammaWithoutVelocity,P_CWithoutVelocity)-f_velo(a,P_H,gammaWithoutVelocity,P_CWithoutVelocity))*f_velo(b,P_H,gammaWithoutVelocity,P_CWithoutVelocity);
        //cout<<"\nThe root of the equation is "<<c*3600<<endl;
        if (f_velo(c,P_H,gammaWithoutVelocity,P_CWithoutVelocity)==0)
        {
            return c;
        }
    }while(abs(c-b)>=e);    //check if the error is greater than the desired accuracy
    
    return c;
}

double f_velo(double U_0,float P_H, double gammaWithoutVelocity, double P_CWithoutVelocity){
    return (7*gammaWithoutVelocity*pow(U_0,4./3.)+1)*P_CWithoutVelocity*U_0-(1-3*gammaWithoutVelocity*pow(U_0,4./3.))*P_H;
}

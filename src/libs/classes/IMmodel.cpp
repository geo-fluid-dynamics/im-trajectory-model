//
//  IMmodel.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 26.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//
#include "IMmodel.hpp"
IMmodel::IMmodel(){
    rho_S=920;                // solid PCM density [kg/m^3]
    rho_L=1000;               // liquid PCM density [kg/m^3]
    h_m=333700;               // latent heat of melting [J/kg]
    T_m=0;                    // melting temperature [degC]
    T_S=0;                    // ice temperature [degC]
    c_p_S=2049.41;            // solid PCM specific heat capacity [J/(kg*K)]
    c_p_L=4222.22;            // liquid PCM specific heat capacity [J/(kg*K)]
    k_L=0.57;                 // liquid PCM thermal conductivity [W/(m*K)]
    k_S=2.18;                 // solid PCM thermal conductivity [W/(m*K)]
    mu_L=0.0013;              // dynamic viscosity [Ns/(m^2)]
    r_cStraight=10000;        // curve radius which is used to mimic straight melting [m]
    L=2;                      // length of the IceMole [m]
    H=0.15;                   // width of the IceMole [m]
    F_H=1000;                 // exerted force [N]
    mass=0;
    weight=0;
    straightMeltingModel=0;   // 0: simple energy balance
    curvilinearMeltingModel=0;// 0: simple energy balance
    r_cDirection[0]=1;
    r_cDirection[1]=0;
    tau=0;
    subSteps=0;
    temporalDiscretization=0;
    subStepsRecalcVelocity=0;
    n_0[0]=1;
    n_0[1]=0;
    n_0[2]=0;
    t_0[0]=0;
    t_0[1]=0;
    t_0[2]=-1;
    current_t_vector[0]=0;
    current_t_vector[1]=0;
    current_t_vector[2]=0;
    p_0[0]=0;
    p_0[1]=0;
    p_0[2]=0;
    gravity_vector[0]=0;
    gravity_vector[1]=0;
    gravity_vector[2]=-1;
}

void IMmodel::solve(){
    
    recalculateVecocity();

    switch (meltingMode) {
        case 1:
            r_cDirection[0]=1;
            r_cDirection[1]=0;
            break;
        case 2:
            r_cDirection[0]=-1;
            r_cDirection[1]=0;
            break;
        case 3:
            r_cDirection[0]=0;
            r_cDirection[1]=-1;
            break;
        case 4:
            r_cDirection[0]=0;
            r_cDirection[1]=1;
            break;
            
        default:
            break;
    }
    
    switch (curvilinearMeltingModel) {
        case 0:
            if (P_W!=0 && P_H!=0) { // if not straight melting; make also sure that there will be no division by zero when P_H is equal to zero
                r_c = P_H / P_W * L*L/(2*H);
                
            }else{
                r_c = r_cStraight;
            }
            break;
            
        default:
            break;
    }
}

void IMmodel::recalculateVecocity() {
    
    double R=H*sqrt(1.0/M_PI); // calculate R which has the same area than H*H
    double U_0_min=0.00000001;
    double U_0_max=0.1;
    double error=0.00000000001;
    
    switch (straightMeltingModel) {
        case 0:
            U_0=P_H/(H * H * rho_S * (h_m + c_p_S * (T_m - T_S)));
            break;
        case 1: // improved analytical solution
            if(P_H==0){
                U_0=0;
            }else{
                
                double gravity=sqrt(gravity_vector[0]*gravity_vector[0]+gravity_vector[1]*gravity_vector[1]+gravity_vector[2]*gravity_vector[2]);
                // calculate the angle in rad between the gravity vector and the tangent
                double phi=acos(current_t_vector[0]*gravity_vector[0]+current_t_vector[1]*gravity_vector[1]+current_t_vector[2]*gravity_vector[2]/(sqrt(current_t_vector[0]*current_t_vector[0]+current_t_vector[1]*current_t_vector[1]+current_t_vector[2]*current_t_vector[2])*gravity));
                
                
                weight=mass*gravity*cos(phi);
                
                U_0=improvedAnalyticalModel_velocity(F_H+weight,P_H,rho_S,rho_L,c_p_S,c_p_L,mu_L,k_L,R,h_m,T_m,T_S,U_0_min,U_0_max,error);
            }
            break;
        default:
            break;
    }
}

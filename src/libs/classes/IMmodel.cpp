//
//  IMmodel.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 26.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//
#include "IMmodel.hpp"
IMmodel::IMmodel(){
    this->rho_S=920;                // solid PCM density [kg/m^3]
    this->rho_L=1000;               // liquid PCM density [kg/m^3]
    this->h_m=333700;               // latent heat of melting [J/kg]
    this->T_m=0;                    // melting temperature [degC]
    this->T_S=0;                    // ice temperature [degC]
    this->c_p_S=2049.41;            // solid PCM specific heat capacity [J/(kg*K)]
    this->c_p_L=4222.22;            // liquid PCM specific heat capacity [J/(kg*K)]
    this->k_L=0.57;                 // liquid PCM thermal conductivity [W/(m*K)]
    this->k_S=2.18;                 // solid PCM thermal conductivity [W/(m*K)]
    this->mu_L=0.0013;              // dynamic viscosity [Ns/(m^2)]
    this->r_cStraight=10000;        // curve radius which is used to mimic straight melting [m]
    this->L=2;                      // length of the IceMole [m]
    this->H=0.15;                   // width of the IceMole [m]
    this->F_H=1000;                 // exerted force [N]
    this->straightMeltingModel=0;   // 0: simple energy balance
    this->curvilinearMeltingModel=0;// 0: simple energy balance
    this->r_cDirection[0]=1;
    this->r_cDirection[1]=0;
    this->tau=0;
    this->subSteps=0;
    this->temporalDiscretization=0;
    this->n_0[0]=1;
    this->n_0[1]=0;
    this->n_0[2]=0;
    this->t_0[0]=0;
    this->t_0[1]=0;
    this->t_0[2]=-1;
    this->p_0[0]=0;
    this->p_0[1]=0;
    this->p_0[2]=0;
}

void IMmodel::solve(){
    
    double R=this->H*sqrt(1.0/M_PI); // calculate R which has the same area than H*H
    double U_0_min=0.00000001;
    double U_0_max=0.1;
    double error=0.00000000001;
    
    switch (this->straightMeltingModel) {
        case 0:
            this->U_0=this->P_H/(this->H * this->H * this->rho_S * (this->h_m + this->c_p_S * (this->T_m - this->T_S)));
            break;
        case 1: // improved analytical solution
            if(this->P_H==0){
                this->U_0=0;
            }else{
                this->U_0=improvedAnalyticalModel_velocity(this->F_H,this->P_H,this->rho_S,this->rho_L,this->c_p_S,this->c_p_L,this->mu_L,this->k_L,R,this->h_m,this->T_m,this->T_S,U_0_min,U_0_max,error);
            }
            break;
        default:
            break;
    }

    switch (this->meltingMode) {
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
    
    switch (this->curvilinearMeltingModel) {
        case 0:
            if (this->P_W!=0 && this->P_H!=0) { // if not straight melting; make also sure that there will be no division by zero when P_H is equal to zero
                this->r_c = this->P_H / this->P_W * this->L*this->L/(2*this->H);
                
            }else{
                this->r_c = this->r_cStraight;
            }
            break;
            
        default:
            break;
    }
}

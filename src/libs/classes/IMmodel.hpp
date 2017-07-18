//
//  IMmodel.hpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 26.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#ifndef IMmodel_hpp
#define IMmodel_hpp

#include <iostream>
using namespace std;

class IMmodel {

public:
    double rho_S;                   // solid PCM density [kg/m^3]
    double rho_L;                   // liquid PCM density [kg/m^3]
    double h_m;                     // latent heat of melting [J/kg]
    double T_m;                     // melting temperature [degC]
    double T_S;                     // ice temperature [degC]
    double c_p_S;                   // solid PCM specific heat capacity [J/(kg*K)]
    double c_p_L;                   // liquid PCM specific heat capacity [J/(kg*K)]
    double k_L;                     // liquid PCM thermal conductivity [W/(m*K)]
    double k_S;                     // solid PCM thermal conductivity [W/(m*K)]
    float P_H;                      // melting head power [W]
    float P_W;                      // active wall power [W]
    float U_0;                      // melting velocity [m/s]
    double r_cDirection[3];         // curve radius direction [-]
    double r_c;                      // curve radius value [m]
    double r_cStraight;              // curve radius which is used to mimic straight melting [m]
    double L;                       // length of the IceMole [m]
    double H;                       // width of the IceMole [m]
    int meltingMode;
    int straightMeltingModel;       // 0: simple energy balance
    int curvilinearMeltingModel;    // 0: simple energy balance
    double p_0[3];                  // initial position vector
    double t_0[3];                  // initial tangent vector
    double n_0[3];                  // initial normal vector
    double tau;
    int subSteps;
    int temporalDiscretization;
    double c1_tau;                  // polynomial coefficients for linear equation for tau
    double c2_tau;                  // polynomial coefficients for linear equation for tau
    IMmodel();
    void solve();
};

#endif /* IMmodel_hpp */

//
//  IMinputFileParser.hpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 04.06.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#ifndef IMinputFileParser_hpp
#define IMinputFileParser_hpp

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "../functions/utilities.hpp"
#include "../classes/IMmodel.hpp"
#include <cmath>
using namespace std;


class IMinputFileParser {
private:
    void setAllflagsToFalse(void);
    template <typename T>
    void useDataFromInputFile(bool, T*, T* ,string , string );
    
public:
    bool isValidInputFile;
    double rho_S;                // solid PCM density [kg/m^3]
    bool flag_rho_S;
    double rho_L;                // liquid PCM density [kg/m^3]
    bool flag_rho_L;
    double h_m;                  // latent heat of melting [J/kg]
    bool flag_h_m;
    double T_m;                  // melting temperature [degC]
    bool flag_T_m;
    double T_S;                  // ice temperature [degC]
    bool flag_T_S;
    double c_p_S;                // solid PCM specific heat capacity [J/(kg*K)]
    bool flag_c_p_S;
    double c_p_L;                // liquid PCM specific heat capacity [J/(kg*K)]
    bool flag_c_p_L;
    double k_L;                  // liquid PCM thermal conductivity [W/(m*K)]
    bool flag_k_L;
    double k_S;                  // solid PCM thermal conductivity [W/(m*K)]
    bool flag_k_S;
    double L;                    // length of the IceMole [m]
    bool flag_L;
    double H;                    // width of the IceMole [m]
    bool flag_H;
    double c1_tau;
    bool flag_c1_tau;
    double c2_tau;
    bool flag_c2_tau;
    double t_0[3];              // initial tangent
    bool flag_t_0;
    double n_0[3];              // initial normal
    bool flag_n_0;
    int subSteps;
    bool flag_subSteps;
    double r_cStraight;
    bool flag_r_cStraight;
    int temporalDiscretization;
    bool flag_temporalDiscretization;
    
    string filename;
    unsigned int numberOfLines;
    
    IMinputFileParser(string filename);
    void parseToModel(IMmodel&);
};

#endif /* IMinputFileParser_hpp */

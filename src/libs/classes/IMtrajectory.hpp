//
//  IMtrajectory.hpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 26.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//
// Please note that r_cDirection must be always like (1 0 0), (-1 0 0), (0 1 0) and so on

#ifndef IMtrajectory_hpp
#define IMtrajectory_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../classes/IMmodel.hpp"

using namespace std;

class IMtrajectory {
    unsigned int pos;
    
public:
    double p_0[3];              // initial position in x-, y- and z-direction
    double t_0[3];              // initial tangent vector
    double n_0[3];              // initial normal vector
    double** p;                 // positions for all times
    double** t;                 // tangents for all times
    double** n;                 // normals for all times
    double** n_fixed;           // fixed normals for all times
    double* times;              // time points (starts at 0)
    double* distance;           // total moltmen distance (starts at 0)
    bool flagCalcDistance;      // if flag is zero than the distance will not be calculated
                                // in the output in distance column, all values will be set to -1
    unsigned int length;        // the length positions
    unsigned int subSteps;      // the frenet-serret equations are solved subSteps+1 times for each time
    int temporalDiscretization; // 0: forward; 1: backward
    double r_cDirection_0[2];   // initial direction of curve radius
    double r_cDirection[2];     // last r_cDirection
    double gravity_vector[3];
    
    IMtrajectory(double* p_0, double* t_0, double* n_0, unsigned int length, double* gravity_vector);
    void add(double,IMmodel*);
    void writeToDisk(string);   //!< writes the solution to disk
    void reset(void);
    void reinitialize(double* p_0, double* t_0, double* n_0, unsigned int length, double* gravity_vector);
    void print(void);
};

#endif /* IMtrajectory_hpp */

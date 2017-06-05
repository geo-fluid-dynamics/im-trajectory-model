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
using namespace std;

class IMtrajectory {
    unsigned int pos;
    
public:
    double p_0[3];          // initial position in x-, y- and z-direction
    double t_0[3];          // initial tangent vector
    double n_0[3];          // initial normal vector
    double** p;             // positions for all times
    double** t;             // tangents for all times
    double** n;             // normals for all times
    double* times;          // time points (starts at 0)
    unsigned int length;    // the length positions
    unsigned int subSteps;  // the frenet-serret equations are solved subSteps+1 times for each time
    int method;             // 0: forward; 1: backward
    double r_cDirection_0[3];// initial direction of curve radius
    double r_cDirection[3];  // last r_cDirection
    
    IMtrajectory(double* p_0, double* t_0, double* n_0,double* r_cDirection_0, unsigned int length);
    void add(double,double,double,double,double*);
    void writeToDisk(string);   //!< writes the solution to disk
    void reset(void);
};

#endif /* IMtrajectory_hpp */

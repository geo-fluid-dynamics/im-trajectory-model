//
//  IMinterpreter.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 26.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#include "IMinterpreter.hpp"

/*
 ----------------------------------------------------------------------------------
 Below you can see the numbering of the heating cartridges.
 The ice screw points in positive X direction (which follows from right hand rule)
 
      15  16  1   2
   +-----------------+              +-------------+
 14| \ \  |   |  / / | 3            |\      \      \
   |  \ \ |   | / /  |              | \      \      \
 13| ---         --- | 4            |  \      \      \
   |        O        |----->        |   \      \      \ ------>
 12| ---         --- | 5  Y+        |\   \   8  \   1  \     Y+
   |  / / |   | \ \  |              | \   \      \      \
 11| / /  |   |  \ \ | 6            |  \   \      \      \
   +-----------------+              +   \ 7 \      \      \
    10    9 | 8   7                  \   \   \      \      \
            |                         \   \   \      \      \
            |                          \   \   \      \      \
            V Z+                        \ 6 \   \~~~~~~\~~~~~~\
                                         \   \  $             $
                                          \   \ $             $
                                           \   \$             $
                                            \   $             $
                                             \  $             $
                                              \ $             $
                                               \$~~~~~~~~~~~~~$
 
 HeatingCartridgeNumber | Power [W]
 ----------------------------------
 1,4,5,8,9,12,13,16     |       200
            all others  |       160
 
 WallPower 8 x 1000 W
 ----------------------------------------------------------------------------------
 */

int IMinterpreter(bool* heaterStates,float *P_H,float *P_W){
    
    /*
     Inputs/Outputs:
     heaterStates:   A 1d array with 24 bool values, i.e. for each heater (head and wall) one on-off state
     return value:   The melting mode
                       -1: stagnation
                        0: straight melting
                        1: curvilinear melting Z+
                        2: curvilinear melting Z-
                        3: curvilinear melting Y+
                        4: curvilinear melting Y-
     P_H:            The total power in the melting head
     P_W:            The power at the wall that drives curvilinear melting
     */
    
    float D;
    float wallHeaterPower=1000;
    int mMode=0;
    float headHeaterPowerMat[2][2]={0};
    float powerDistr[4]={200,160,160,200};  // the power distribution in each head corner
    bool wallHeaterStates[8]={0};
    
    for (unsigned int i=0; i<8; i++) {
        wallHeaterStates[i]=heaterStates[i+16];
        //cout << wallHeaterStates[i];
    }
    
    // we summarize the heating cartridges in each corner to get 4 power values in total
    for (unsigned int i=0; i<4; i++) {
        headHeaterPowerMat[0][0]=headHeaterPowerMat[0][0]+heaterStates[i+12]*powerDistr[i];
        headHeaterPowerMat[1][1]=headHeaterPowerMat[1][1]+heaterStates[i+4]*powerDistr[i];
        headHeaterPowerMat[0][1]=headHeaterPowerMat[0][1]+heaterStates[i]*powerDistr[i];
        headHeaterPowerMat[1][0]=headHeaterPowerMat[1][0]+heaterStates[i+8]*powerDistr[i];
    }
    
    // calculate the sum of power inside of the melting head
    *P_H=headHeaterPowerMat[0][0]+headHeaterPowerMat[0][1]+headHeaterPowerMat[1][0]+headHeaterPowerMat[1][1];
    
    if(headHeaterPowerMat[0][0]==headHeaterPowerMat[1][1] || headHeaterPowerMat[1][0]==headHeaterPowerMat[0][1]){
        mMode=0;
    }else{
        mMode=1;
    }
    
    D=abs(headHeaterPowerMat[0][0]+headHeaterPowerMat[1][0]-(headHeaterPowerMat[0][1]+headHeaterPowerMat[1][1]))
    -abs(headHeaterPowerMat[0][0]+headHeaterPowerMat[0][1]-(headHeaterPowerMat[1][0]+headHeaterPowerMat[1][1]));
    
    if (D>0) {
        if(headHeaterPowerMat[0][0]+headHeaterPowerMat[1][0]>headHeaterPowerMat[0][1]+headHeaterPowerMat[1][1]){ // Z+
            mMode=1;
            *P_W=wallHeaterPower*(wallHeaterStates[5]+wallHeaterStates[6]);
        }else if(headHeaterPowerMat[0][0]+headHeaterPowerMat[1][0]<headHeaterPowerMat[0][1]+headHeaterPowerMat[1][1]){// % Z-
            mMode=2;
            *P_W=wallHeaterPower*(wallHeaterStates[1]+wallHeaterStates[2]);
        }
    }else if (D<0){
        if(headHeaterPowerMat[1][1]+headHeaterPowerMat[1][0]>headHeaterPowerMat[0][0]+headHeaterPowerMat[0][1]){ // Y+
            mMode=3;
            *P_W=wallHeaterPower*(wallHeaterStates[3]+wallHeaterStates[4]);
        }else if(headHeaterPowerMat[1][1]+headHeaterPowerMat[1][0]<headHeaterPowerMat[0][0]+headHeaterPowerMat[0][1]){// % Y-
            mMode=4;
            *P_W=wallHeaterPower*(wallHeaterStates[0]+wallHeaterStates[7]);
        }
    }
    
    // in the current release we assume that curvilinear melting
    // is only possible if P_W>0 - might be changed in the future
    if (*P_W==0) {
        mMode=0;
    }
    
    if (*P_H==0){
        mMode=-1; // stagnation
    }
    
    return mMode;
}

//
//  IMtrajectory.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 26.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#include "IMtrajectory.hpp"

// matrix multiplication of a 9x9 matrix
void MatrixMultiply(double A1[9][9], double A2[9][9],double Sol[9][9]){
    unsigned int i=0;
    unsigned int j=0;
    unsigned int k=0;

    for (i=0; i<9; i++) {
        for (k=0; k<9; k++) {
            Sol[i][k] = 0;
        }
    }
    
    for (i=0; i<9; i++) {
        for (j=0; j<9; j++) {
            for (k=0; k<9; k++) {
                Sol[i][j] += A1[i][k] * A2[k][j];
            }
        }
    }
}

// LU decomposition for a 9x9 matrix
void LUdecompose(double A[9][9], double l[9][9], double u[9][9]){
    int i = 0, j = 0, k = 0;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (j < i)
                l[j][i] = 0;
            else
            {
                l[j][i] = A[j][i];
                for (k = 0; k < i; k++)
                {
                    l[j][i] = l[j][i] - l[j][k] * u[k][i];
                }
            }
        }
        for (j = 0; j < 9; j++)
        {
            if (j < i)
                u[i][j] = 0;
            else if (j == i)
                u[i][j] = 1;
            else
            {
                u[i][j] = A[i][j] / l[i][i];
                for (k = 0; k < i; k++)
                {
                    // this is the wrong way to calculate lu
                    u[i][j] = u[i][j] - ((l[i][k] * u[k][j]) / l[i][i]);
                }
            }
        }
    }
}

// solve the linear system
void LUevaluate(double l[9][9], double u[9][9], double b[9], double x[9]){
    // Ax = b -> LUx = b. Then y is defined to be Ux
    int i = 0;
    int j = 0;
    double y[9]={0};
    // Forward solve Ly = b
    for (i = 0; i < 9; i++)
    {
        y[i] = b[i];
        for (j = 0; j < i; j++)
        {
            y[i] -= l[i][j] * y[j];
        }
        y[i] /= l[i][i];
    }
    // Backward solve Ux = y
    for (i = 9 - 1; i >= 0; i--)
    {
        x[i] = y[i];
        for (j = i + 1; j < 9; j++)
        {
            x[i] -= u[i][j] * x[j];
        }
        x[i] /= u[i][i];
    }
}

IMtrajectory::IMtrajectory(double* p_0, double* t_0, double* n_0,unsigned int length){
    this->subSteps=0;
    this->temporalDiscretization=0; // forward euler
    IMtrajectory::reinitialize(p_0, t_0, n_0, length);
}

void IMtrajectory::add(double dt,double U_0, double r_c, double tau, double* r_cDirection){
    unsigned int newPos=this->pos;
    unsigned int oldPos=newPos-1;
    
    double b[3];
    double bLastIter[3]={0};
    double pOld[3];
    double pNew[3];
    double tOld[3];
    double tNew[3];
    double nOld[3];
    double nNew[3];
    double A[9][9]={0};
    double l[9][9]={0};
    double u[9][9]={0};
    double x[9]={0};
    double rhs[9]={0};
    double epsilon=0;
    
    
    
    double dtSub=dt/(this->subSteps+1);
    this->times[newPos]=this->times[oldPos]+dt;
    
    for (unsigned int i=0; i<3; i++) {
        pOld[i]=this->p[oldPos][i];
        tOld[i]=this->t[oldPos][i];
        nOld[i]=this->n[oldPos][i];
    }
    
    
    // check if r_cDirection has changed -> eventually we have to flip the normal
    if (this->r_cDirection[0] != r_cDirection[0] || this->r_cDirection[1] != r_cDirection[1]) {
        if ((this->r_cDirection[0] == -r_cDirection[0] && this->r_cDirection[1] == r_cDirection[1]) || (this->r_cDirection[0] == r_cDirection[0] && this->r_cDirection[1] == -r_cDirection[1])) {
            for (unsigned int k=0; k<3; k++) {
                b[k]=-nOld[k];
            }
        }else
            if ((this->r_cDirection[0]==-1 && r_cDirection[0]==0 && this->r_cDirection[1]==0 && r_cDirection[1]==1) ||
                (this->r_cDirection[0]==1 && r_cDirection[0]==0 && this->r_cDirection[1]==0 && r_cDirection[1]==-1) ||
                (this->r_cDirection[0]==0 && r_cDirection[0]==-1 && this->r_cDirection[1]==-1 && r_cDirection[1]==0) ||
                (this->r_cDirection[0]==0 && r_cDirection[0]==1 && this->r_cDirection[1]==1 && r_cDirection[1]==0)) {
            b[0]=tOld[1]*nOld[2]-tOld[2]*nOld[1];
            b[1]=tOld[2]*nOld[0]-tOld[0]*nOld[2];
            b[2]=tOld[0]*nOld[1]-tOld[1]*nOld[0];
            this->flag_r_c_direction_has_changed=1;
        }else{
            b[0]=tOld[2]*nOld[1]-tOld[1]*nOld[2];
            b[1]=tOld[0]*nOld[2]-tOld[2]*nOld[0];
            b[2]=tOld[1]*nOld[0]-tOld[0]*nOld[1];
            this->flag_r_c_direction_has_changed=0;
        }

            
        for (unsigned int k=0; k<3; k++) {
            nOld[k]=b[k];
        }
        // save the new direction for the next add
        this->r_cDirection[0]=r_cDirection[0];
        this->r_cDirection[1]=r_cDirection[1];
    }
    
    for (unsigned int i=0; i<this->subSteps+1; i++) {
        
        b[0]=tOld[1]*nOld[2]-tOld[2]*nOld[1];
        b[1]=tOld[2]*nOld[0]-tOld[0]*nOld[2];
        b[2]=tOld[0]*nOld[1]-tOld[1]*nOld[0];
        
        for (unsigned int k=0; k<3; k++) {
            if(this->temporalDiscretization==0){ // forward euler
                pNew[k]=pOld[k] + dtSub * U_0 * tOld[k];
                tNew[k]=tOld[k] + dtSub * U_0/r_c * nOld[k];
                nNew[k]=nOld[k] + dtSub * U_0 * ( tau * b[k] - tOld[k]/r_c);
            }else if(this->temporalDiscretization==1){ // implicit euler
                for(unsigned int m=0;m<9;m++){
                    A[m][m]=1;
                }
                for(unsigned int m=3;m<6;m++){
                    A[m-3][m]=-dtSub*U_0;
                }
                for(unsigned int m=6;m<9;m++){
                    A[m-3][m]=-dtSub*U_0/r_c;
                }
                for(unsigned int m=3;m<6;m++){
                    A[m+3][m]=dtSub*U_0 /r_c;
                }
                
                // linearization of the cross product for torsion
                for (unsigned int j=0; j<1000; j++) {
                    for(unsigned int m=0;m<3;m++){
                        rhs[m]=pOld[m];
                        rhs[m+3]=tOld[m];
                        rhs[m+6]=nOld[m]+tau*U_0*dtSub*b[m];
                    }
                    LUdecompose(A, l, u);
                    LUevaluate(l,u,rhs,x);
                    
                    for (unsigned int ii=0; ii<3; ii++) {
                        bLastIter[ii]=b[ii];
                    }
                    
                    b[0]=x[4]*x[8]-x[5]*x[7];
                    b[1]=x[5]*x[6]-x[3]*x[8];
                    b[2]=x[3]*x[7]-x[4]*x[6];
                    epsilon=1e-10;
                    if (abs(b[0]-bLastIter[0])<epsilon && abs(b[1]-bLastIter[1])<epsilon && abs(b[2]-bLastIter[2])<epsilon) {
                        break;
                    }
                }
                
                // uncomment to test l u decomposition
                /*
                 double Sol[9][9];
                 MatrixMultiply(l,u,Sol);
                 for(unsigned int m=0;m<9;m++){
                    //cout << x[m];
                    for (unsigned int n=0; n<9; n++) {
                        cout << Sol[m][n]-A[m][n] << " ";
                    }
                    cout << endl;
                }*/
                for(unsigned int m=0;m<3;m++){
                    pNew[m]=x[m];
                    tNew[m]=x[m+3];
                    nNew[m]=x[m+6];
                }
                
            }
            
            pOld[k]=pNew[k];
            tOld[k]=tNew[k];
            nOld[k]=nNew[k];
            
        }
    }
    
    for (unsigned int i=0; i<3; i++) {
        this->p[newPos][i]=pNew[i];
        this->t[newPos][i]=tNew[i];
        this->n[newPos][i]=nNew[i];
    }
    
    this->pos++;
}

void IMtrajectory::writeToDisk(string filename){
    ofstream myfile (filename);
    if (myfile.is_open())
    {
        myfile << "time px py pz tx ty tz nx ny nz y+_x y+_y y+_z" << endl;
        for (unsigned int i=0; i<this->length; i++) {
            myfile << this->times[i] << " "
                   << this->p[i][0] << " "
                   << this->p[i][1] << " "
                   << this->p[i][2] << " "
                   << this->t[i][0] << " "
                   << this->t[i][1] << " "
                   << this->t[i][2] << " "
                   << this->n[i][0] << " "
                   << this->n[i][1] << " "
                   << this->n[i][2] << endl;
        }
        myfile.close();
    }
    else cout << "Unable to open file";
}

void IMtrajectory::reset(void){
    this->pos=1;
    for (unsigned int i=0; i<3; i++) {
        this->r_cDirection[i]=this->r_cDirection_0[i];
    }
}

void IMtrajectory::reinitialize(double* p_0, double* t_0, double* n_0, unsigned int length){
    this->flag_r_c_direction_has_changed=0;
    
    this->length=length;
    
    this->times = new double[this->length];
    this->times[0]=0;
    
    this->p = new double*[this->length];
    this->t = new double*[this->length];
    this->n = new double*[this->length];
    
    this->r_cDirection_0[0]=1;
    this->r_cDirection_0[1]=0;
    for (unsigned int i=0; i<2; i++) {
        this->r_cDirection[i]=this->r_cDirection_0[i];
    }
    
    for (unsigned int i=0; i<3; i++) {
        this->p_0[i]=p_0[i];
        this->t_0[i]=t_0[i];
        this->n_0[i]=n_0[i];
    }
    
    for(unsigned int i = 0; i < this->length; i++){
        this->p[i] = new double[3];
        this->t[i] = new double[3];
        this->n[i] = new double[3];
    }
    for (unsigned int i=0; i<3; i++) {
        this->p[0][i]=this->p_0[i];
        this->t[0][i]=this->t_0[i];
        this->n[0][i]=this->n_0[i];
    }
    this->pos=1;
}

void IMtrajectory::print(void){
    for (unsigned int i=0; i<this->length; i++) {
        printf("p[%i] = {%.5f,%.5f,%.5f},\t t[%i] = {%.5f,%.5f,%.5f},\t n[%i] = {%.5f,%.5f,%.5f}\n",i,p[i][0],p[i][1],p[i][2],i,t[i][0],t[i][1],t[i][2],i,n[i][0],n[i][1],n[i][2]);
    }
}

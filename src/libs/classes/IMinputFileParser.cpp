//
//  IMinputFileParser.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 04.06.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#include "IMinputFileParser.hpp"

// Purpose: Add a value of a IMinputFileParser object to an IMmodel object
template <typename T>
void IMinputFileParser::useDataFromInputFile(bool flag, T* inputFileValue, T* modelValue,string filename, string variablename){
    if (flag) {
        *modelValue=*inputFileValue;
    }else{
        cout << "WARNING: " << variablename << " was not found in " << filename << ". Using the default value " << *modelValue << " instead." << endl;
    }
}

void IMinputFileParser::setAllflagsToFalse(void){
    flag_rho_S=0;
    flag_rho_L=0;
    flag_h_m=0;
    flag_T_m=0;
    flag_T_S=0;
    flag_c_p_S=0;
    flag_c_p_L=0;
    flag_k_L=0;
    flag_k_S=0;
    flag_L=0;
    flag_H=0;
    flag_tau=0;
    flag_subSteps=0;
    flag_r_cStraight=0;
    flag_temporalDiscretization=0;
    subStepsRecalcVelocity=0;
    flag_straightMeltingModel=0;
    flag_F_H=0;
    flag_mass=0;
    flag_mu_L=0;
    flag_gravity_vector=0;
    flag_n_0=0;
    flag_t_0=0;
}

IMinputFileParser::IMinputFileParser(string filename){
    isValidInputFile=1;
    string content;
    string keyword;
    string valueStr;
    double value;
    unsigned int keywordBegin=0;
    unsigned int keywordEnd=0;
    unsigned int valueBegin=0;
    unsigned int valueEnd=0;
    
    unsigned int i,k,m;
    
    setAllflagsToFalse();
    
    filename=filename;
    string line;
    ifstream myfile (filename.c_str());
    if (myfile.is_open())
    {
        numberOfLines=0;
        while ( getline (myfile,line) )
        {
            numberOfLines++;
            //cout << line << '\n';
        }
        
        myfile.clear();
        myfile.seekg(0, myfile.beg);
        
        for (i=0; i<numberOfLines; i++) {
            getline (myfile,line);
            content=line;
            
            if(content[0]=='/' && content[1]=='/'){
                continue;
            }
            
            for (k=0; k<content.length(); k++) {
                if(content[k]=='='){
                    for (m=0; m<k; m++) {
                        if (content[m]!=' ') {
                            keywordBegin=m;
                            break;
                        }
                    }
                    for (m=keywordBegin; m<=k; m++) {
                        if (content[m]==' ' || content[m]=='=') {
                            keywordEnd=m;
                            break;
                        }
                    }
                    keyword = content.substr(keywordBegin,keywordEnd-keywordBegin);
                    
                    for (m=k+1; m<content.length(); m++) {
                        if (content[m]!=' ') {
                            valueBegin=m;
                            break;
                        }
                    }
                    valueEnd=(unsigned int)content.length();
                    for (m=valueBegin+1; m<content.length(); m++) {
                        if (content[m]==' ' || content[m]=='\n' || content[m]==';' || content[m]=='/') {
                            valueEnd=m;
                            break;
                        }
                    }
                    valueStr = content.substr(valueBegin,valueEnd-valueBegin);
                    value=atof(valueStr.c_str());
                    
                    // Now compare keyword to check if we found variable names
                    if (!keyword.compare("rho_S")) {
                        rho_S=value;
                        flag_rho_S=1;
                    }else if (!keyword.compare("rho_L")){
                        rho_L=value;
                        flag_rho_L=1;
                    }else if (!keyword.compare("k_S")){
                        k_S=value;
                        flag_k_S=1;
                    }else if (!keyword.compare("k_L")){
                        k_L=value;
                        flag_k_L=1;
                    }else if (!keyword.compare("c_p_S")){
                        c_p_S=value;
                        flag_c_p_S=1;
                    }else if (!keyword.compare("c_p_L")){
                        c_p_L=value;
                        flag_c_p_L=1;
                    }else if (!keyword.compare("h_m")){
                        h_m=value;
                        flag_h_m=1;
                    }else if (!keyword.compare("T_m")){
                        T_m=value;
                        flag_T_m=1;
                    }else if (!keyword.compare("T_S")){
                        T_S=value;
                        flag_T_S=1;
                    }else if (!keyword.compare("L")){
                        L=value;
                        flag_L=1;
                    }else if (!keyword.compare("H")){
                        H=value;
                        flag_H=1;
                    }else if (!keyword.compare("tau")){
                        tau=value;
                        flag_tau=1;
                    }else if (!keyword.compare("subSteps")){
                        subSteps=value;
                        flag_subSteps=1;
                    }else if (!keyword.compare("r_cStraight")){
                        r_cStraight=value;
                        flag_r_cStraight=1;
                    }else if (!keyword.compare("temporalDiscretization")){
                        temporalDiscretization=value;
                        flag_temporalDiscretization=1;
                    }else if (!keyword.compare("subStepsRecalcVelocity")){
                        subStepsRecalcVelocity=value;
                        flag_subStepsRecalcVelocity=1;
                    }else if (!keyword.compare("straightMeltingModel")){
                        straightMeltingModel=value;
                        flag_straightMeltingModel=1;
                    }else if (!keyword.compare("F_H")){
                        F_H=value;
                        flag_F_H=1;
                    }else if (!keyword.compare("mass")){
                        mass=value;
                        flag_mass=1;
                    }else if (!keyword.compare("mu_L")){
                        mu_L=value;
                        flag_mu_L=1;
                    }else if (!keyword.compare("t_0")){
                        vector<string> sep = split(valueStr, ',');
                        t_0[0]=atof(sep[0].c_str());
                        t_0[1]=atof(sep[1].c_str());
                        t_0[2]=atof(sep[2].c_str());
                        flag_t_0=1;
                    }else if (!keyword.compare("n_0")){
                        vector<string> sep = split(valueStr, ',');
                        n_0[0]=atof(sep[0].c_str());
                        n_0[1]=atof(sep[1].c_str());
                        n_0[2]=atof(sep[2].c_str());
                        flag_n_0=1;
                    }else if (!keyword.compare("gravity_vector")){
                        vector<string> sep = split(valueStr, ',');
                        gravity_vector[0]=atof(sep[0].c_str());
                        gravity_vector[1]=atof(sep[1].c_str());
                        gravity_vector[2]=atof(sep[2].c_str());
                        flag_gravity_vector=1;
                    }else{
                        std::cerr << keyword << " is not a valid keyword." << std::endl;
                        continue;
                    }
                    
                    break;
                }
            }
        }
        
        
    }else{
        isValidInputFile=0;
    }
}

void IMinputFileParser::parseToModel(IMmodel& im_model){
    
    string inputFileName=filename;
    
    // now print a warning if variables has not been defined in the input file
    useDataFromInputFile(flag_rho_S, &rho_S, &im_model.rho_S,inputFileName,"rho_S");
    useDataFromInputFile(flag_rho_L, &rho_L, &im_model.rho_L,inputFileName,"rho_L");
    useDataFromInputFile(flag_h_m, &h_m, &im_model.h_m,inputFileName,"h_m");
    useDataFromInputFile(flag_T_m, &T_m, &im_model.T_m,inputFileName,"T_m");
    useDataFromInputFile(flag_T_S, &T_S, &im_model.T_S,inputFileName,"T_S");
    useDataFromInputFile(flag_c_p_S, &c_p_S, &im_model.c_p_S,inputFileName,"c_p_S");
    useDataFromInputFile(flag_c_p_L, &c_p_L, &im_model.c_p_L,inputFileName,"c_p_S");
    useDataFromInputFile(flag_k_L, &k_L, &im_model.k_L,inputFileName,"k_L");
    useDataFromInputFile(flag_k_S, &k_S, &im_model.k_S,inputFileName,"k_S");
    useDataFromInputFile(flag_L, &L, &im_model.L,inputFileName,"L");
    useDataFromInputFile(flag_H, &H, &im_model.H,inputFileName,"H");
    useDataFromInputFile(flag_tau, &tau, &im_model.tau,inputFileName,"tau");
    useDataFromInputFile(flag_subSteps, &subSteps, &im_model.subSteps,inputFileName,"subSteps");
    useDataFromInputFile(flag_r_cStraight, &r_cStraight, &im_model.r_cStraight,inputFileName,"r_cStraight");
    useDataFromInputFile(flag_temporalDiscretization, &temporalDiscretization, &im_model.temporalDiscretization,inputFileName,"temporalDiscretization");
    useDataFromInputFile(flag_subStepsRecalcVelocity, &subStepsRecalcVelocity, &im_model.subStepsRecalcVelocity,inputFileName,"subStepsRecalcVelocity");
    useDataFromInputFile(flag_straightMeltingModel, &straightMeltingModel, &im_model.straightMeltingModel,inputFileName,"straightMeltingModel");
    useDataFromInputFile(flag_F_H, &F_H, &im_model.F_H,inputFileName,"F_H");
    useDataFromInputFile(flag_mass, &mass, &im_model.mass,inputFileName,"mass");
    useDataFromInputFile(flag_mu_L, &mu_L, &im_model.mu_L,inputFileName,"mu_L");
    
    if (flag_t_0) {
        im_model.t_0[0]=t_0[0];
        im_model.t_0[1]=t_0[1];
        im_model.t_0[2]=t_0[2];
    }else{
        cout << "WARNING: t_0 was not found in " << inputFileName << ". Using the default values " << t_0[0] << "," << t_0[1] << "," << t_0[2] << " instead." << endl;
    }
    if (flag_n_0) {
        im_model.n_0[0]=n_0[0];
        im_model.n_0[1]=n_0[1];
        im_model.n_0[2]=n_0[2];
    }else{
        cout << "WARNING: n_0 was not found in " << inputFileName << ". Using the default values " << n_0[0] << "," << n_0[1] << "," << n_0[2] << " instead." << endl;
    }
    if (flag_gravity_vector) {
        im_model.gravity_vector[0]=gravity_vector[0];
        im_model.gravity_vector[1]=gravity_vector[1];
        im_model.gravity_vector[2]=gravity_vector[2];
    }else{
        cout << "WARNING: gravity_vector was not found in " << inputFileName << ". Using the default values " << gravity_vector[0] << "," << gravity_vector[1] << "," << gravity_vector[2] << " instead." << endl;
    }
    
    // make sure that we are using unit vectors
    double norm_t_0=sqrt(im_model.t_0[0]*im_model.t_0[0]+im_model.t_0[1]*im_model.t_0[1]+im_model.t_0[2]*im_model.t_0[2]);
    double norm_n_0=sqrt(im_model.n_0[0]*im_model.n_0[0]+im_model.n_0[1]*im_model.n_0[1]+im_model.n_0[2]*im_model.n_0[2]);
    for (unsigned int i=0; i<3; i++) {
        im_model.t_0[i]=im_model.t_0[i]/norm_t_0;
        im_model.n_0[i]=im_model.n_0[i]/norm_n_0;
    }
}

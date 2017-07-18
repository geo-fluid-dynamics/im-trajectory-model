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
    this->flag_rho_S=0;
    this->flag_rho_L=0;
    this->flag_h_m=0;
    this->flag_T_m=0;
    this->flag_T_S=0;
    this->flag_c_p_S=0;
    this->flag_c_p_L=0;
    this->flag_k_L=0;
    this->flag_k_S=0;
    this->flag_L=0;
    this->flag_H=0;
    this->flag_c1_tau=0;
    this->flag_c2_tau=0;
    this->flag_subSteps=0;
    this->flag_r_cStraight=0;
}

IMinputFileParser::IMinputFileParser(string filename){
    this->isValidInputFile=1;
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
    
    this->filename=filename;
    string line;
    ifstream myfile (this->filename.c_str());
    if (myfile.is_open())
    {
        this->numberOfLines=0;
        while ( getline (myfile,line) )
        {
            this->numberOfLines++;
            //cout << line << '\n';
        }
        
        myfile.clear();
        myfile.seekg(0, myfile.beg);
        
        for (i=0; i<this->numberOfLines; i++) {
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
                        this->rho_S=value;
                        this->flag_rho_S=1;
                    }else if (!keyword.compare("rho_L")){
                        this->rho_L=value;
                        this->flag_rho_L=1;
                    }else if (!keyword.compare("k_S")){
                        this->k_S=value;
                        this->flag_k_S=1;
                    }else if (!keyword.compare("k_L")){
                        this->k_L=value;
                        this->flag_k_L=1;
                    }else if (!keyword.compare("c_p_S")){
                        this->c_p_S=value;
                        this->flag_c_p_S=1;
                    }else if (!keyword.compare("c_p_L")){
                        this->c_p_L=value;
                        this->flag_c_p_L=1;
                    }else if (!keyword.compare("h_m")){
                        this->h_m=value;
                        this->flag_h_m=1;
                    }else if (!keyword.compare("T_m")){
                        this->T_m=value;
                        this->flag_T_m=1;
                    }else if (!keyword.compare("T_S")){
                        this->T_S=value;
                        this->flag_T_S=1;
                    }else if (!keyword.compare("L")){
                        this->L=value;
                        this->flag_L=1;
                    }else if (!keyword.compare("H")){
                        this->H=value;
                        this->flag_H=1;
                    }else if (!keyword.compare("c1_tau")){
                        this->c1_tau=value;
                        this->flag_c1_tau=1;
                    }else if (!keyword.compare("c2_tau")){
                        this->c2_tau=value;
                        this->flag_c2_tau=1;
                    }else if (!keyword.compare("subSteps")){
                        this->subSteps=value;
                        this->flag_subSteps=1;
                    }else if (!keyword.compare("r_cStraight")){
                        this->r_cStraight=value;
                        this->flag_r_cStraight=1;
                    }else if (!keyword.compare("temporalDiscretization")){
                        this->temporalDiscretization=value;
                        this->flag_temporalDiscretization=1;
                    }else if (!keyword.compare("t_0")){
                        vector<string> sep = split(valueStr, ',');
                        this->t_0[0]=stod(sep[0]);
                        this->t_0[1]=stod(sep[1]);
                        this->t_0[2]=stod(sep[2]);
                        this->flag_t_0=1;
                    }else if (!keyword.compare("n_0")){
                        vector<string> sep = split(valueStr, ',');
                        this->n_0[0]=stod(sep[0]);
                        this->n_0[1]=stod(sep[1]);
                        this->n_0[2]=stod(sep[2]);
                        this->flag_n_0=1;
                    }else{
                        std::cerr << keyword << " is not a valid keyword." << std::endl;
                        return;
                    }
                    
                    break;
                }
            }
        }
        
        
    }else{
        this->isValidInputFile=0;
    }
}

void IMinputFileParser::parseToModel(IMmodel& im_model){
    
    string inputFileName=this->filename;
    
    // now print a warning if variables has not been defined in the input file
    useDataFromInputFile(this->flag_rho_S, &this->rho_S, &im_model.rho_S,inputFileName,"rho_S");
    useDataFromInputFile(this->flag_rho_L, &this->rho_L, &im_model.rho_L,inputFileName,"rho_L");
    useDataFromInputFile(this->flag_h_m, &this->h_m, &im_model.h_m,inputFileName,"h_m");
    useDataFromInputFile(this->flag_T_m, &this->T_m, &im_model.T_m,inputFileName,"T_m");
    useDataFromInputFile(this->flag_T_S, &this->T_S, &im_model.T_S,inputFileName,"T_S");
    useDataFromInputFile(this->flag_c_p_S, &this->c_p_S, &im_model.c_p_S,inputFileName,"c_p_S");
    useDataFromInputFile(this->flag_c_p_L, &this->c_p_L, &im_model.c_p_L,inputFileName,"c_p_S");
    useDataFromInputFile(this->flag_k_L, &this->k_L, &im_model.k_L,inputFileName,"k_L");
    useDataFromInputFile(this->flag_k_S, &this->k_S, &im_model.k_S,inputFileName,"k_S");
    useDataFromInputFile(this->flag_L, &this->L, &im_model.L,inputFileName,"L");
    useDataFromInputFile(this->flag_H, &this->H, &im_model.H,inputFileName,"H");
    useDataFromInputFile(this->flag_c1_tau, &this->c1_tau, &im_model.c1_tau,inputFileName,"c1_tau");
    useDataFromInputFile(this->flag_c2_tau, &this->c2_tau, &im_model.c2_tau,inputFileName,"c2_tau");
    useDataFromInputFile(this->flag_subSteps, &this->subSteps, &im_model.subSteps,inputFileName,"subSteps");
    useDataFromInputFile(this->flag_r_cStraight, &this->r_cStraight, &im_model.r_cStraight,inputFileName,"r_cStraight");
    useDataFromInputFile(this->flag_temporalDiscretization, &this->temporalDiscretization, &im_model.temporalDiscretization,inputFileName,"temporalDiscretization");
    
    if (this->flag_t_0) {
        im_model.t_0[0]=this->t_0[0];
        im_model.t_0[1]=this->t_0[1];
        im_model.t_0[2]=this->t_0[2];
    }else{
        cout << "WARNING: t_0 was not found in " << inputFileName << ". Using the default values " << this->t_0[0] << "," << this->t_0[1] << "," << this->t_0[2] << " instead." << endl;
    }
    if (this->flag_n_0) {
        im_model.n_0[0]=this->n_0[0];
        im_model.n_0[1]=this->n_0[1];
        im_model.n_0[2]=this->n_0[2];
    }else{
        cout << "WARNING: n_0 was not found in " << inputFileName << ". Using the default values " << this->n_0[0] << "," << this->n_0[1] << "," << this->n_0[2] << " instead." << endl;
    }
    
    // make sure that we are using unit vectors
    double norm_t_0=sqrt(im_model.t_0[0]*im_model.t_0[0]+im_model.t_0[1]*im_model.t_0[1]+im_model.t_0[2]*im_model.t_0[2]);
    double norm_n_0=sqrt(im_model.n_0[0]*im_model.n_0[0]+im_model.n_0[1]*im_model.n_0[1]+im_model.n_0[2]*im_model.n_0[2]);
    for (unsigned int i=0; i<3; i++) {
        im_model.t_0[i]=im_model.t_0[i]/norm_t_0;
        im_model.n_0[i]=im_model.n_0[i]/norm_n_0;
    }
}

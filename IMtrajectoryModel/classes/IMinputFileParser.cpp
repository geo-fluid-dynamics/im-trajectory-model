//
//  IMinputFileParser.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 04.06.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#include "IMinputFileParser.hpp"

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
}

IMinputFileParser::IMinputFileParser(string filename){
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
    ifstream myfile (this->filename);
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
                    value=stod(valueStr);
                    
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
                    }else{
                        std::cerr << keyword << " is not a valid keyword." << std::endl;
                        return;
                    }
                    
                    break;
                }
            }
        }
        
        
    }
}

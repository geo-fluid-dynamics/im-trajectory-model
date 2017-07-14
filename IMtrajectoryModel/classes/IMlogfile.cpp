//
//  IMlogfile.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 25.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#include "IMlogfile.hpp"

unsigned int dateTimeToSeconds(int year,int month, int day,int hours,int minutes,int seconds){
    unsigned int secondsOut=hours*3600+minutes*60+seconds;
    secondsOut=secondsOut+365*(year-2008)*3600*24;
    for(unsigned int i=1;i<=month;i++){
        switch (i) {
            case 2:
                secondsOut=secondsOut+31*24*3600;
                break;
            case 3:
                secondsOut=secondsOut+28*24*3600;
                break;
            case 4:
                secondsOut=secondsOut+31*24*3600;
                break;
            case 5:
                secondsOut=secondsOut+30*24*3600;
                break;
            case 6:
                secondsOut=secondsOut+31*24*3600;
                break;
            case 7:
                secondsOut=secondsOut+30*24*3600;
                break;
            case 8:
                secondsOut=secondsOut+31*24*3600;
                break;
            case 9:
                secondsOut=secondsOut+31*24*3600;
                break;
            case 10:
                secondsOut=secondsOut+30*24*3600;
                break;
            case 11:
                secondsOut=secondsOut+31*24*3600;
                break;
            case 12:
                secondsOut=secondsOut+30*24*3600;
                break;
                
            default:
                break;
        }
    }
    secondsOut=secondsOut+(day-1)*24*3600;
    return secondsOut;
}

IMlogfile::IMlogfile(string filename, bool simpleLogfile){
    unsigned int i;
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
        
        this->content = new string[this->numberOfLines];
        this->dataLines = new bool[this->numberOfLines];
        // delete[]this->content;
        
        myfile.clear();
        myfile.seekg(0, myfile.beg);

        this->numberOfDataLines=0;
        for (i=0; i<this->numberOfLines; i++) {
            getline (myfile,line);
            this->content[i]=line;
            if(this->content[i][0]=='#' || this->content[i][0]=='\r' || this->content[i][0]=='D'){
                this->dataLines[i]=false;
                
            }else{
                this->dataLines[i]=true;
                this->numberOfDataLines++;

            }
            //cout << this->content[i] << '\n';
        }
        
        if(this->numberOfLines==0){
            this->isValidLogfile=0;
            return;
        }
        
        this->heaterStates = new bool*[this->numberOfDataLines];
        this->timeInSeconds = new unsigned int[this->numberOfDataLines];
        for(unsigned int k = 0; k < this->numberOfDataLines; k++)
            this->heaterStates[k] = new bool[24];
        
        unsigned int k=0;
        for (i=0; i<this->numberOfLines; i++) {
            if (this->dataLines[i]) {
                vector<string> sep = split(this->content[i], ' ');

                if(simpleLogfile){
                    this->timeInSeconds[k]=atoi( sep[0].c_str() );
                    for (unsigned n = 0; n<24; n++) {
                        
                        if(sep[n+1][0]=='0'){
                            heaterStates[k][n]=false;
                        }else{
                            heaterStates[k][n]=true;
                        }
                        //cout << heaterStates[k][n];
                    }
                }else{
                    vector<string> dateValues=split(sep[0], '.');
                    vector<string> timeValues=split(sep[1], ':');
                    
                    this->timeInSeconds[k]=dateTimeToSeconds(atoi( dateValues[2].c_str() ),atoi( dateValues[1].c_str() ),atoi( dateValues[0].c_str() ),atoi( timeValues[0].c_str() ),atoi( timeValues[1].c_str() ),atoi( timeValues[2].c_str() ));
                
                    for (unsigned n = 0; n<24; n++) {

                        if(sep[13][n]=='0'){
                            heaterStates[k][n]=false;
                        }else{
                            heaterStates[k][n]=true;
                        }
                        //cout << heaterStates[k][n];
                    }
                }
                //cout << "\n";
                k++;
            }
        }
        
        myfile.close();
        
        for (int i=numberOfDataLines-1; i>=0; i--) {
            this->timeInSeconds[i]=this->timeInSeconds[i]-this->timeInSeconds[0];
        }
        this->timeInSeconds[0]=0;
        
    }
    
    else cout << "Unable to open the file " << this->filename;
};

IMlogfile::~IMlogfile(){
    //delete[] this->content;
}

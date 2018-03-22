//
//  IMlogfile.cpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 25.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#include "IMlogfile.hpp"

double dateTimeToSeconds(int year,int month, int day,int hours,int minutes,int seconds){
    double secondsOut=hours*3600+minutes*60+seconds;
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

IMlogfile::IMlogfile(string filenameIn, bool simpleLogfile){
    isValidLogfile=1;
    unsigned int i;
    filename=filenameIn;
    string line;
    ifstream myfile (filename.c_str());
    if (myfile.is_open())
    {
        numberOfLines=0;
        while ( getline (myfile,line) )
        {
            numberOfLines++;
        }
        
        content = new string[numberOfLines];
        dataLines = new bool[numberOfLines];
        
        myfile.clear();
        myfile.seekg(0, myfile.beg);

        numberOfDataLines=0;
        for (i=0; i<numberOfLines; i++) {
            getline (myfile,line);
            content[i]=line;
            if(content[i][0]=='#' || content[i][0]=='\r' || content[i][0]=='D'){
                dataLines[i]=false;
                
            }else{
                dataLines[i]=true;
                numberOfDataLines++;

            }
        }
        
        if(numberOfLines==0){
            isValidLogfile=0;
            return;
        }
        
        heaterStates = new bool*[numberOfDataLines];
        timeInSeconds = new double[numberOfDataLines];
        for(unsigned int k = 0; k < numberOfDataLines; k++)
            heaterStates[k] = new bool[24];
        
        unsigned int k=0;
        for (i=0; i<numberOfLines; i++) {
            if (dataLines[i]) {
                vector<string> sep = split(content[i], ' ');

                if(simpleLogfile){
                    timeInSeconds[k]=atof( sep[0].c_str() );
                    for (unsigned n = 0; n<24; n++) {
                        
                        if(sep[n+1][0]=='0'){
                            heaterStates[k][n]=false;
                        }else{
                            heaterStates[k][n]=true;
                        }
                    }
                }else{
                    vector<string> dateValues=split(sep[0], '.');
                    vector<string> timeValues=split(sep[1], ':');
                    
                    timeInSeconds[k]=dateTimeToSeconds(atoi( dateValues[2].c_str() ),atoi( dateValues[1].c_str() ),atoi( dateValues[0].c_str() ),atoi( timeValues[0].c_str() ),atoi( timeValues[1].c_str() ),atoi( timeValues[2].c_str() ));
                
                    for (unsigned n = 0; n<24; n++) {

                        if(sep[13][n]=='0'){
                            heaterStates[k][n]=false;
                        }else{
                            heaterStates[k][n]=true;
                        }
                    }
                }
                k++;
            }
        }
        
        myfile.close();
        
        for (int i=numberOfDataLines-1; i>=0; i--) {
            timeInSeconds[i]=timeInSeconds[i]-timeInSeconds[0];
        }
        timeInSeconds[0]=0;
        
    }
    
    else{
        isValidLogfile=0;
        cout << "Unable to open the file " << filename << endl;
    }
};

IMlogfile::~IMlogfile(){
    for(unsigned int k = 0; k < numberOfDataLines; k++)
        delete [] heaterStates[k];
        delete [] heaterStates;
        delete [] content;
        delete [] timeInSeconds;
        delete [] dataLines;
}

void IMlogfile::extendLogfile(unsigned int extendValue){
    string extendedLogfilename=filename;
    extendedLogfilename.replace(extendedLogfilename.end()-4,extendedLogfilename.end(),"_ext.csv");
    
    ofstream myfile (extendedLogfilename.c_str());
    if (myfile.is_open())
    {
        myfile << content[0] << endl;
        for (unsigned int i=1; i<numberOfLines-1; i++) {
            for (unsigned int k=0; k<extendValue; k++) {
                myfile << fixed << timeInSeconds[i-1]+k*(timeInSeconds[i]-timeInSeconds[i-1])/extendValue;
                for (unsigned int kk=0; kk<24; kk++) {
                    myfile << " " << heaterStates[i-1][kk];
                }
                myfile << endl;
            }
        }
        myfile << content[numberOfLines-1];
        myfile.close();
    }
    else cout << "Unable to open file";
}

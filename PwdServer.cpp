/*
 * PwdServer.cpp
 *
 *  Created on: 15.12.2020
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner 
 */

#include <iostream>
#include <cstdlib>

#include "PwdServer.hpp"

PwdServer::PwdServer(int port, int maxDataSizeRecv, int pwdLength, int symbSetSize) :
    TCPserver(port, maxDataSizeRecv){
    ptrBBS_ = new TASK1::BlackBoxSafe(pwdLength, symbSetSize);
    return;
}

PwdServer::~PwdServer(){
    if(dataRecv_ != NULL){
        delete [] dataRecv_;
    }
    if(ptrBBS_ != NULL){
        delete ptrBBS_;
    }
    return;
}

string PwdServer::response(string incomingMsg){
    string msg;

    cout << "received:" << incomingMsg << endl;

    if(incomingMsg.compare(0,7,"BYEBYE!") == 0){
        msg = string("BYEBYE!");
    }else{
        msg = myResponse(incomingMsg);
    }

    cout << "send back:" << msg << endl;

    flush();
    return msg;
}

string PwdServer::myResponse(string input){
    string output;
    if(input.compare(0,15,"UPDATE PASSWORD") == 0){
        int newPwdLength, newSymbSetSize = 0;

        try{
            newPwdLength   = stoi(input.substr(16, (input.find(",", 16) - 16)));
            newSymbSetSize = stoi(input.substr(input.find(",", 16) + 1));
        }catch(...){
            //cerr << "ERROR: can't read received password parameters." << endl;
            return string("UNRECOGNISED PASSWORD PARAMETERS");
        }

        if(ptrBBS_ != NULL) delete ptrBBS_;

        ptrBBS_ = new TASK1::BlackBoxSafe(newPwdLength, newSymbSetSize);

        return string("PASSWORD UPDATED");
    }

    return string(ptrBBS_->input(input));
}

void PwdServer::flush(){
    if(dataRecv_ != NULL){
        delete [] dataRecv_;
        dataRecv_ = new char[maxDataSizeRecv_];
    }
}
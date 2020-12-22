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
    if(incomingMsg.compare(0,7,"BYEBYE!") == 0){
        msg = string("BYEBYE!");
    }else{
        msg = myResponse(incomingMsg);
    }

    cout << "received :" << incomingMsg << endl;
    cout << "send back:" << msg << endl;

    return msg;
}

string PwdServer::myResponse(string input){
    return string(ptrBBS_->input(input));
}
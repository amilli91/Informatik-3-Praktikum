/*
 * PwdServer.cpp
 *
 *  Created on: 15.12.2020
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner 
 */

#include <iostream>
#include <stdio.h>
#include <cstdlib>

#include "PwdServer.hpp"

PwdServer::PwdServer(int port, int maxDataSizeRecv) : TCPserver(port, maxDataSizeRecv){;};

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

    //cout << "received:" << incomingMsg << endl;

    if(incomingMsg.compare(0,7,"BYEBYE!") == 0){
        msg = string("BYEBYE!");
    }else{
        msg = myResponse(incomingMsg);
    }

    //cout << "send back:" << msg << endl;

    this->flush();
    return msg;
}

string PwdServer::myResponse(string input){
    string output;
    if(input.compare(0,15,"UPDATE PASSWORD") == 0){
        int newPwdLength, newSymbSetSize = 0;

        if(!sscanf(input.c_str(), "UPDATE PASSWORD[%d,%d]", &newPwdLength, &newSymbSetSize)){
            return string("UNRECOGNISED PASSWORD PARAMETERS");
        }
        
        if( (newPwdLength < 1) || (newSymbSetSize < 1) || (newSymbSetSize > TASK1::SYMBOLS.length()) ){
            return string("INVALID PASSWORD PARAMETERS");
        }else{
            if(ptrBBS_ != NULL) delete ptrBBS_;

            ptrBBS_ = new TASK1::BlackBoxSafe(newPwdLength, newSymbSetSize);

            return string("PASSWORD UPDATED");
        }
    }

    return string(ptrBBS_->input(input));
}

void PwdServer::flush(){
    //delete [] dataRecv_;
    //dataRecv_ = new char[maxDataSizeRecv_];

    
    for(int i = 0; i < maxDataSizeRecv_; i++){
        dataRecv_[i] = '\0';
    }   
    

    return;
}


PwdServerSingleThread::PwdServerSingleThread(int port, int maxDataSizeRecv, int pwdLength, int symbSetSize) :
    PwdServer(port, maxDataSizeRecv){
    ptrBBS_ = new TASK1::BlackBoxSafe(pwdLength, symbSetSize);
    return;
}

PwdServerMultiThread::PwdServerMultiThread(int port, int maxDataSizeRecv, int pwdLength, int SymbSetSize, int threadCount = 1){
    ptrBBS_ = new TASK1::BlackBoxSafe(pwdLength, SymbSetSize);

    pwdServers_ = new PwdServer*[threadCount];
    serverThreads_ = new std::thread*[threadCount];

    for(int i = 0; i < threadCount; i++){
        pwdServers_[i] = new PwdServer((port + i), maxDataSizeRecv);
    }
}

void PwdServerMultiThread::startServer(){
    for(int i = 0; i < threadCount_; i++){
        serverThreads_[i] = new std::thread(&TCPserver::run, ref(pwdServers_[i]));
    }


}
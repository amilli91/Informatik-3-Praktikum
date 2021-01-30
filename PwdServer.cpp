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

PwdServer::PwdServer(int port, int maxDataSizeRecv) : 
    TCPserver(port, maxDataSizeRecv){
        
        ptrBBS_ = nullptr;
        
        return;
    };

PwdServer::~PwdServer(){
    if(dataRecv_ != NULL){
        delete [] dataRecv_;
    }
    if(ptrBBS_ != NULL){
        delete ptrBBS_;
    }
    return;
}

void PwdServer::run(){
    if(ptrBBS_ == nullptr){
        throw string("BlackBox not initialized!");
    }

    this->TCPserver::run();
}

string PwdServer::myResponse(string input){
    string output;
    if(input.compare(0,15,"UPDATE_PASSWORD") == 0){
        int newPwdLength, newSymbSetSize = 0;

        if(!sscanf(input.c_str(), "UPDATE_PASSWORD[%d,%d]", &newPwdLength, &newSymbSetSize)){
            return string("UNRECOGNISED_PASSWORD_PARAMETERS");
        }
        
        if( (newPwdLength < 1) || (newSymbSetSize < 1) || (newSymbSetSize > TASK1::SYMBOLS.length()) ){
            return string("INVALID_PASSWORD_PARAMETERS");
        }else{
            ptrBBS_->updatePwd(newPwdLength, newSymbSetSize);
            return string("PASSWORD_UPDATED");
        }
    }

    return string(ptrBBS_->input(input));
}

void PwdServer::parseBlackBox(TASK1::BlackBoxSafe *ptrBBS){
    ptrBBS_ = ptrBBS;
}

void PwdServer::flush(){
    //delete [] dataRecv_;
    //dataRecv_ = new char[maxDataSizeRecv_];

    for(int i = 0; i < maxDataSizeRecv_; i++){
        dataRecv_[i] = '\0';
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



PwdServerSingleThread::PwdServerSingleThread(int port, int maxDataSizeRecv, int pwdLength, int symbSetSize) :
    PwdServer(port, maxDataSizeRecv){
    if(port < 1) throw string("invalid value for port!");
    if(pwdLength < 1) throw string("invalid value for pwdLength!");
    if(symbSetSize < 1) throw string("invalid value for SymbSetSize!");
    ptrBBS_ = new TASK1::BlackBoxSafe(pwdLength, symbSetSize);
    return;
}



PwdServerMultiThread::PwdServerMultiThread(int port, int maxDataSizeRecv, int pwdLength, int symbSetSize, int threadCount){
    if(port < 1) throw string("invalid value for port!");
    if(pwdLength < 1) throw string("invalid value for pwdLength!");
    if(symbSetSize < 1) throw string("invalid value for SymbSetSize!");
    if(threadCount < 1) throw string("invalid value for threadCount!");

    threadCount_ = threadCount;

    ptrBBS_ = new TASK1::BlackBoxSafe(pwdLength, symbSetSize);

    pwdServers_ = new PwdServer*[threadCount];
    serverThreads_ = new std::thread*[threadCount];

    for(int i = 0; i < threadCount; i++){
        pwdServers_[i] = new PwdServer((port + i), maxDataSizeRecv);
        pwdServers_[i]->parseBlackBox(ptrBBS_);
    }

    return;
}

void PwdServerMultiThread::startServer(){

    try{
        for(int i = 0; i < threadCount_; i++){
            serverThreads_[i] = new std::thread(&TCPserver::run, ref(pwdServers_[i]));
        }

        int joinedThreads = 0;

        while(joinedThreads < threadCount_){
            for(int i = 0; i < threadCount_; i++)
                if(serverThreads_[i]->joinable()){
                    serverThreads_[i]->join();
                    joinedThreads++;
                }
        }
            
    }catch(...){
        cerr << "Error. terminate programm " << endl;
        exit(0);
    }
    return;
}

PwdServerMultiThread::~PwdServerMultiThread(){
    if(ptrBBS_ != NULL) delete ptrBBS_;
    if(pwdServers_ != NULL) delete [] pwdServers_;
    
    return;
}
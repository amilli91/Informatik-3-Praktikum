/*
 * PwdClient.cpp
 * 
 *  Created on: 15.12.2020 
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <math.h>

#include "PwdClient.hpp"

PwdClient::PwdClient(int pwdLength, int symbSetSize):
    TCPclient(){
    if((symbSetSize < 1) || (symbSetSize > SYMBOLS.length()) ){
        lengthSymbArray_ = SYMBOLS.length();
    }else{
        lengthSymbArray_ = symbSetSize;
    }

    charSymbArray_ = new char [lengthSymbArray_ + 1];
    strncpy(charSymbArray_, SYMBOLS.c_str(), lengthSymbArray_);

    if(pwdLength < 0){
        pwdLength_ = 0;
    }else{
        pwdLength_ = pwdLength;
    }

    return;
}

PwdClient::~PwdClient(){
    if(charSymbArray_ != NULL){
        delete [] charSymbArray_;
    }
    return;
}

unsigned int PwdClient::bruteForce(){
    string pwdGuess, response;
    unsigned int n = 0;
    unsigned int index = 0;

    pwdGuess = string("");
    
    if(pwdLength_ > 0){
        for(unsigned int i = 0; i < pwdLength_; i++){
            pwdGuess += charSymbArray_[0];
        }

        do{
            for(unsigned int i = 0; i < pwdLength_; i++){
                if(i == 0){
                    pwdGuess.replace(i, 1, 1, charSymbArray_[unsigned(n % lengthSymbArray_)]);
                }else{
                    index = ( n / unsigned(pow(float(lengthSymbArray_), float(i))) ) % (lengthSymbArray_);
                    pwdGuess.replace(i, 1, 1, charSymbArray_[index]);
                }
            }

            cout << "client sends:" << pwdGuess << endl;
            this->sendData(pwdGuess);

            response = this->receive(32);
            cout << "got response:" << response << endl;

            n++;

        }while(response.compare("ACCESS GRANTED") != 0);

        return unsigned(n);
    }else{
        unsigned int lengthJmp = lengthSymbArray_;

        pwdGuess += charSymbArray_[0];
        do{
            unsigned int counterShft = 0;

            for(unsigned int i = 0; i < pwdGuess.length(); i++){
                if(i == 0){
                    pwdGuess.replace(i, 1, 1, charSymbArray_[unsigned(n % lengthSymbArray_)]);
                }else{
                    for(unsigned int j = 1; j <= i; j++){
                        counterShft += unsigned(pow(float(lengthSymbArray_), float(j)));
                    }
                    index = ( (n - counterShft) / unsigned(pow(float(lengthSymbArray_), float(i))) ) % (lengthSymbArray_);
                    pwdGuess.replace(i, 1, 1, charSymbArray_[index]);
                    counterShft = 0;
                }
            }

            cout << "client sends:" << pwdGuess << endl;
            this->sendData(pwdGuess);

            response = this->receive(32);
            cout << "got response:" << response << endl;

            n++;
            if( (n % lengthJmp) == 0){
                pwdGuess += charSymbArray_[0];
                lengthJmp += unsigned(pow(float(lengthSymbArray_), float(pwdGuess.length())));
            }

        }while(response.compare("ACCESS GRANTED") != 0);

        return unsigned(n);
    }

    
}

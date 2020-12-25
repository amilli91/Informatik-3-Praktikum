/*
 * PwdClient.hpp
 * 
 *  Created on: 17.12.2020
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner
 * 
 */

#ifndef PwdClient_hpp_
#define PwdClient_hpp_

#include "SIMPLESOCKET.hpp"

const string SYMBOLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

class PwdClient : public TCPclient{
    protected:
        char *charSymbArray_;
        int  lengthSymbArray_;
        int pwdLength_;

    public:
        PwdClient(int pwdLength, int symbSetSize);
        ~PwdClient();
        int bruteForce();
        
};

#endif /* PwdClient_hpp_ */
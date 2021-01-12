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
        unsigned int lengthSymbArray_;
        unsigned int pwdLength_;

    public:
        PwdClient(int pwdLength, int symbSetSize);
        ~PwdClient();
        unsigned int bruteForce();
        
};

#endif /* PwdClient_hpp_ */
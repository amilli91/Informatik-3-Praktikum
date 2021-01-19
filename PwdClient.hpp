/*
 * PwdClient.hpp
 * 
 *  Created on: 17.12.2020
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner
 * 
 */

/**
 *
 * \file PwdClient.hpp
 *
 * \brief Contains class definitions of different 
 * password client implementations.
 *
 */


#ifndef PwdClient_hpp_
#define PwdClient_hpp_

#include "SIMPLESOCKET.hpp"

/**
 *
 * \var const string SYMBOLS
 *
 *	\brief This contains all possible characters a password can contain
 *
 */

const string SYMBOLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

/**
 *
 * \class PwdClient
 *
 *	\brief Implements the client class, which connects to PwdServer 
 and tries to guess the right password.
 *
 */

class PwdClient : public TCPclient{
    protected:
        char *charSymbArray_;
        unsigned int lengthSymbArray_;
        unsigned int pwdLength_;
        unsigned int foundPwdLength_;

    public:
        PwdClient(int pwdLength, int symbSetSize);
        ~PwdClient();
        unsigned int bruteForce();
        unsigned int getLength();
        bool setLength(int newLength);
        unsigned int getSymbSetSize();
        bool setSymbSetSize(int newSize);
        unsigned int getFoundPwdLength();
        
};

#endif /* PwdClient_hpp_ */
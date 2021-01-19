/*
 * PwdServer.hpp
 * 
 *  Created on: 15.12.2020
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner
 * 
 */

/**
 *
 * \file PwdServer.hpp
 *
 * \brief Contains class definitions of different 
 * password server implementations.
 *
 */

#ifndef PwdServer_hpp_
#define PwdServer_hpp_

#include <thread>

#include "SIMPLESOCKET.hpp"
#include "TASK1.hpp"

/**
 *
 * \class PwdServer
 *
 * \brief Implements the server class, which is connected by
 * the Client. The Server sets the password and encodes it with
 * an hash-function.
 *
 */

class PwdServer : public TCPserver{
    protected:
        TASK1::BlackBoxSafe *ptrBBS_; 

    public:
        PwdServer(int port, int maxDataSizeRecv);
        ~PwdServer();

    protected:
        string myResponse(string input);
        void flush();
        string response(string incomingMsg);

};

/**
 *
 * \class PwdServerSingleThread
 *
 * \brief Implements the single thread class, which  XXXXX
 *
 */

class PwdServerSingleThread : public PwdServer{
    public: 
        PwdServerSingleThread(int port, int maxDataSizeRecv, int pwdLength, int SymbSetSize);
};

/**
 *
 * \class PwdServerMultiThread
 *
 * \brief Implements the multi thread class, which XXXX
 *
 */

class PwdServerMultiThread{
    protected:
        int threadCount_;
        int port_;
        int maxDataSizeRecv_;
        TASK1::BlackBoxSafe *ptrBBS_;

        PwdServer **pwdServers_;
        std::thread **serverThreads_;

    public:
        PwdServerMultiThread(int port, int maxDataSizeRecv, int pwdLength, int SymbSetSize, int threadCount);
        ~PwdServerMultiThread();
        void startServer();
};


#endif /* PwdServer_hpp_ */
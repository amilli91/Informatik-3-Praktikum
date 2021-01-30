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
 * \brief Contains the definition of the classes 
 *        for a password-storing server implementation.
 * 
 */

#ifndef PwdServer_hpp_
#define PwdServer_hpp_

#include <thread>   //std::thread

#include "SIMPLESOCKET.hpp"     //TCPserver
#include "TASK1.hpp"    //BlackBoxSafe


/**
 * 
 * \class PwdServer
 * \brief The class definition of a server that stores a password and checks if incoming messages match the correct password
 *        An actual pointer to a BlackBoxSafe object has to be parsed using the parseBlackBox method. Therefore the server 
 *        cannot function properly by just calling the constructor. But this allows multiple class instantiations to share the
 *        same BlackBox. 
 * 
 */
class PwdServer : public TCPserver{
    protected:
        TASK1::BlackBoxSafe *ptrBBS_; 

    public:
        /**
         * 
         * \brief constructor
         * 
         * \param port int port number the server listens on.
         * \param maxDataSizeRecv int maximal of the messages the server can receive.
         * 
         */
        PwdServer(int port, int maxDataSizeRecv);
        
        /**
         * 
         * \brief destructor
         * 
         */
        ~PwdServer();

        /**
         * 
         * \brief starts the server.
         * 
         */
        void run();

        /**
         * 
         * \brief allows multiple PwdServers to store the same password in a single BlackBoxSafe. Used for multithreading.
         * 
         * \param ptrBBS TASK1::BlackBoxSafe* pointer to a BlackBoxSafe object that stores the servers password.
         * 
         */
        virtual void parseBlackBox(TASK1::BlackBoxSafe *ptrBBS);

    protected:
        /**
         * 
         * \brief Defines the response to the given received data.
         * 
         * \param input contains the data the server has received.
         * 
         * \return string containing the response of the server.
         * 
         */
        string myResponse(string input);
        
        /**
         * 
         * \brief clears the receive buffer to remove remnants of older received data. 
         * 
         */
        void flush();

        /**
         * 
         * \brief Wrapping method for method myResponse. Calls flush method after every received message to clear the receive buffer.
         * 
         * \param incomingMsg string containing the data the server has received.
         * 
         * \return string containing the response of the server. 
         * 
         */
        string response(string incomingMsg);

};


/**
 * 
 * \class PwdServerSingleThread
 * \brief The class definition of a server that stores a password and checks if incoming messages match the correct password.
 * 
 */
class PwdServerSingleThread : public PwdServer{
    public: 
        /**
         * 
         * \brief constructor
         * 
         * \param port int port number the server listens on.
         * \param maxDataSizeRecv int maximal size of the messages the server can receive.
         * \param pwdLength int number of characters in the password.
         * \param symbSetSize int alphabet size of the password.
         * 
         */
        PwdServerSingleThread(int port, int maxDataSizeRecv, int pwdLength, int SymbSetSize);

        /**
         * 
         * \brief overwrites the parseBlackBox method of the class PwdServer to deny attempts to change the member ptrBBS externally.
         * 
         */
        void parseBlackBox(){cerr << "Cant parse a BlackBox to an object of class PwdServerSingleThread!"; exit(0);};

    protected:
        /**
         * 
         * \brief guarantees that the response method from the class PwdServer is called when using pointers. Prevents the possibilies of buffer remnants. 
         * 
         */
        string response(string incomingMsg){return this->PwdServer::response(incomingMsg);}
};


/**
 * 
 * \class PwdSeverMultiThread
 * \brief The class definition of a server that runs multiple PwdServer objects simultaneoulsy.
 *        All PwdServers store the same BlackBoxSafe and therefore have the same password.
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
        /**
         * 
         * \brief constructor
         * 
         * \param port int port number the server listens on.
         * \param maxDataSizeRecv int maximal size of the messages the server can receive.
         * \param pwdLength int number of characters in the password.
         * \param symbSetSize int alphabet size of the password.
         * \param threadCount int number of threads and PwdServer objects that will run simultaneously.
         * 
         */
        PwdServerMultiThread(int port, int maxDataSizeRecv, int pwdLength, int SymbSetSize, int threadCount);
 
        /**
         * 
         * \brief destructor
         * 
         */
        ~PwdServerMultiThread();
        
        /**
         * 
         * \brief starts the server
         * 
         */
        void startServer();
};


#endif /* PwdServer_hpp_ */
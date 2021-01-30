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
 * \brief Contains the definition of the classes 
 *        for a password-cracking client implementation.
 * 
 */

#ifndef PwdClient_hpp_
#define PwdClient_hpp_

#include <thread>   //std::thread

#include "SIMPLESOCKET.hpp" //TCPclient

/**
 *
 * \var const string SYMBOLS
 *
 *	\brief This contains all possible characters a password can contain.
 *
 */
const string SYMBOLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

/**
 * 
 * \class PwdClient
 * \brief The class definition of a client that tries to find a password by trying all possible combinations.
 *        This class is explicity written to be used by PwdClientMultiThread for simultaneous execution of the 
 *        bruteForce method belonging to this class.
 * 
 */
class PwdClient : public TCPclient{
    protected:
        char *charSymbArray_;
        int lengthSymbArray_;
        unsigned foundPwdLength_;
        unsigned bruteForceResult_;

    public:
        /**
         * 
         * \brief constructor
         * 
         * \param symbSetSize int size of the password alphabet.
         * 
         */
        PwdClient(int symbSetSize);

        /**
         * 
         * \brief destructor
         * 
         */
        ~PwdClient();


        virtual bool bruteForce(bool *status, unsigned pwdLength, unsigned startPos, unsigned endPos);
        
        /**
         * 
         * \brief returns the results of the latest bruteForce attack.
         * 
         * \return unsigned int number of tries made during the last call of the bruteForce method.
         * 
         */
        unsigned getBruteForceResults();

        /**
         * 
         * \brief returns the length of the found recently password.
         * 
         * \return unsigned int number of characters in the most recently found password.
         * 
         */
        unsigned getFoundPwdLength();

        /**
         * 
         * \brief sends an update request message to the connected password server containing new password parameters.
         *
         * \param newPwdLength int number of characters in the new password.
         * \param newSymbSetSize int alphabet size of the new password.
         * 
         * \return bool true if the update request was accepted by the server.
         * 
         */ 
        virtual bool sendUpdateRequest(int newPwdLength, int newSymbSetSize);
        
        /**
         * 
         * \brief updates the members lengthSymbArray_ and charSymbArray_.
         * 
         * \param newSymbSetSize int new password alphabet size.
         * 
         */
        void setSymbSetSize(int newSymbSetSize);
};


/**
 * 
 * \class PwdClientSingleThread
 * \brief The class definition of a client that tries to find a password by trying all possible combinations.
 * 
 */
class PwdClientSingleThread : public PwdClient{
    protected:
        unsigned pwdLength_;

    public:
        /**
         * 
         * \brief constructor
         * 
         * \param pwdLength int number of characters in the password.
         * \param symbSetSize int alphabet size of the password.
         * 
         */
        PwdClientSingleThread(int pwdLength, int symbSetSize);

        /**
         * 
         * \brief destructor
         * 
         */
        ~PwdClientSingleThread();

        /**
         * 
         * \brief Tries to find the password by trying out all possible password combinations.
         * 
         * \param pwdLength int number of characters in the password. If called without a parameter
         *                  or with a negative valuethe value of the member pwdLength_ will be assumed.
         *                  If called with 0 the number of characters will be assumed as unknown.
         * 
         * \return unsigned int number of tries needed to find the correct password  
         * 
         */
        unsigned bruteForce(int pwdLength = -1);
};


/**
 * 
 * \class PwdClientMultiThread
 * \brief The class definition of a client that runs multiple PwdClient objects simultaneously.
 *        The work needed to find a password is shared between all PwdClient objects.
 * 
 */
class PwdClientMultiThread{
    protected:
        unsigned short threadCount_;
        int pwdLength_;
        int symbSetSize_;
        //unsigned *threadResults_; //member currently unused; would be used if PwdClients could each crack a different password

        PwdClient **pwdClients_;
        std::thread **clientThreads_;

    public:
        /**
         * 
         * \brief constructor
         * 
         * \param threadCount short number of threads and PwdClient objects that will work simultaneously 
         * \param pwdLength int number of characters in the password. If called without a parameter or with 0
         *                  the number of characters will be assumed as unknown.
         * \param symbSetSize int alphabet size of the password. If calles without a parameter or with 0
         *                    the alphabet size will be assumed as maximal.
         * 
         */
        PwdClientMultiThread(short threadCount, int pwdLength = 0, int symbSetSize = 0);

        /**
         * 
         * \brief destructor
         * 
         */
        ~PwdClientMultiThread();

        /**
         * 
         * \brief tries to connect all password client threads to a password server.
         * 
         * \param address string IP address of the password server. 
         * \param port int number of the password server.
         * 
         */
        bool connect(string address, int port);

        /**
         * 
         * \brief Tries to find the password by trying out all possible password combinations.
         * 
         * \param pwdLength int number of characters in the password. If called without a parameter
         *                  or with a negative valuethe value of the member pwdLength_ will be assumed.
         *                  If called with 0 the number of characters will be assumed as unknown.
         * 
         * \return unsigned int number of tries needed to find the correct password 
         * 
         */
        unsigned bruteForce(int pwdLength = -1);

        /*
        unsigned long long getBruteForceResults();    //method currently unused; would return BruteForce-results of individual threads
        */                                            //if password client threads could each crack a different password
        
        /**
         * 
         * \brief sends an update request message to the connected password server containing new password parameters.
         *
         * \param newPwdLength int number of characters in the new password.
         * 
         * \param newSymbSetSize int alphabet size of the new password.
         * 
         * \return bool true if the update request was accepted by the server
         * 
         */ 
         bool sendUpdateRequest(int newPwdLength, int newSymbSetSize);

        /**
         * 
         * \brief disconnects all password client threads by sending "BYEBYE!".
         * 
         */
        void disconnect();
};


#endif /* PwdClient_hpp_ */
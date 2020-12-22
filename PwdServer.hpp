/*
 * PwdServer.hpp
 * 
 *  Created on: 15.12.2020
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner
 * 
 */

#ifndef PwdServer_hpp_
#define PwdServer_hpp_

#include "SIMPLESOCKET.hpp"
#include "TASK1.hpp"

class PwdServer : public TCPserver{
    protected:
        TASK1::BlackBoxSafe *ptrBBS_; 

    public:
             PwdServer(int port, int maxDataSizeRecv, int pwdLength, int symbSetSize);
            ~PwdServer();

    protected:
        string myResponse(string input);

    private:
        string response(string incomingMsg);

};

#endif /* PwdServer_hpp_ */
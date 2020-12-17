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
        PwdServer(int port, int maxDataSizeRecv, int pwdLength, int symbSetSize) :
            TCPserver(port, maxDataSizeRecv){
                ptrBBS_ = new TASK1::BlackBoxSafe(pwdLength, symbSetSize);
            }

        ~PwdServer(){
            if(dataRecv_ != NULL){
                delete [] dataRecv_;
            }
            if(ptrBBS_ != NULL){
                delete ptrBBS_;
            }
            return;
        }

        void run();

    private:

        string response

    




};

#endif /* PwdServer_hpp_ */
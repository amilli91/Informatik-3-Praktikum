/*
 * passwordServer.hpp
 * 
 *  Created on: 15.12.2020
 *      Author: Luca Magdeburg
 * 
 */

#ifndef passwordServer_hpp_
#define passwordServer_hpp_

#include "SIMPLESOCKET.hpp"
#include "TASK1.hpp"

class PwdServer : public TCPserver{
    protected:

        TASK1::BlackBoxSafe *ptrBBS_; 
        //virtual string myResponse(string input);

    public:

        PwdServer(int port, int maxDataSizeRecv, int pwdLength, int symbSetSize) :
            TCPserver(port, maxDataSizeRecv){
                ptrBBS_ = new TASK1::BlackBoxSafe(pwdLength, symbSetSize);
            }

        ~PwdServer(){
            delete [] dataRecv_;
            if(ptrBBS_ != NULL){
                delete ptrBBS_;
            }
            return;
        }
    




};

#endif /* passwordServer_hpp_ */
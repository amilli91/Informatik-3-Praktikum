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
#include "SHA256.hpp"
#include "TASK1.hpp"

class PwdServer : public TCPserver{
    public:

        PwdServer(int port, int maxDataSizeRecv) :
            TCPserver(port, maxDataSizeRecv){
                
            }

    protected:

        TASK1::BlackBoxSafe BlackBoxSafe_; 
        virtual string myResponse(string input);




};

#endif /* passwordServer_hpp_ */
/*
 * passwordClient.cpp
 * 
 *  Created on: 15.12.2020 
 *      Author: Luca Magdeburg
 */

#include <iostream>

#include "SIMPLESOCKET.hpp"

int main(){
    
    TCPclient c;
    string host = "localhost";
    int port = 1234;
    string pwd;
    string tstMsg = "test";

    c.conn(host, port);

    std::cout << "client sends:" << tstMsg << endl;
    c.sendData(tstMsg);
    std::cout << "got response:" << c.receive(32) << endl;

}
/*
 * passwordClient.cpp
 * 
 *  Created on: 15.12.2020 
 *      Author: Luca Magdeburg
 */

#include <iostream>
#include <cstdlib>

#include "SIMPLESOCKET.hpp"

void printInfo(int argc, char *argv[]);

int main(int argc, char *argv[]){
    
    char *PASSWORD_SERVER_ADRESS;
    int PASSWORD_SERVER_PORT;
    int PASSWORD_LENGTH;
    int PASSWORD_SYMBOLSET_SIZE;
    int THREAD_COUNT; 

    printInfo(argc, argv);

    try{
        PASSWORD_SERVER_ADRESS  = argv[1];
        PASSWORD_SERVER_PORT    = atoi(argv[2]);
        PASSWORD_LENGTH         = atoi(argv[3]);
        PASSWORD_SYMBOLSET_SIZE = atoi(argv[4]);
        THREAD_COUNT            = atoi(argv[5]);
    }catch(...){
        cerr << "Cant't read command line parameters, terminate process.\n";
        exit(0);
    }

    TCPclient c;

    string pwd;
    string tstMsg = "test";

    c.conn(PASSWORD_SERVER_ADRESS, PASSWORD_SERVER_PORT);

    std::cout << "client sends:" << tstMsg << endl;
    c.sendData(tstMsg);
    std::cout << "got response:" << c.receive(32) << endl;

}

void printInfo(int argc, char *argv[]){
    if(argc == 6){
        return;
    }else{
        cerr << "Usage of " << argv[0] << " : \n\n"
             << argv[0] << " <server host> <server port> <password length> <password symbolset size> <thread count>" << endl;
        cerr << "\n"
             << "<server host>              hostname of the running password server\n"
             << "<server port>              port of the running password server\n"
             << "<password length>          known (!0) or unknown (0) password length\n"
             << "<password symbolset size>  known (!0) or unknown (0) password symbolset size\n"
             << "<thread count>             number of threads that simultaneusly try to find the password\n";
        exit(0);
    };
};
/*
 * PwdClient.cpp
 * 
 *  Created on: 15.12.2020 
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner
 */

#include <iostream>
#include <cstdlib>

#include "SIMPLESOCKET.hpp"
//#include "PwdClient.hpp"

void printInfo(int argc, char *argv[]);

int main(int argc, char *argv[]){
    
    TCPclient c;
    string pwd = "A";

    char *PASSWORD_SERVER_ADRESS;
    int PASSWORD_SERVER_PORT;
    int PASSWORD_LENGTH = 1;
    int PASSWORD_SYMBOLSET_SIZE = 4;
    int THREAD_COUNT = 1; 

    printInfo(argc, argv);
    if(argc > 1){
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
            if(c.conn(PASSWORD_SERVER_ADRESS, PASSWORD_SERVER_PORT)){
                cerr << "program terminated.\n";
                exit(0);
            }
    }else{
            if(!c.conn("localhost", 1234)){
                cerr << "program terminated test.\n";
                exit(0);
            }
    }

    std::cout << "client sends:" << pwd << endl;
    c.sendData(pwd);
    std::cout << "got response:" << c.receive(32) << endl;

    std::cout << "client sends:UPDATE PASSWORD,2,5" << endl;
    c.sendData("UPDATE PASSWORD,2,5");
    std::cout << "got response:" << c.receive(32) << endl;

    std::cout << "client sends:BYEBYE!" << endl;
    c.sendData("BYEBYE!");
    std::cout << "got response:" << c.receive(32) << endl;

    return 0;
}

void printInfo(int argc, char *argv[]){
    if(argc == 6 || argc == 1){
        return;
    }else{
        cerr << "Usage of " << argv[0] << " : \n\n"
             << argv[0] << " <server host> <server port> <password length> <password symbolset size> <thread count>" << endl;
        cerr << "\n"
             << "<server host>              hostname of running password server\n"
             << "<server port>              port of the running password server\n"
             << "<password length>          known (!0) or unknown (0) password length\n"
             << "<password symbolset size>  known (!0) or unknown (0) password symbolset size\n"
             << "<thread count>             number of threads that simultaneusly try to find the password\n";
        exit(0);
    };
};
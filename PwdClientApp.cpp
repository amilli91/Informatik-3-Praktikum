/*
 * PwdClient.cpp
 * 
 *  Created on: 23.12.2020 
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner
 */

#include <iostream>
#include <cstdlib>
#include <string.h>  //strcpy

#include "PwdClient.hpp"

void printInfo(int argc, char *argv[]);

char DEFAULT_PASSWORD_SERVER_ADRESS[] = "localhost";
char *PASSWORD_SERVER_ADRESS = DEFAULT_PASSWORD_SERVER_ADRESS;
int PASSWORD_SERVER_PORT = 1234;
int PASSWORD_LENGTH = 0;
int PASSWORD_SYMBOLSET_SIZE = 62;
int THREAD_COUNT = 1; 


int main(int argc, char *argv[]){

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
    }

    PwdClient pwdClnt(PASSWORD_LENGTH, PASSWORD_SYMBOLSET_SIZE);

    
    if(!pwdClnt.conn(PASSWORD_SERVER_ADRESS, PASSWORD_SERVER_PORT)){
        cerr << "program terminated.\n";
        exit(0);
    }

    
    cout << pwdClnt.bruteForce() << " tries needed." << endl;

    cout << "client sends:UPDATE PASSWORD[4,26]" << endl;
    pwdClnt.sendData("UPDATE PASSWORD[4,26]");
    cout << "got response:" << pwdClnt.receive(32) << endl;


    cout << "client sends:BYEBYE!" << endl;
    pwdClnt.sendData("BYEBYE!");
    cout << "got response:" << pwdClnt.receive(32) << endl;
    

    return 0;
}

void printInfo(int argc, char *argv[]){
    if(argc == 6){
        return;
    }else if(argc == 1){
        char usrInput;

        cout << "\n" << argv[0] << " will be initialized with these default parameters:\n\n"
             << "server host:               " << DEFAULT_PASSWORD_SERVER_ADRESS << "\t(hostname of running password server)\n"
             << "server port:               " << PASSWORD_SERVER_PORT << "\t\t(port of the running password server)\n"
             << "password length:           " << PASSWORD_LENGTH << "\t\t(known (not 0) or unknown (0) password length)\n"         
             << "password symbolset size:   " << PASSWORD_SYMBOLSET_SIZE << "\t\t(known (not 0) or unknown (0) password symbolset size)\n" 
             << "thread count:              " << THREAD_COUNT << "\t\t(number of threads that simultaneusly try to find the password)\n\n";

        cout << "Do you want to continue?\n"
             << "[y,n] ";

        cin >> usrInput;

        if(usrInput == 'y' || usrInput == 'Y'){
            return;
        }else{
            exit(0);
        }

    }else{
        cerr << "Usage of " << argv[0] << " : \n\n"
             << argv[0] << " <server host> <server port> <password length> <password symbolset size> <thread count>" << endl;
        cerr << "\n"
             << "<server host>              hostname of running password server\n"
             << "<server port>              port of the running password server\n"
             << "<password length>          known (not 0) or unknown (0) password length\n"
             << "<password symbolset size>  known (not 0) or unknown (0) password symbolset size\n"
             << "<thread count>             number of threads that simultaneusly try to find the password\n";
        exit(0);
    };
};


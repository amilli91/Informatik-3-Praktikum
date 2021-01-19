/*
 * PwdServerApp.hpp
 * 
 *  Created on: 17.12.2020
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner
 * 
 */


#include <iostream>
#include <cstdlib>
#include <thread>

#include "PwdServer.hpp"

void printInfo(int argc, char *argv[]);

int SERVER_PORT = 1234;
int MAX_DATA_SIZE_RECEIVE = 32;
int PASSWORD_LENGTH = 1;
int PASSWORD_SYMBOLSET_SIZE = 62;
int THREAD_COUNT = 1;

int main(int argc, char *argv[]){
    srand(time(nullptr));

    printInfo(argc, argv);
    if(argc > 1){
        try{
            SERVER_PORT              = atoi(argv[1]);
            PASSWORD_LENGTH          = atoi(argv[2]);
            PASSWORD_SYMBOLSET_SIZE  = atoi(argv[3]);
            THREAD_COUNT             = atoi(argv[4]);
        }catch(...){
            cerr << "Can't read all parameter values, terminate programm.\n\n";
            exit(0);
        }
    }

    PwdServerSingleThread pwdSvr(SERVER_PORT, MAX_DATA_SIZE_RECEIVE, PASSWORD_LENGTH, PASSWORD_SYMBOLSET_SIZE);
    
    pwdSvr.run();

    return 0;
}

void printInfo(int argc, char *argv[]){
    if(argc == 5){
        return;
    }else if(argc == 1){
        char usrInput;

        cout << "\n" << argv[0] << " will be initialized with these default parameters:\n\n"
             << "port:                          " << SERVER_PORT << "\t(port number this server listens on)\n"
             << "password length:               " << PASSWORD_LENGTH << "\t(number of characters in the generated password)\n"
             << "password symbolset size:       " << PASSWORD_SYMBOLSET_SIZE << "\t(number of different symbols for generating the password)\n"
             << "thread count:                  " << THREAD_COUNT << "\t(number of simultaneulsy available communication channels\n\n";

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
             << argv[0] << " <port> <password length> <password symbolset size> " << endl;
        cerr << "\n"
             << "<port>                         port number this server listens on\n"
             << "<password length>              number of characters in the generated password\n"
             << "<password symbolset size>      number of different symbols for generating the password\n" 
             << "<thread count>                 number of simultaneusly available communication channels\n";
        exit(0);
    };
};
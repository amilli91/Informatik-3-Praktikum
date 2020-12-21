/*
 * PwdServerApp.hpp
 * 
 *  Created on: 17.12.2020
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner
 * 
 */


#include <iostream>
#include <cstdlib>

#include "PwdServer.hpp"

void printInfo(int argc, char *argv[]);


int main(int argc, char *argv[]){

    int SERVER_PORT;
    int MAX_DATA_SIZE_RECEIVE = 25;
    int PASSWORD_LENGTH;
    int PASSWORD_SYMBOLSET_SIZE;

    printInfo(argc, argv);

    try{
        SERVER_PORT              = atoi(argv[1]);
        PASSWORD_LENGTH          = atoi(argv[2]);
        PASSWORD_SYMBOLSET_SIZE  = atoi(argv[3]);
    }catch(...){
        cerr << "Can't read all parameter values, terminate programm.\n\n";
        exit(0);
    }

    PwdServer pSvr(SERVER_PORT, MAX_DATA_SIZE_RECEIVE, PASSWORD_LENGTH, PASSWORD_SYMBOLSET_SIZE);
    
    pSvr.run();

    return 0;
}

void printInfo(int argc, char *argv[]){
    if(argc == 4){
        return;
    }else{
        cerr << "Usage of " << argv[0] << " : \n\n"
             << argv[0] << " <port> <password length> <password symbolset size> " << endl;
        cerr << "\n"
             << "<port>                         port number of this server\n"
             << "<password length>              number of characters in the generated password\n"
             << "<password symbolset size>      number of different symbols for generating the password\n"; 
        exit(0);
    };
};
/*
 * PwdClient.cpp
 * 
 *  Created on: 23.12.2020 
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner
 */

#include <iostream>     //cout; cerr
#include <fstream>      //fstream; ofstream
#include <string.h>     //strcpy
#include <sstream>      //stringcopy

#include "PwdClient.hpp"

void printInfo(int argc, char *argv[]);
bool statistics(PwdClientMultiThread *tmpClient);
void statisticParameters(int &minPwdLength, int &maxPwdLength, int &minPwdSymbSetSize, int &maxPwdSymbSetSize, int &iterations);

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
            THREAD_COUNT            = atoi(argv[3]);
        }catch(...){
            cerr << "Cant't read command line parameters, terminate process.\n";
            exit(0);
        }         
    }

    try{
        PwdClientMultiThread pwdClnt(THREAD_COUNT, PASSWORD_LENGTH, PASSWORD_SYMBOLSET_SIZE);
        
        if(!pwdClnt.connect(PASSWORD_SERVER_ADRESS, PASSWORD_SERVER_PORT)){
            cerr << "program terminated.\n";
            exit(0);
        }

        char usrInput;
        cout << "Do you want to want to create a bruteForce statistic?\n"
             << "[y,n] ";
        cin >> usrInput;

        if(usrInput == 'y' || usrInput == 'Y'){
            statistics(&pwdClnt);

            pwdClnt.disconnect();
        }else{
            usrInput = 'y';

            do{
                cout << "password length: ";
                cin >> PASSWORD_LENGTH;

                cout << "password symbolset size: ";
                cin >> PASSWORD_SYMBOLSET_SIZE;

                if(!pwdClnt.sendUpdateRequest(PASSWORD_LENGTH, PASSWORD_SYMBOLSET_SIZE)){
                    cout << "invalid input! please try again" << endl;
                    continue;
                }

                cout << pwdClnt.bruteForce() << " tries needed." << endl;

                cout << endl << "Do you want to find another password?\n"
                     << "[y,n] ";

                cin >> usrInput;

            }while(usrInput != 'n');

            pwdClnt.disconnect();
        }

    }catch(string s){
        cout << "Error: " << s << endl;
        exit(0);
    }catch(...){
        cout << "Unknown Error occured!" << endl;
        exit(0);
    }
    
    return 0;
}

bool statistics(PwdClientMultiThread *tmpClient){
    int sum, minPwdLength, maxPwdLength, minPwdSymbSetSize, maxPwdSymbSetSize, iterations = 0;
    int configCount = 1;
 
    statisticParameters(minPwdLength, maxPwdLength, minPwdSymbSetSize, maxPwdSymbSetSize, iterations);

    fstream test("Bruteforce_Stats.txt");
    ofstream stats;
    if(test.is_open()){
        test.close();
        stats.open("Bruteforce_Stats.txt", ios::app);
    }else{
        test.close();
        stats.open("Bruteforce_Stats.txt");
        stats << "Tries\tLength\tSize\t" << endl;
    }

    if(stats.good() != true){
        cout << endl << "Die Datei Bruteforce_Stats.txt konnte nicht beschrieben werden \n\n";
        return EXIT_FAILURE;
    }

    for(int currPwdSymbSetSize = minPwdSymbSetSize; currPwdSymbSetSize <= maxPwdSymbSetSize; currPwdSymbSetSize++){

        for(int currPwdLength = minPwdLength; currPwdLength <= maxPwdLength; currPwdLength++){
            cout << configCount << ". " << flush;
            sum = 0;
        
            for(int i=0; i < iterations; i++){
                tmpClient -> sendUpdateRequest(currPwdLength, currPwdSymbSetSize);
                sum += tmpClient -> bruteForce();
                cout << "#" << flush;
            }

            stats << (sum / iterations) << "\t";
            stats << currPwdLength << "\t";
            stats << currPwdSymbSetSize << "\n";
            cout << "\n" << flush;
            configCount++;
        }
    }

    stats.close();
    cout << "statistic done" << endl;
    return true;    
}

void printInfo(int argc, char *argv[]){
    if(argc == 4){
        return;
    }else if(argc == 1){
        char usrInput;

        cout << endl << "Usage of PwdClientApp : " << argv[0] << " <server host> <server port> <thread count>" << endl;

        cout << "\n" << argv[0] << " will be initialized with these default parameters:\n\n"
             << "server host:               " << DEFAULT_PASSWORD_SERVER_ADRESS << "\t(hostname of running password server)\n"
             << "server port:               " << PASSWORD_SERVER_PORT << "\t\t(port of the running password server)\n"
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
             << argv[0] << " <server host> <server port> <thread count>" << endl;
        cerr << "\n"
             << "<server host>              hostname of running password server\n"
             << "<server port>              port of the running password server\n"
             << "<thread count>             number of threads that simultaneusly try to find the password\n";
        exit(0);
    };
};

void statisticParameters(int &minPwdLength, int &maxPwdLength, int &minPwdSymbSetSize, int &maxPwdSymbSetSize, int &iterations){
    cout << endl << "The following parameters have to be specified:" << endl << endl;

    cout << "minimal password length:           lowest number of characters in the range of tested password configurations\n"
         << "maximal password length:           largest number of characters in the range of tested password configurations\n"
         << "minimal password symbolset size:   lowest number of different symbols in the range of tested password configurations\n"
         << "maximal password symbolset size:   largest number of different symbols in the range of tested password configurations\n"
         << "iterations per configuration:      number of passwords that will be tested for each password configuration\n\n";

    cout << "minimal password length: ";
    cin >> minPwdLength;
    cout << "maximal password length: ";
    cin >> maxPwdLength;
    cout << "minimal password symbolset size: ";
    cin >> minPwdSymbSetSize;
    cout << "maximal password symbolset size: ";
    cin >> maxPwdSymbSetSize;
    cout << "iterations per configuration: ";
    cin >> iterations;

    if( (minPwdLength < 1) || (maxPwdLength <= minPwdLength) || (minPwdSymbSetSize < 1) || (maxPwdSymbSetSize <= minPwdSymbSetSize) || (iterations < 1)){
        cout << endl << "invalid input! programm terminated." << endl;
        exit(0);
    }

    return;
}
/*
 * PwdClient.cpp
 * 
 *  Created on: 15.12.2020 
 *      Author: Luca Magdeburg, Maximilian Moss, Achim Werner
 */

#include <iostream> //cout
#include <cstring>  //strncpy
#include <sstream>  //stringstream
#include <math.h>   //pow()

#include "PwdClient.hpp"

PwdClient::PwdClient(int symbSetSize):
    TCPclient(){
    if(symbSetSize < 0){
        throw string("invalid value for symbSetSize!");
    }else if(symbSetSize > SYMBOLS.length() || symbSetSize == 0){
        lengthSymbArray_ = SYMBOLS.length();
    }else{
        lengthSymbArray_ = symbSetSize;
    }
    
    charSymbArray_ = new char [lengthSymbArray_ + 1];
    strncpy(charSymbArray_, SYMBOLS.c_str(), lengthSymbArray_);     //extract and save the subset of valid symbols

    bruteForceResult_ = 0;

    return;
}

PwdClient::~PwdClient(){
    if(charSymbArray_ != NULL){
        delete [] charSymbArray_;   //freeing memory if possible
    }

    return;
}

void PwdClient::setSymbSetSize(int newSymbSetSize){
    if(newSymbSetSize < 0){
        throw string("invalid value for symbSetSize!");
    }else if(newSymbSetSize > SYMBOLS.length() || newSymbSetSize == 0){
        lengthSymbArray_ = SYMBOLS.length();
    }else{
        lengthSymbArray_ = newSymbSetSize;
    }

    if(charSymbArray_ != NULL){
        delete [] charSymbArray_;
    }

    charSymbArray_ = new char [lengthSymbArray_ + 1];
    strncpy(charSymbArray_, SYMBOLS.c_str(), lengthSymbArray_);     //extract and save the new subset of valid symbols

    return;
}

bool PwdClient::bruteForce(bool *status, unsigned pwdLength, unsigned startPos, unsigned endPos){
    foundPwdLength_ = 0;
    bruteForceResult_ = 0;
    string pwdGuess, response;
    unsigned int n = 0;
    unsigned int index = 0;

    pwdGuess = string("");
    
    if(pwdLength > 0){
        n = startPos;
        if(endPos == 0){endPos = unsigned(pow(float(lengthSymbArray_), float(pwdLength)));}
        unsigned int tmpCounter = 0;

        for(unsigned int i = 0; i < pwdLength; i++){
            pwdGuess += charSymbArray_[0];
        }

        do{
            for(unsigned int i = 0; i < pwdLength; i++){
                if(i == 0){
                    pwdGuess.replace(i, 1, 1, charSymbArray_[unsigned(n % lengthSymbArray_)]);
                }else{
                    index = ( n / unsigned(pow(float(lengthSymbArray_), float(i))) ) % (lengthSymbArray_);
                    pwdGuess.replace(i, 1, 1, charSymbArray_[index]);
                }
            }

            cout << "client sends:" << pwdGuess << endl;
            this->sendData(pwdGuess);

            response = this->receive(32);
            cout << "got response:" << response << endl;

            n++;
            tmpCounter++;

        }while( (response.compare("ACCESS_GRANTED") != 0) && (n != endPos) && (*status != true) );

        bruteForceResult_ = tmpCounter;

        if(response.compare("ACCESS_GRANTED") != 0){
            //cout << "password could not be found with the given specifications!" << endl;
            return false;
        }

        *status = true;

        return true;

    }else{
        unsigned int lengthJmp = lengthSymbArray_;

        pwdGuess += charSymbArray_[0];
        do{
            unsigned int counterShft = 0;

            for(unsigned int i = 0; i < pwdGuess.length(); i++){
                if(i == 0){
                    pwdGuess.replace(i, 1, 1, charSymbArray_[unsigned(n % lengthSymbArray_)]);
                }else{
                    for(unsigned int j = 1; j <= i; j++){
                        counterShft += unsigned(pow(float(lengthSymbArray_), float(j)));
                    }
                    index = ( (n - counterShft) / unsigned(pow(float(lengthSymbArray_), float(i))) ) % (lengthSymbArray_);
                    pwdGuess.replace(i, 1, 1, charSymbArray_[index]);
                    counterShft = 0;
                }
            }

            cout << "client sends:" << pwdGuess << endl;
            this->sendData(pwdGuess);

            response = this->receive(32);
            cout << "got response:" << response << endl;

            n++;
            if( (n % lengthJmp) == 0){
                pwdGuess += charSymbArray_[0];
                lengthJmp += unsigned(pow(float(lengthSymbArray_), float(pwdGuess.length())));
            }

        }while( (response.compare("ACCESS_GRANTED") != 0) && (*status != true) );

        bruteForceResult_ = n;
        foundPwdLength_ = pwdGuess.length();

        return true;
    }   
}

unsigned PwdClient::getBruteForceResults(){
    return bruteForceResult_;       //returns the number of tries that the BruteForce-method made
}

unsigned PwdClient::getFoundPwdLength(){
    return foundPwdLength_;         //returns the length of the found password (useful if password length was set to unknown)
}

bool PwdClient::sendUpdateRequest(int newPwdLength, int newSymbSetSize){
    string response = string("");

    stringstream updateMsg;                                                                 //passing method-parameters into a stringstream to construct
    updateMsg << "UPDATE_PASSWORD[" << newPwdLength << "," << newSymbSetSize << "]";        //a message, that tells the server to change it's password

    this->sendData(updateMsg.str());
    response = this->receive(32);

    if(response.compare("PASSWORD_UPDATED") == 0){      //checks the servers response
        this->setSymbSetSize(newSymbSetSize);           //updates member-variable setSymbSetSize to the newSymbSetSize if the server accepts the new parameters

        return true;
    }

    return false;
}


PwdClientSingleThread::PwdClientSingleThread(int pwdLength, int symbSetSize):
    PwdClient(symbSetSize){
        if(pwdLength < 0){
            throw string("invalid value for pwdLength!");
        }
        pwdLength_ = pwdLength;

    return;
}

PwdClientSingleThread::~PwdClientSingleThread(){
    if(charSymbArray_ != NULL){
        delete [] charSymbArray_;   //freeing memory if possible
    }
    
    return;
}

unsigned PwdClientSingleThread::bruteForce(int pwdLength){
    bool cond = false;
    if(pwdLength < 0){
        pwdLength = pwdLength_;
    }

    this->PwdClient::bruteForce(&cond, pwdLength_, 0, 0);

    return bruteForceResult_;    
}


PwdClientMultiThread::PwdClientMultiThread(short threadCount, int pwdLength, int symbSetSize){
    if(threadCount < 1){
        throw string("invalid value for thread count!");
    }
    threadCount_ = threadCount;

    if(pwdLength < 0){
        throw string("invalid value for pwdLength!");
    }
    pwdLength_ = pwdLength;

    if(symbSetSize < 0){
        throw string("invalid value for symbSetSize!");
    }else if(symbSetSize > SYMBOLS.length() || symbSetSize == 0){
        symbSetSize_ = SYMBOLS.length();
    }else{
        symbSetSize_ = symbSetSize;
    }
    
    //threadResults_ = new unsigned[threadCount_];          //member currently unused; would be used if PwdClients could each crack a different password         

    pwdClients_ = new PwdClient*[threadCount_];             //allocating memory for threadCount number of PwdClients
    clientThreads_ = new std::thread*[threadCount_];        //allocating memory for threadCount number of std::threads

    for(int i = 0; i < threadCount_; i++){
        pwdClients_[i] = new PwdClient(symbSetSize_);       //creating the PwdClient-objects and storing a pointer to each object
    }

    return;
}

PwdClientMultiThread::~PwdClientMultiThread(){
    /*
    if(threadResults_ != NULL){         
        delete [] threadResults_;           //freeing memory if possible         
    }
    */

    if(pwdClients_ != NULL){                
        delete [] pwdClients_;              //freeing memory if possible
    }

    if(clientThreads_ != NULL){
        delete [] clientThreads_;           //freeing memory if possible
    }
}

bool PwdClientMultiThread::connect(string address, int port){       //tries to connect all PwdClient-objects to a PwdServer
    for(int i = 0; i < threadCount_; i++){
        if(!pwdClients_[i]->conn(address, port+i)){         //returns false if a connect-attempt fails
            return false;       
        }
    }
    return true;
}

unsigned PwdClientMultiThread::bruteForce(int pwdLength){
    unsigned joinedThreads = 0;     //variable that counts the number of joined Threads
    bool *isPwdCracked = new bool;  //bool pointer used to communicate between threads
    *isPwdCracked = false;
    unsigned totalTries = 0;        //Total number of tries that all threads make

    if(pwdLength < 0){              //if bruteForce() is called without a parameter the default parameter(-1) is used   
        pwdLength = pwdLength_;     //and the value in member pwdLength_ will be assumed for the password length 
    }

    if(pwdLength == 0){                         //bei pwdLength = 0 wird die Password-Länge des zu knackenden Passworts als unbekannt angenommen 
        cout << "pwdLength unknown" << endl;    //-> die Passwort-Länge startet mit 1 und wird erhört bis das Passwort gefunden wurde
        unsigned nextLength = 1;                //-> für die aktuelle Password-Länge werden alle möglichen Kombinationen ausprobiert 

        for(int i = 0; i < threadCount_; i++){
            clientThreads_[i] = new std::thread(&PwdClient::bruteForce, ref(pwdClients_[i]), ref(isPwdCracked), nextLength, 0, 0);  //create threads that execute the bruteForce-method with increasing password-length parameter
            nextLength++;                                                                                                           
        }

        while(joinedThreads < threadCount_){                //wait for all threads to join
            for(int i = 0; i < threadCount_; i++){
                if(clientThreads_[i]->joinable()){          //check if a thread is joinable (finished the bruteForce-method)
                    clientThreads_[i]->join();              //join the thead
                    totalTries += pwdClients_[i]->getBruteForceResults();   //extract the number of tries that the newly joined thread made
                    if(*isPwdCracked == false){             //check if the password is cracked yet
                        clientThreads_[i] = new std::thread(&PwdClient::bruteForce, ref(pwdClients_[i]), ref(isPwdCracked), nextLength, 0, 0);  //if the password isn't cracked yet another thread is created
                        nextLength++;                                                                                                           //with the smallest password-length that wasn't tried yet
                    }else{
                        joinedThreads++;    
                    }
                }
            }
        }

        return totalTries;

    }else{                          //
        unsigned maxTries = pow(symbSetSize_, pwdLength_);      //calculate maximum number of possible-password combinations with the given password-parameters
        unsigned triesPerThread = maxTries / threadCount_;      //calculate equal number of password-combinations for each thread to try out 

        for(int i = 0; i < threadCount_ - 1; i++){
            clientThreads_[i] = new std::thread(&PwdClient::bruteForce, ref(pwdClients_[i]), ref(isPwdCracked), pwdLength, (i*triesPerThread), ((i+1)*triesPerThread));                         //create threads that execute the bruteForce-method
        }                                                                                                                                                                                       //with the same password-length but in different  
        clientThreads_[threadCount_-1] = new std::thread(&PwdClient::bruteForce, ref(pwdClients_[threadCount_-1]), ref(isPwdCracked), pwdLength, ((threadCount_-1)*triesPerThread), maxTries);  //intervalls within all possible password-combinations

        cout << "after thread creation" << endl;

        while(joinedThreads < threadCount_){                //wait for all threads to join
            for(int i = 0; i < threadCount_; i++){          
                if(clientThreads_[i]->joinable()){          //check if a thread is joinable (finished the bruteForce-method)
                    clientThreads_[i]->join();              //joins the thread
                    joinedThreads++;
                    totalTries += pwdClients_[i]->getBruteForceResults();   //extract the number of thries that the newly joined thread made
                }
            }
        }
        return totalTries;
    }

}

/*
unsigned long long PwdClientMultiThread::getBruteForceResults(){
    unsigned long long totalTries = 0;
    for(int i = 0; i < threadCount_; i++){
        totalTries += threadResults_[i]; 
    }
    return totalTries;
}
*/

bool PwdClientMultiThread::sendUpdateRequest(int newPwdLength, int newSymbSetSize){
    if(pwdClients_[0]->sendUpdateRequest(newPwdLength, newSymbSetSize)){                //checking if the valus of newPwdLenth and newSymbSetSize are valid
        pwdLength_ = newPwdLength;                                                      //by checking the return-value of sendUpdateRequest from the first client
        symbSetSize_ = newSymbSetSize;

        for(int i = 1; i < threadCount_; i++){
            pwdClients_[0]->setSymbSetSize(newSymbSetSize);                             //updating the SymbolSet of every client-object
        }

        return true;
    }else{

        return false;
    }
}

void PwdClientMultiThread::disconnect(){        
    for(int i = 0; i < threadCount_; i++){
        pwdClients_[i]->sendData("BYEBYE!");    //sends "BYEBYE!" from every Passwort-Client to end communication
    }

    return;
}
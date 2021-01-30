/*
 * TASK1.cpp
 *
 *  Created on: 10.09.2019
 *      Author: aml
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "TASK1.hpp"
#include "SHA256.hpp"

namespace TASK1{


string BlackBoxUnsafe::getSymbols(){
	return string(charSymbArray_);
};

BlackBoxUnsafe::BlackBoxUnsafe(int pwdLength, int symbSetSize){
	if((symbSetSize < 1) || (symbSetSize > SYMBOLS.length()) ){
		lengthSymbArray_ = SYMBOLS.length();
	}else{
		lengthSymbArray_ = symbSetSize;
	}
	// extract and save the subset of valid symbols
	charSymbArray_ = new char [lengthSymbArray_ + 1];
	strncpy(charSymbArray_, SYMBOLS.c_str(), lengthSymbArray_);
	// create and save random password
	pwd_ = this->randomPwd(pwdLength);
	return;
}

string BlackBoxUnsafe::input(string strPwd){
	if(strPwd.compare(pwd_) == 0){
		return string("ACCESS ACCEPTED");
	}
	return string("ACCESS DENIED");
}

string BlackBoxUnsafe::randomPwd(int l){
	int symbolIdx;
	if(l < MINIMAL_PWD_LENGTH){ l = MINIMAL_PWD_LENGTH;};
	pwd_ = string("");
	for(int i=0; i < l; i++){
		symbolIdx = rand() % lengthSymbArray_;
		pwd_ += charSymbArray_[symbolIdx];
	}
	return pwd_;
}

BlackBoxSafe::BlackBoxSafe(int pwdLength, int symbSetSize) :
	BlackBoxUnsafe(pwdLength, symbSetSize){
	checkSum_ = sha256(pwd_);
	
	cout << pwd_ << endl;
	cout << checkSum_ << endl;

	pwd_ = string("");
	
	return;
}

BlackBoxSafe::~BlackBoxSafe(){
	if(charSymbArray_ != NULL){
		delete [] charSymbArray_;
	}
}

string BlackBoxSafe::input(string strPwd){
	if(checkSum_.compare(sha256(strPwd)) == 0){
		return string("ACCESS_GRANTED");
	}
	return string("ACCESS_DENIED");
}

void BlackBoxSafe::updatePwd(int pwdLength, int symbSetSize){
	if((symbSetSize < 1) || (symbSetSize > SYMBOLS.length()) ){
		lengthSymbArray_ = SYMBOLS.length();
	}else{
		lengthSymbArray_ = symbSetSize;
	}

	if(charSymbArray_ != NULL){
		delete[] charSymbArray_;
	}
	// extract and save the subset of valid symbols
	charSymbArray_ = new char [lengthSymbArray_ + 1];
	strncpy(charSymbArray_, SYMBOLS.c_str(), lengthSymbArray_);
	// create and save random password
	checkSum_ = sha256(this->randomPwd(pwdLength));

	cout << pwd_ << endl;
	cout << checkSum_ << endl;

	pwd_ = string ("");

	return;	
}

void demoTASK1_00(){
	string pwd("meinpasswort");
	cout << "pwd   : " << pwd << endl;
	cout << "SHA256: " << sha256(pwd) << endl;
	pwd = ("einpaswort");
	cout << "pwd   : " << pwd << endl;
	cout << "SHA256: " << sha256(pwd) << endl;
	return;
}

void demoTASK1_01(){
	TASK1::BlackBoxSafe bs(1, 4);
	cout << "pwd_ : " << bs.pwd_ << endl;
	cout << "checkSum_ : " << bs.checkSum_ << endl;

	cout << bs.input("A") << endl;

	return;
}

}
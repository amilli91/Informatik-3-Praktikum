//============================================================================
// Name        : INF3_Prak.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <string>
#include <iostream>

#include <cstdio>      /* printf, NULL */
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include <unistd.h>

#include "SHA256.hpp"
#include "TASK1.hpp"
#include "TASK2.hpp"
#include "TASK4.hpp"
#include "TASK6.hpp"
#include "PwdServer.hpp"

using namespace std;


int main(){
	srand(time(nullptr));

    TASK1::demoTASK1_01();

    return 0;
}






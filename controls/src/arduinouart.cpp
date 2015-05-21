#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <iostream>

#include "BlackUART.h"

using namespace std;
using namespace BlackLib;

int main(int argc, char** argv){
	std::string writeToUart1;
    std::ostringstream os1;

    BlackLib::BlackUART  Uart1(BlackLib::UART1,
                               BlackLib::Baud9600,
                               BlackLib::ParityEven,
                               BlackLib::StopOne,
                               BlackLib::Char8 );


    Uart1.open( BlackLib::ReadWrite | BlackLib::NonBlock );
//	string string1=argv[1];
int i;
    while (true){
        os1.str("");
        os1.clear();
        os1 <<i<<"\n";
        writeToUart1 = os1.str();
        Uart1 << writeToUart1;
	i+=5;
	usleep(5000);
	if(i==255)
	i=0;
    }

    return 1;
}


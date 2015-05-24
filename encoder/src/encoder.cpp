#include "encoder.h"
#include "serial_lnx.h"
	
	Encoder::Encoder(char *port, int baudRate) {
		
		serialConnection = new Tserial();
        serialConnection->connect(BOT_COM_PORT, BOT_BAUD_RATE, spNONE);
        usleep(100);
        
	}
	
	EncoderData Encoder::fetchEncoderData() {
		
		EncoderData returnValue;
		char input = ' ';
		
		returnValue.leftCount = 0;
		returnValue.rightCount = 0;
		
		do {
			input = serialConnection->getChar();
			if ( (input < 58 && input > 47)  || (input==45)) {
				returnValue.leftCount *= 10;
				returnValue.leftCount += input - 48;
			}
		} while (input != ' ');
		
		do {
			input = serialConnection->getChar();
			if ((input < 58 && input > 47 ) || (input==45)) {
				returnValue.rightCount *= 10;
				returnValue.rightCount += input - 48;
			}
		} while (input != ' ');
		
		return returnValue;
		
	}
	


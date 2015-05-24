#ifndef _ENCODER_H
#define _ENCODER_H

#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>
#include "serial_lnx.h"
#include "devices.h"




	
	class EncoderData {
		
		public:
		int leftCount;
		int rightCount;
		
	};
	
	class Encoder {
		
		
		private:
		Tserial *serialConnection;
		
		public:
		Encoder(char *port, int baudRate);
		EncoderData fetchEncoderData();
	};


#endif

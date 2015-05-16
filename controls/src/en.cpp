#include "eqep.h"
#include <string>
#include <iostream>

/*
encoder_msg
-> left_rpm	
-> right_rpm
*/


using namespace std;

int main (int argc, char** argv)
{
    
	//std::string eQEP0 = "/sys/devices/ocp.2/48300000.epwmss/48300180.eqep";
     //std::string eQEP1 = "/sys/devices/ocp.2/48302000.epwmss/48302180.eqep";
    //eQEP2 = "/sys/devices/ocp.2/48304000.epwmss/48304180.eqep"
	
	//left wheel encoder at epeq0
    eQEP left_wheel(std::string ("/sys/devices/ocp.3/48300000.epwmss/48300180.eqep"), eQEP::eQEP_Mode_Absolute);
    
	//right wheel encoder at eqep1
    eQEP right_wheel(std::string ("/sys/devices/ocp.3/48302000.epwmss/48302180.eqep"), eQEP::eQEP_Mode_Absolute);
    
	
	// Set the unit time period to 100,000,0 ns, or 0.001 seconds
    
    left_wheel.set_period(1000000L);
    right_wheel.set_period(1000000L);

    
	while(1)
    {
    
	float lw_rpm = (float)left_wheel.get_position()/3200*4;   
	float rw_rpm = (float)right_wheel.get_position()/3200*4;

	std::cout << "RPM_L = " << lw_rpm << " RPM_R = "<< rw_rpm <<std::endl;
	
    }
    
    
    return 0;
}

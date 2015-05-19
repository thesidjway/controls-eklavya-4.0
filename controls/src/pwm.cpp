#include <iostream>
#include <unistd.h>
#include "BlackPWM.h"
#include "BlackGPIO.h"
using namespace std;
using namespace BlackLib;

int main()
{
//BlackGPIO led1(GPIO_60,output);
BlackPWM pwmLed2(EHRPWM2A); // initialization pwm with second led
float percent =0.0;

pwmLed2.setPeriodTime(5000000); // set period of pwm to 5.000.000 nanosecond// set percentage of pwm to 0 // give some time to Beaglebone Black

percent=40;                                                                                                                                                          
while(1)
{
pwmLed2.setDutyPercent(75); // set percentage of pwm
}
return 0;
}



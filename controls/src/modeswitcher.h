#ifndef MODE_SWITCHER
#define MODE_SWITCHER

#include <ros/ros.h>
#include <unistd.h>
#include <mutex>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include "modeswitcher.h"

using namespace std; 
using namespace ros;


class ModeSwitcher {
	
private:
	double d;//distance between steering and the back tires in meters
	
	double maxalpha,minalpha;

	std::mutex Vx_Xbox_lock , W_Xbox_lock , Vx_planner_lock , W_planner_lock , xbox_flag_lock;

	float alpha;

	float W_xbox;
	float Vx_Xbox;

	float W_Planner;
	float Vx_Planner;
	
	double  Max_Xbox_Vx;
	//node handler for xbox data
	int xboxflag; //flag to tell whether xbox is currently sending data or not

	geometry_msgs::Twist finaltwist ;
	
public:

	ModeSwitcher();
	
	void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
	
	void planCallback(const geometry_msgs::Twist::ConstPtr& pose);
	
	void publish(int argc, char** argv);
};

#endif

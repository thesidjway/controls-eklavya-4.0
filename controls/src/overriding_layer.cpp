
#include <iostream>
#include "overriding_layer.h"


using namespace ros;
using namespace BlackLib;


OverridingLayer::OverridingLayer() :
	finaltwist(), Vx_Xbox_lock() , W_Xbox_lock() , Vx_planner_lock() , W_planner_lock() , xbox_flag_lock() , planner_flag_lock()
{

	estopflag=0; //flag to tell whether estop is currently enabled
	xboxflag=0; //flag to tell whether xbox is currently sending data or not
	planflag=0;
	
	MULTI_FACTOR=((maxalpha-minalpha)*100/18)*0.756;
	
}

void OverridingLayer::joyCallback(const sensor_msgs::Joy::ConstPtr& joy) //main callback function for xbox data
{

  	if ( (joy->axes[5]<1.0)&&(joy->axes[5]>= -1.0) ){
	
	xbox_flag_lock.lock();
	xboxflag=1;
	xbox_flag_lock.unlock();
	
	float val = joy->axes[5];       //Reading from the right top trigger
	float duty=100-((val+1)*50.0);  //duty cycle to be sent to the BLDC control function
	
	Vx_Xbox_lock.lock();
	
	Vx_Xbox=(duty*Max_Xbox_Vx)/100;
	
	Vx_Xbox_lock.unlock();
	
	}
 
	float dataaxes=joy->axes[2];
	
	if(dataaxes>=0.244) 
	{
		normalized=(dataaxes-0.244)*MULTI_FACTOR; 
		
		W_Xbox_lock.lock();
		W_xbox=(Vx_Xbox*tan(normalized))/d;
		W_Xbox_lock.unlock();
		
		xbox_flag_lock.lock();
		xboxflag=1;
		xbox_flag_lock.unlock();
	}
	else if(dataaxes<=-0.244)
	{
		normalized=(dataaxes+0.244)*MULTI_FACTOR;
		
		W_Xbox_lock.lock();
		W_xbox=(Vx_Xbox*tan(normalized))/d;
		W_Xbox_lock.unlock();
		
		xbox_flag_lock.lock();
		xboxflag=1;
		xbox_flag_lock.unlock();
	}
	

////////////////////////////////////////////////////////////////E STOP/////////////////////////////////////////////////////////////////////////////////////////////////
	
	if((joy->buttons[3]==1) && (estopflag==0)) //Setting E-stop on if it is not already on
	{
		estopflag=1;
		
		xbox_flag_lock.lock();
		xboxflag=1;
		xbox_flag_lock.unlock();
		
		cout<<"E-stop Enabled";
		cout<<endl;
	}
	if((joy->buttons[2]==1) && (estopflag==1)) //Setting E-stop off if it is not already off
	{
		estopflag=0;
		xbox_flag_lock.lock();
		xboxflag=1;
		
		xbox_flag_lock.unlock();
		cout<<"E-stop Disabled";
		cout<<endl;
	}
	
	BlackGPIO E_stop_enable(GPIO_31 ,output);
	
	 if(estopflag==0)
	{
		E_stop_enable.setValue(low); //set the pin to low, to disable the e-stop
	}
	else if(estopflag==1)
	{
		E_stop_enable.setValue(high); //set the pin to high to click the relay and activate the relay
	}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

void OverridingLayer::planCallback(const geometry_msgs::Twist::ConstPtr& pose)
{		
		xbox_flag_lock.lock();
		
		if(xboxflag==1)
		{
			return ;
		}
		else if(xboxflag==0)
		{
		planner_flag_lock.lock();
		planflag=1;
		planner_flag_lock.unlock();
		W_Planner = pose->angular.z; //calculate angle required to be sent
		Vx_Planner=pose->linear.x; //add data in transfer variable
		return ;
		}
		xbox_flag_lock.unlock();
}

 void OverridingLayer::publish(int argc, char** argv){

	ros::init(argc, argv, "overriding_layer");
	
	ros::NodeHandle nh_;
	ros::Rate loop_rate(200);
	ros::Subscriber joy_sub;
	ros::Subscriber plan_sub;  

  	joy_sub = nh_.subscribe <sensor_msgs::Joy> ("joy", 1000 , &OverridingLayer::joyCallback , this); 
  	plan_sub = nh_.subscribe <geometry_msgs::Twist> ("cmd_vel", 1000 , &OverridingLayer::planCallback, this); 
	nh_.getParam("/overriding_layer/maxvelocity", Max_Xbox_Vx);
	nh_.getParam("d",d);
	nh_.getParam("Alpha_Max",maxalpha);
	nh_.getParam("Alpha_Min",minalpha);

	ros::Publisher send_twist = nh_.advertise<geometry_msgs::Twist>("target_pose", 5);

	while(ros::ok)
	{
		xbox_flag_lock.lock();
		
		planner_flag_lock.lock();
		
		if(xboxflag==1)
		{
			Vx_Xbox_lock.lock();
				finaltwist.linear.x=Vx_Xbox;
			Vx_Xbox_lock.unlock();
			
			W_Xbox_lock.lock();
				finaltwist.angular.z= W_xbox;
			W_Xbox_lock.unlock();
			
			send_twist.publish(finaltwist);
			
			xboxflag=0;
			planflag=0;
			
		}
		else if(xboxflag==0)
		{
			if(planflag==1)
			{	Vx_planner_lock.lock();
					finaltwist.linear.x=Vx_Planner;
				Vx_planner_lock.unlock();
				
				W_planner_lock.lock();
					finaltwist.linear.x=W_Planner;
				W_planner_lock.unlock();
				
				planflag=0;
				
			}
		}
		
		planner_flag_lock.unlock();
		xbox_flag_lock.unlock();
		
		
	}
 }
 
 
 
int main(int argc, char** argv)
{	

	OverridingLayer * layer = new OverridingLayer();
	
	layer->publish(argc, argv);
	
	delete layer;
}

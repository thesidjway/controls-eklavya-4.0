#include <iostream>
#include "vx_pid.h"

using namespace std;

VxPid::VxPid(): Vx_t_lock() , Alpha_lock() , Vl_Vr_a_lock() { 
	
	
	

  }



double VxPid::getMinMax(int Cur_Var,int max,int min){ 

	if(Cur_Var>max)
	  return max;
	else if(Cur_Var<min)  
	  return min;
	else
	  return Cur_Var;
  }

void VxPid::vxTargetUpdateCallback(const geometry_msgs::Twist::ConstPtr& msg){
     
	 Vx_t_lock.lock();
		Alpha_lock.lock();
			Vx_t= (msg->linear.x );
		//	Vs_t= (msg->linear.x )/( cos( (Alpha_a * PI)/180));
		Alpha_lock.unlock();
	 Vx_t_lock.unlock();

  }

/*
void VxPid::Alpha_actual_callback(const std_msgs::Float64::ConstPtr& msg)
 {  
  ROS_INFO("\n Vs_PID_node: Alpha received \n");
   
 Alpha_lock.lock();
  Alpha_a=msg->data;
 Alpha_lock.unlock();
 
 }
*/

void VxPid::encoderCallback(const controls::encoder_msg::ConstPtr& msg)
 {

  ROS_INFO("\n Ws_PID: Encoder Reading Received \n");

	Vl_Vr_a_lock.lock();
		Vl_a=msg->left_vel;
		Vr_a=msg->right_vel ;
	Vl_Vr_a_lock.unlock(); 
 
 }

void VxPid::implementPid(int argc, char** argv){
	
	ros::init(argc, argv,"vxpid_node");

	ros::NodeHandle nh_; 
	ros::Subscriber Override_Subscriber = nh_.subscribe<geometry_msgs::Twist>("target_pose" , 5 , vxTargetUpdateCallback);
	//ros::Subscriber Alpha_Actual_Subscriber = nh_.subscribe<std_msgs::Float64>("alpha_val_actual" , 5 , Alpha_actual_callback);
	ros::Subscriber Encoder_Subscriber = nh_.subscribe<controls::encoder_msg>("encoders" , 5 , encoderCallback);

	//ros::Rate loop_rate(Vs_PID_loop_rate);


	nh_.getParam("/vxpid_node/Kp_Vx", Kp_Vx);
	nh_.getParam("/vxpid_node/Ki_Vx", Ki_Vx);
	nh_.getParam("/vxpid_node/Kd_Vx", Kd_Vx);
	//nh_.getParam("/Vs_PID/Vs_PID_loop_rate", Vs_PID_loop_rate);
	nh_.getParam("/vxpid_node/PWM_min", PWM_min_percent);
	nh_.getParam("/vxpid_node/PWM_max", PWM_max_percent);
	nh_.getParam("/vxpid_node/PWM_PERIOD_TIME", PWM_PERIOD_TIME);

	nh_.getParam("d", d); 					       // Front wheel center to rear wheel line center distance
	nh_.getParam("r", r);					       // Rear wheel center to center of line joining distance

	BlackLib::BlackPWM pwm_signal_pin(BlackLib::EHRPWM2A); 

	pwm_signal_pin.setPeriodTime(PWM_PERIOD_TIME);


	  while( ros::ok() ){
		
		/*
		Vl_Vr_a_lock.lock();
		Vx_t_lock.lock();
			Alpha_lock.lock();
				double Vs_error = Vs_t - Vs_a;
			Alpha_lock.unlock();
		Vx_t_lock.unlock();	   	
		Vl_Vr_a_lock.unlock();
		*/
		
		Vl_Vr_a_lock.lock();
		Vx_t_lock.lock();
			Alpha_lock.lock();
				double Vx_error = Vx_t - Vx_a;
			Alpha_lock.unlock();
		Vx_t_lock.unlock();	   	
		Vl_Vr_a_lock.unlock();
		
		
		
		Vx_error_diff = Vx_error_old - Vx_error;
		Vx_error_sum += Vx_error;
		Vx_error_old = Vx_error;

		Vx_error_integral = getMinMax( Vx_error_sum * Ki_Vx , PWM_max_percent , PWM_min_percent);
		
		PWM_Duty_Cycle = (Vx_error)*Kp_Vx + (Vx_error_integral) + (Vx_error_diff)*Kd_Vx;
		
		PWM_Duty_Cycle  = getMinMax( PWM_Duty_Cycle , PWM_max_percent , PWM_min_percent);
	 
		pwm_signal_pin.setDutyPercent(PWM_Duty_Cycle);

		ros::spin();

		//loop_rate.sleep();

		}	
	}


int main (int argc, char** argv) {

	VxPid * vxPid = new vxPid();
	
	vxPid->implementPid(argc,  argv);
	
	delete vxPid;

}
  

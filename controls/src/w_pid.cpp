 
#include "w_pid.h" 
#include <iostream>


WPid::WPid(): W_t_Lock() , Alpha_Lock() , Vl_Vr_a_lock() {
	
	W_t=0;
	Vr_a=0;
	Vl_a=0;
	Alpha_a=0;
	Alpha_t=0;
	Kp_W=0;
	Ki_W=0;
	Kd_W=0;

}

double WPid::getMinMax(int Cur_Var,int max,int min){ 
	
	if(Cur_Var>max)
      return max;
    else if(Cur_Var<min)  
      return min;
    else
      return Cur_Var;
  }

void WPid::encoderCallback(const controls::encoder_msg::ConstPtr& msg)
 {

  ROS_INFO("\n Ws_PID: Encoder Reading Received \n");

	Vl_Vr_a_lock.lock();
		Vl_a=msg->left_vel;
		Vr_a=msg->right_vel ;
	Vl_Vr_a_lock.unlock(); 
 
 }


void WPid::wTargetUpdateCallback(const geometry_msgs::Twist::ConstPtr& msg){

	W_t_Lock.lock();
		W_t=msg->angular.z;
	W_t_Lock.unlock();

}

void WPid::implementPid(int argc, char** argv){
	
	ros::init(argc, argv,"wpid_node");

	ros::NodeHandle pid_nh_; 

	ros::Subscriber Override_Subscriber = pid_nh_.subscribe<geometry_msgs::Twist>("target_pose" , 5 , vxTargetUpdateCallback);
	ros::Subscriber Encoder_Subscriber = pid_nh_.subscribe<controls::encoder_msg>("encoders" , 5 , encoderCallback);

	ros::Publisher alpha_pub = pid_nh_.advertise<std_msgs::Float64>("alpha_val_manipulated", 100); 


	pid_nh_.getParam("/wpid_node/Kp_W", Kp_W);
	pid_nh_.getParam("/wpid_node/Ki_W", Ki_W);
	pid_nh_.getParam("/wpid_node/Kd_W", Kd_W);
	//pid_nh_.getParam("/wpid_node/W_PID_loop_rate", W_PID_loop_rate);


	pid_nh_.getParam("Alpha_max", Alpha_max);
	pid_nh_.getParam("Alpha_min", Alpha_min);
	pid_nh_.getParam("d", d); 					// Front wheel center to rear wheel line center distance
	pid_nh_.getParam("r", r);					// Rear wheel center to center of line joining distance
			 
	ros::Rate loop_rate(W_PID_loop_rate);

	std_msgs::Float64 alpha_msg;
  
  while( ros::ok() ){
  
	Vl_Vr_a_lock.lock();
		W_t_Lock.lock();
			double W_error = W_t - W_a;
		W_t_Lock.unlock();   	
	Vl_Vr_a_lock.unlock();


	W_error_diff = W_error_old - W_error;
	W_error_sum += W_error;
	W_error_old = W_error;

	W_error_integral = getMinMax( (W_error_sum)*Ki_W , Alpha_max , Alpha_min);
		W_t_Lock.unlock();   	
	Vl_Vr_a_lock.unlock();


	W_error_diff = W_error_old - W_error;
	W_error_sum += W_error;
	W_error_old = W_error;

	W_error_integral = getMinMax( (W_error_sum)*Ki_W , Alpha_max , Alpha_min);
	
	double Alpha_manipulated = (W_error)*Kp_W + (W_error_integral) + (W_error_diff)*Kd_W;
	
	Alpha_manipulated = getMinMax( Alpha_manipulated , Alpha_max , Alpha_min );

	alpha_msg.data = Alpha_manipulated;

	alpha_pub.publish(alpha_msg);
	
	ros::spinOnce();
	loop_rate.sleep();

	}

}

int main (int argc, char** argv) 
{

	WPid *wPid = new WPid();
	wPid->implementPid(argc, argv);
	delete wPid;
	 

}
  


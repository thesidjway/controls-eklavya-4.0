#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <cstdlib>
#include <iostream>

class XboxData
{
public:
  XboxData();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  
  ros::Subscriber joy_sub_;
  
};


XboxData::XboxData()
{
  
  ros::NodeHandle nh_;

  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &XboxData::joyCallback, this);

}

void XboxData::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  
  int i;
  std::cout<<"Buttons : ";
  for (i = 0; i < 11; i++) {
	std::cout<<", "<<i<<" : "<<joy->buttons[i];
  
  }
  std::cout<<std::endl<<"Axes : ";
  for (i = 0; i < 8; i++) {
	std::cout<<", "<<i<<" : "<<joy->axes[i];
  
  }
  
  
  std::cout<<std::endl;
  float val = joy->axes[4];
  std::cout<<" float val="<<val;
  std::cout<<std::endl;
  
  
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "xbox_node");
  XboxData myXbox;

  ros::spin();
}

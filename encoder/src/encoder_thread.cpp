#include "encoder.h"
#include "serial_lnx.h"
#include <iostream>

#define factor 0.000024935
using namespace std ;


long prev_left=0,prev_right=0;

int main(int argc, char** argv) 
{
    
    Encoder encoder(ENCODER_COM_PORT, ENCODER_BAUD_RATE);
    EncoderData encoderData;

    ros::init(argc, argv, "encoder_node");

    ros::NodeHandle n;
    ros::Publisher encoder_pub = n.advertise<geometry_msgs::Pose2D>("encoders", 1);
   
    ros::Rate loop_rate(40);

    while (ros::ok()) {
        /* Fetch data from Quadrature Encoder and load it in local vars */
        encoderData = encoder.fetchEncoderData();


        geometry_msgs::Pose2D encoder_msg;

  if( (encoderData.leftCount>400000) || (encoderData.leftCount< -400000) )
     encoderData.leftCount=prev_left;

  if( (encoderData.rightCount>400000) || (encoderData.rightCount< -400000) )
     encoderData.rightCount=prev_right;

	prev_left=encoderData.leftCount;
	prev_right=encoderData.rightCount;

	encoder_msg.x = encoderData.leftCount; 
	encoder_msg.y = encoderData.rightCount; 

	encoder_msg.x = factor * encoder_msg.x ;
	encoder_msg.y = factor * encoder_msg.y ;

    encoder_pub.publish(encoder_msg);


    ros::spinOnce();
    loop_rate.sleep();

        // pthread_mutex_lock(&pose_mutex);

        /* Update the pose_data using the data in local vars */

        // pthread_mutex_unlock(&pose_mutex);

        // usleep(10);
    }

    return 0;
}

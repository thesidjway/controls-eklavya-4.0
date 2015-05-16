cmake_minimum_required(VERSION 2.8.3)
project(controls_msgs)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
)

 add_message_files(
 	FILES
    encoder_msg.msg
 )

generate_messages( )

catkin_package(
	CATKIN_DEPENDS roscpp  message_runtime
)


cmake_minimum_required(VERSION 2.8.3)

project(controls)

ADD_DEFINITIONS(-std=c++0x)


## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS 
	roscpp 
	rospy 
	std_msgs 
	sensor_msgs 
	genmsg 
	message_generation 
	joy
	diagnostic_updater
)

## Declare ROS messages and services
add_message_files(FILES encoder_msg.msg)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package(
#	INCLUDE_DIRS include
	LIBRARIES controls
	CATKIN_DEPENDS roscpp joy rospy geometry_msgs sensor_msgs std_msgs diagnostic_updater joy message_runtime
	)
	


## Build talker and listener
include_directories(
	${catkin_INCLUDE_DIRS}
	${roscpp_INCLUDE_DIRS}
	${rospy_INCLUDE_DIRS}
)

include_directories(include)



add_executable(encoders src/encoders.cpp src/eqep.cpp)
target_link_libraries(encoders ${catkin_LIBRARIES})
add_dependencies(encoders roscpp std_msgs geometry_msgs )

add_executable(test_encoders src/test_encoder.cpp)
target_link_libraries(test_encoders ${catkin_LIBRARIES})

add_executable(overriding_node src/overriding_layer.cpp src/BlackGPIO.cpp  src/BlackCore.cpp src/BlackPWM.cpp)
target_link_libraries(overriding_node ${catkin_LIBRARIES} ) 
add_dependencies(overriding_node roscpp rospy joy sensor_msgs std_msgs geometry_msgs diagnostic_updater)

add_executable(wpid_node src/w_pid.cpp )
target_link_libraries(wpid_node ${catkin_LIBRARIES})
add_dependencies(wpid_node roscpp std_msgs geometry_msgs)

add_executable(vxpid_node src/vx_pid.cpp src/BlackCore.cpp src/BlackPWM.cpp)
target_link_libraries(vxpid_node ${catkin_LIBRARIES})
add_dependencies(vxpid_node roscpp std_msgs geometry_msgs)





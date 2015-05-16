"encoder" node in "controls" package publishes both the rear wheels' RPM as a controls/encoder_msg on the topic "encoders" encoder_msg: float32 left_vel
float32 right_vel

eQEP2 = "/sys/devices/ocp.2/48304000.epwmss/48304180.eqep"

	0x0B0 0x33  P8_41 = GPIO2_10 = EQEP2A_in, MODE3 
	0x0B4 0x33  P8_42 = GPIO2_11 = EQEP2B_in, MODE3 */
right wheel encoder at eqep1

sudo echo bone_eqep1 > /sys/devices/bone_capemgr.*/slots");
eQEP1 = "/sys/devices/ocp.2/48302000.epwmss/48302180.eqep";

	0x0D4 0x32  P8_33 = GPIO0_9 = EQEP1B_in, MODE2
	0x0D0 0x32  P8_35 = GPIO0_8 = EQEP1A_in, MODE2 
left wheel encoder at epeq0

sudo echo bone_eqep0 > /sys/devices/bone_capemgr.*/slots
eQEP0 = "/sys/devices/ocp.2/48300000.epwmss/48300180.eqep";

	0x1A0 0x31  P9_42 = GPIO3_18 = EQEP0A_in, MODE1        
	0x1A4 0x31  P9_27 = GPIO3_19 = EQEP0B_in, MODE1 

Planner's pose is published on "cmd_vel" topic  (running on PC)
"joy" node publishes Xbox controller messages on "joy" topic by
"overriding_layer" node publishes final pose on "target_pose"
"wpid_node" publishes manipulated steering angle value on "alpha_val_manipulated"

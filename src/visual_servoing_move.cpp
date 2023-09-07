#include <iostream>
#include <math.h> 
#include <tf/tf.h>

#include <ros/ros.h>
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/SetModelState.h>
#include <geometry_msgs/Pose.h>


int main(int argc, char **argv){
    ros::init(argc, argv, "visual_servoing_move");
    ros::NodeHandle n;
    ros::ServiceClient srv_set;
    ros::ServiceClient srv_get;

    srv_set = n.serviceClient<gazebo_msgs::SetModelState>("gazebo/set_model_state");
    srv_get = n.serviceClient<gazebo_msgs::GetModelState>("gazebo/get_model_state");

    gazebo_msgs::SetModelState srv_msg_set;
    gazebo_msgs::GetModelState srv_msg_get;

    srv_msg_set.request.model_state.model_name = "ball";
    srv_msg_get.request.model_name = "ball";

    double amplitude = 1.0; // Amplitude of the Z motion
    double frequency = 0.05; // Frequency of the Z motion (in Hz)

    srv_get.call(srv_msg_get);
    double default_position_z = srv_msg_get.response.pose.position.z;
    while(ros::ok()){ 
        // ros::Rate rate(10);

        srv_get.call(srv_msg_get);

        geometry_msgs::Pose pose = srv_msg_get.response.pose;
        pose.position.z = default_position_z + amplitude * sin(2.0 * M_PI * frequency * ros::Time::now().toSec());

        srv_msg_set.request.model_state.pose = pose;


        if(srv_set.call(srv_msg_set)){
            // ROS_INFO("SUCCESS");
        }else{
            ROS_INFO("FAILED");
        }

        ros::spinOnce();
    }
    return 0;
}





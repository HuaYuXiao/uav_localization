#include <iostream>
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/TransformStamped.h>

ros::Publisher mavros_odom_pub_;
std::string subject_name;
std::string segment_name;
std::string child_frame_id;
std::string frame_id;

void viconCallback(const geometry_msgs::TransformStamped::ConstPtr& msg)
{
    nav_msgs::Odometry odom;
    odom.child_frame_id = child_frame_id;
    odom.header.frame_id = frame_id;
    odom.header.stamp = msg->header.stamp;
    odom.pose.pose.position.x = msg->transform.translation.x;
    odom.pose.pose.position.y = msg->transform.translation.y;
    odom.pose.pose.position.z = msg->transform.translation.z;
    odom.pose.pose.orientation.w = msg->transform.rotation.w;
    odom.pose.pose.orientation.x = msg->transform.rotation.x;
    odom.pose.pose.orientation.y = msg->transform.rotation.y;
    odom.pose.pose.orientation.z = msg->transform.rotation.z;
    mavros_odom_pub_.publish(odom);
}


int main(int argc, char** argv){
    ros::init(argc, argv, "vicon_to_mavros");
    ros::NodeHandle n("~");
    n.getParam("subject_name",subject_name);
    n.getParam("segment_name",segment_name);
    n.getParam("child_frame_id",child_frame_id);
    n.getParam("frame_id",frame_id);

    mavros_odom_pub_ = n.advertise<nav_msgs::Odometry>("/mavros/odometry/out", 10);
    ros::Subscriber sub = n.subscribe("/vicon/" + subject_name + "/" + segment_name, 1000, viconCallback);
    ros::spin();
    return 0;
}

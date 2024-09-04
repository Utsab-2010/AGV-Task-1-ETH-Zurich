#include "ros/ros.h"
#include "std_msgs/String.h"

int main( int argc, char** argv)
{
    ros::init(argc,argv,"pub");
    ros::NodeHandle nh;
    ros::Publisher topic_publisher = nh.advertise<std_msgs::String>("chat",10);
    ros::Rate loopRate(10);

    while(ros::ok()){
        std_msgs::String message;
        message.data = "dummy";
        ROS_INFO_STREAM(message.data);
        
        topic_publisher.publish(message);
        ros::spinOnce();
        loopRate.sleep();

    }
    return 0;
}
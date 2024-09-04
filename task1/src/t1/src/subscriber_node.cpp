#include "ros/ros.h"
#include "std_msgs/String.h"

void callback(const std_msgs::String& msg)
{
    ROS_INFO("The message published was: [%s]", msg.data.c_str());
}

int main( int argc, char** argv)
{
    ros::init(argc,argv,"sub");
    ros::NodeHandle nh;
      ROS_INFO("working1");

    ros::Subscriber topic_subscriber = nh.subscribe("chatter",10,callback);
 	 ROS_INFO("working2");
    
    ros::spin();

    return 0;
}

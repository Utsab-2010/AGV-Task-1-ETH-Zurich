#include <ros/ros.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <cmath>


class ros_node{
    private:
        ros::NodeHandle nh;
        ros::Subscriber sub;
        ros::Publisher pub;
        double mean = 0.0;
        double stdDev = 1.5;


        // Function to generate a single pair of Gaussian random variables using Box-Muller transform
        void boxMuller(double& z0, double& z1) {
            // Generate two uniform random numbers in the interval (0, 1)
            double u1 = static_cast<double>(rand()) / RAND_MAX;
            double u2 = static_cast<double>(rand()) / RAND_MAX;

            // Apply the Box-Muller transform
            z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
            z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);
        }

        // Function to generate Gaussian noise with given mean and standard deviation
        void generateGaussianNoise(double mean, double stdDev, int numSamples,double* output) {
            double z0, z1;
            int i = 0;
            while (i < numSamples) {
                boxMuller(z0, z1); 
                if (i < numSamples) {
                    output[i++] = mean + stdDev * z0;
                }
                if (i < numSamples) {
                    output[i++] = mean + stdDev * z1;
                }
                
            }
        }

    public:
        ros_node(){

            ROS_INFO("Gaussian Noise Node started....");

            sub=nh.subscribe("cmd_vel",100,&ros_node::callback,this);
            pub = nh.advertise<geometry_msgs::Vector3>("gaussian_noise_topic",100);
        }
    
        void callback(const geometry_msgs::Twist::ConstPtr &msg){
            srand(static_cast<unsigned>(time(0))); //seeds the random value or smth... idk(GPT'd this algorithm)
            double* noise = new double[3];

            ROS_INFO("Linear Speed:[%f,%f,%f]",msg->linear.x,msg->linear.y,msg->linear.z);

            generateGaussianNoise(mean,stdDev,3,noise);

            ROS_INFO("Noise: %f,%f,%f",noise[0],noise[1],noise[2]);

            geometry_msgs::Vector3 gaussian_speed;

            gaussian_speed.x = msg->linear.x + noise[0];
            gaussian_speed.y = msg->linear.y + noise[1];
            gaussian_speed.z = msg->linear.z + noise[2];

            ROS_INFO("Gaussian Speed:[%f,%f,%f]",gaussian_speed.x,gaussian_speed.y,gaussian_speed.z);
            pub.publish(gaussian_speed);
        }

};


int main( int argc, char** argv)
{
    ros::init(argc,argv,"gaussian_noise");
    ros_node Node;
        
    ros::spin();

    return 0;
}

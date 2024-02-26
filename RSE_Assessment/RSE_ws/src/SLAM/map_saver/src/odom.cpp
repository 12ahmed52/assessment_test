#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "sensor_msgs/PointCloud2.h"

class OdomRepublisher {
public:
    OdomRepublisher() {
        // Initialize the node handle
        nh = ros::NodeHandle("~");

        // Subscribe to the input odometry topic
        odom_sub = nh.subscribe("/steer_bot/ackermann_steering_controller/odom", 10, &OdomRepublisher::odomCallback, this);

        // Subscribe to the /steer_bot/points topic to get the timestamp
        points_sub = nh.subscribe("/steer_bot/points", 1, &OdomRepublisher::pointsCallback, this);

        // Advertise the republished odometry topic
        odom_pub = nh.advertise<nav_msgs::Odometry>("/odom_input", 10);
    }

    void odomCallback(const nav_msgs::Odometry::ConstPtr& msg) {
        // Republish the received odometry message with the frame ID changed to "odom"
        nav_msgs::Odometry republished_msg = *msg;
        republished_msg.header.frame_id = "odom";
        //republished_msg.header.stamp = latest_stamp; // Assign the timestamp from /steer_bot/points
        odom_pub.publish(republished_msg);
    }

    void pointsCallback(const sensor_msgs::PointCloud2::ConstPtr& msg) {
        // Store the timestamp from /steer_bot/points
        latest_stamp = msg->header.stamp;
    }

private:
    ros::NodeHandle nh;
    ros::Subscriber odom_sub;
    ros::Subscriber points_sub;
    ros::Publisher odom_pub;
    ros::Time latest_stamp; // Store the latest timestamp from /steer_bot/points
};

int main(int argc, char** argv) {
    // Initialize the ROS node
    ros::init(argc, argv, "odom_republisher_node");

    // Create an instance of the OdomRepublisher class
    OdomRepublisher odom_republisher;

    // Spin to process callbacks
    ros::spin();

    return 0;
}


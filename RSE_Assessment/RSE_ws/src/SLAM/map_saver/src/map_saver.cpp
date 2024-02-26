#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/io/pcd_io.h>

class MapSaverNode {
public:
    MapSaverNode() : nh_("~") {
        points_sub_ = nh_.subscribe("/hdl_graph_slam/map_points", 1, &MapSaverNode::pointsCallback, this);
    }

    void pointsCallback(const sensor_msgs::PointCloud2ConstPtr& msg) {
        pcl::PointCloud<pcl::PointXYZI> cloud;
        pcl::fromROSMsg(*msg, cloud);
        
        // Save the point cloud to a PCD file in the XYzi format
        pcl::io::savePCDFileASCII("/home/ahmed/map_points_xyzi_two.pcd", cloud);
        
        ROS_INFO("Point cloud saved to map_points_xyzi.pcd");
        
        // Shutdown the node after saving the point cloud
        ros::shutdown();
    }

private:
    ros::NodeHandle nh_;
    ros::Subscriber points_sub_;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "map_saver_node");

    MapSaverNode map_saver_node;

    ros::spin();

    return 0;
}


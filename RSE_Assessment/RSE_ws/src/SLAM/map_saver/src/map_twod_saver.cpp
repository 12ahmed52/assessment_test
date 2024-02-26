#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <opencv2/opencv.hpp>
#include <fstream>

class MapSaverNode {
public:
    MapSaverNode() {
        map_subscriber = nh.subscribe("/hdl_global_localization/bbs/gridmap", 1, &MapSaverNode::mapCallback, this);
        saved_map = false;
    }

    void mapCallback(const nav_msgs::OccupancyGrid::ConstPtr& map_msg) {
        if (!saved_map) {
            // Extract map metadata
            int width = map_msg->info.width;
            int height = map_msg->info.height;
            double resolution = map_msg->info.resolution;

            // Convert occupancy grid data to OpenCV image format
            cv::Mat map_image(height, width, CV_8UC1);
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    int index = i * width + j;
                    // Adjust the row index to match the correct orientation
                    int row = height - i - 1;
                    if (map_msg->data[index] == 0)
                        map_image.at<uchar>(row, j) = 255; // Free
                    else
                        map_image.at<uchar>(row, j) = 0; // Unknown
                }
            }

            // Save map image to PGM file
            std::string map_image_filename = "map.pgm";
            cv::imwrite(map_image_filename, map_image);

            // Save map metadata to YAML file
            std::string map_metadata_filename = "map.yaml";
            std::ofstream yaml_file(map_metadata_filename);
            if (yaml_file.is_open()) {
                yaml_file << "image: " << map_image_filename << std::endl;
                yaml_file << "resolution: " << resolution << std::endl;
                yaml_file << "origin: [" << map_msg->info.origin.position.x << ", "
                                          << map_msg->info.origin.position.y << ", "
                                          << map_msg->info.origin.position.z << "]" << std::endl;
                yaml_file.close();
                ROS_INFO("Map saved successfully.");
            } else {
                ROS_ERROR("Failed to save map metadata.");
            }

            // Unsubscribe from the map topic
            map_subscriber.shutdown();
            saved_map = true;
        }
    }

private:
    ros::NodeHandle nh;
    ros::Subscriber map_subscriber;
    bool saved_map;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "map_saver_node");
    MapSaverNode map_saver_node;
    ros::spin();
    return 0;
}


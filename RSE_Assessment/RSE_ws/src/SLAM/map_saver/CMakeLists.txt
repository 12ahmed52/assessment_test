cmake_minimum_required(VERSION 3.0.2)
project(map_saver)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  pcl_conversions
  cv_bridge
  image_transport
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(map_saver src/map_saver.cpp)
target_link_libraries(map_saver ${catkin_LIBRARIES})

add_executable(odom_republisher_node src/odom.cpp)
target_link_libraries(odom_republisher_node ${catkin_LIBRARIES})


add_executable(map_twod_saver_node src/map_twod_saver.cpp)
target_link_libraries(map_twod_saver_node ${catkin_LIBRARIES})

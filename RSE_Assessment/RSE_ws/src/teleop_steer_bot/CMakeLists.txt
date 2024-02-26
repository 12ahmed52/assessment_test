cmake_minimum_required(VERSION 3.0.2)
project(teleop_steer_bot)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  geometry_msgs
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(teleop_steer_bot src/teleop.cpp)

target_link_libraries(teleop_steer_bot
  ${catkin_LIBRARIES}
)

install(TARGETS teleop_steer_bot
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)


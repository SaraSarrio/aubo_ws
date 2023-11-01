cmake_minimum_required(VERSION 3.8)
project(control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)
find_package(gazebo REQUIRED)
find_package(gazebo_ros REQUIRED)
find_package(control_msgs REQUIRED)
find_package(trajectory_msgs REQUIRED)
# find_package(aubo_ros2_common REQUIRED)
find_package(gazebo_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

# find_package(rosidl_default_generators REQUIRED)
# rosidl_generate_interfaces(${PROJECT_NAME}
#   "msg/JointTrajectory.msg"
#  )

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

include_directories(
  include
)
include_directories(${GAZEBO_INCLUDE_DIRS})
add_executable(controller src/controller.cpp)
add_executable(dir_kin src/dir_kin.cpp)
add_executable(inv_kin src/inv_kin.cpp)
add_executable(test_pub src/test_pub.cpp)
add_executable(test_pub_q src/test_pub_q.cpp)
add_executable(sim_pub src/sim_pub.cpp)
add_executable(sim_control src/sim_control.cpp)

install(TARGETS
    controller
    dir_kin
    inv_kin
    test_pub_q
    test_pub
    sim_pub
    sim_control
  DESTINATION lib/${PROJECT_NAME}
)

install (DIRECTORY launch worlds
  DESTINATION share/${PROJECT_NAME})

ament_target_dependencies(controller rclcpp std_msgs trajectory_msgs  gazebo_msgs)
ament_target_dependencies(dir_kin rclcpp std_msgs)
ament_target_dependencies(inv_kin rclcpp std_msgs)
ament_target_dependencies(test_pub rclcpp std_msgs) 
ament_target_dependencies(test_pub_q rclcpp std_msgs)
ament_target_dependencies(sim_pub rclcpp std_msgs gazebo_msgs gazebo gazebo_ros)
ament_target_dependencies(sim_control rclcpp rclcpp_action rclcpp_components std_msgs control_msgs trajectory_msgs  )

# ament_target_dependencies(collisionsusbcriber rclcpp std_msgs) 
ament_package()

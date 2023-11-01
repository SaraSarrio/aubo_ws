#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"

#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/contact.pb.h>  // Si estás trabajando con contactos

#include "gazebo_msgs/msg/contact_state.hpp"  // Si estás trabajando con contactos

class Sim_pub : public rclcpp::Node
{
public:
    Sim_pub();
        void publish_method();
        void funcion(const std_msgs::msg::Float32MultiArray::SharedPtr msg);
         trajectory_msgs::msg::JointTrajectory sim_goal;
    ~Sim_pub();

private:
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pub_;
    std_msgs::msg::Bool previous_state_;


};

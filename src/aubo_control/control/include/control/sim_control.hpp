#include <functional>
#include <future>
#include <memory>
#include <string>
#include <sstream>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/executors.hpp"

#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "control_msgs/action/follow_joint_trajectory.hpp"

class Sim_control : public rclcpp::Node
{
public:
    Sim_control();
        void send_goal();
        void publish_method();
        // void feedback_callback(const rclcpp_action::ClientGoalHandle<control_msgs::action::FollowJointTrajectory>::SharedPtr goal_handle);
        void funcion(const std_msgs::msg::Float32MultiArray::SharedPtr msg);


    ~Sim_control();
      rclcpp_action::Client<control_msgs::action::FollowJointTrajectory>::SharedPtr action_client_;

      control_msgs::action::FollowJointTrajectory::Goal goal_;

private:
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr sub_;

    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pub_;
    float q0;
    float q1;
    float q2;
    float q3;
    float q4;
    float q5;
    int pub_int_;
   // std_msgs::msg::Bool previous_state_;

};




#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

#include "trajectory_msgs/msg/joint_trajectory.hpp"
#include "trajectory_msgs/msg/joint_trajectory_point.hpp"

#include "gazebo_msgs/msg/contacts_state.hpp"

class Controller : public rclcpp::Node
{
public:
    Controller();

    void goal_sub_f(const std_msgs::msg::Float32MultiArray::SharedPtr msg);
    //void sim_sub_f(const std_msgs::msg::Float32MultiArray::SharedPtr msgrec);
    void sim_sub_f(const std_msgs::msg::Bool state);

    void coll_sub_f(const std_msgs::msg::Bool msg_coll_robot);

    void publish_user();
    void publish_sim();
    void publish_robot();

    ~Controller();

private:

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_sim_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_user_;

    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr sub_goal_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_sim_;

    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_coll_;


    std_msgs::msg::Float32MultiArray msg;
    std_msgs::msg::Float32MultiArray goal;
    std_msgs::msg::Float32MultiArray msgrec;
    trajectory_msgs::msg::JointTrajectoryPoint msg_robot;


    int pub_ind_user=0;
    int pub_ind_sim=0;
    int pub_ind_robot=0;
};

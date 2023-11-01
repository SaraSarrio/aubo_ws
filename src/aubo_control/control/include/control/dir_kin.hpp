
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

class Dir_kin : public rclcpp::Node
{
public:
    Dir_kin();

    void funcion(const std_msgs::msg::Float32MultiArray::SharedPtr msg);
    void publish_method();

    ~Dir_kin();

private:
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_;

    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr sub_;

    double T[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double q0[6];
    std_msgs::msg::Float32MultiArray msg;
    int pub_ind=0;

};

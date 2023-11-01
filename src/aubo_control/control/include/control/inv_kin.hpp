
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
//FALTAN MIL COSAS

class Inv_kin : public rclcpp::Node
{
public:
    Inv_kin();

    void funcion(const std_msgs::msg::Float32MultiArray::SharedPtr msg);
    void publish_method();

    ~Inv_kin();

private:
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_;

    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr sub_;
    double c_goal[6]={0.097,-0.128,0.959,0.0,0.0,0.0};
    double j_goal[6]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    int pub_ind=0;
    int est_codo=1;
};

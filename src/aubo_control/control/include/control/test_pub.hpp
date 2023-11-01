
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

class Test_pub : public rclcpp::Node
{
public:
    Test_pub();
     void publish_method();


    ~Test_pub();

private:
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_;
    int pub_ind;
    float x;
    float y;
    float z;
    float alpha;
    float beta;
    float gamma;
};

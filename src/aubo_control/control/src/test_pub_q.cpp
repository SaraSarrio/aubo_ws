

#include <control/test_pub.hpp>

// LO DE PUN_IN NO HACE NADA PARA QUE SOLO PUBLIQUE UNA VEZ HE QUITADO EL BUCLE WHILE DEL MAIN
Test_pub::Test_pub():Node ("test_pub_q")
{
    pub_=this -> create_publisher<std_msgs::msg::Float32MultiArray>("/Joint_goal",1000);

    std_msgs::msg::Float32MultiArray msg;
    int pub_ind=1;
    float x=0.2;
    float y=-0.5;
    float z=0.5;
    float alpha=1.57;
    float beta=3.14;
    float gamma=0.0;


}

Test_pub::~Test_pub() //Destructor
{
  printf("Leaving gently\n");
}

void Test_pub::publish_method()
{
    if(pub_ind=1){
        char input = getchar();
        switch (input){
        case 'A':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected A\r\n");
            x=x+0.1;
        break;

        case 'S':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected S\r\n");
             y=y+0.1;
        break;

        case 'D':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected D\r\n");
             z=z+0.1;
        break;

        case 'F':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected F\r\n");
             alpha=alpha+0.1;
        break;

        case 'G':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected G\r\n");
            beta=beta+0.1;
        break;

        case 'H':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected H\r\n");
            gamma=gamma+0.1;
        break;
        case 'Q':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected A\r\n");
            x=x-0.1;
        break;

        case 'W':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected S\r\n");
             y=y-0.1;
        break;

        case 'E':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected D\r\n");
             z=z-0.1;
        break;

        case 'R':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected F\r\n");
             alpha=alpha-0.1;
        break;

        case 'T':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected G\r\n");
            beta=beta-0.1;
        break;

        case 'Y':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected H\r\n");
            gamma=gamma-0.1;
        break;
        case 'C':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected C (CASA)\r\n");
             x=0.0;
             y=0.0;
             z=0.0;
             alpha=0.0;
             beta=0.0;
             gamma=0.0;
        break;

        case 'X':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected X (TODO 0)\r\n");
             x=0.0;
             y=-0.2;
             z=0.985;
             alpha=0.0;
             beta=1.57;
             gamma=0.0;


        break;
        case 'Z':
            RCLCPP_INFO(this->get_logger(), "[KeyController] Detected Z (TODO 0)\r\n");
             x=0.0;
             y=-1.2;
             z=0.9 ;
             alpha=0.0;
             beta=0.0;
             gamma=0.0;


        break;

        default:
            RCLCPP_INFO(this->get_logger(), "NULL\r\n");
        }
        std_msgs::msg::Float32MultiArray msg;
        msg.data.clear();

        msg.data.push_back(x);
        msg.data.push_back(y);
        msg.data.push_back(z);
        msg.data.push_back(alpha);
        msg.data.push_back(beta);
        msg.data.push_back(gamma);
        pub_-> publish(msg);//Publish the message
        pub_ind=0;
           printf("%1.6f %1.6f %1.6f %1.6f %1.6f %1.6f\n", msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5]);

    printf("\n");
    }

}

int main (int argc,char * argv[])
{
    rclcpp::init(argc,argv);

    auto node = std::make_shared<Test_pub>();

    rclcpp::Rate loop_rate(1);
    while (rclcpp::ok)
    {

        rclcpp::spin_some(node);
        node->publish_method();

    }

    rclcpp::shutdown();
    return 0;
}

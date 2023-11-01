#include <control/dir_kin.hpp>
#include <control/configuracion.hpp>
#include <control/aubo_kin_mod.hpp>

using std::placeholders::_1;

Dir_kin::Dir_kin():Node ("dir_kin")
{
    pub_=this -> create_publisher<std_msgs::msg::Float32MultiArray>("Cartesian_pos",1000);
    sub_= this -> create_subscription <std_msgs::msg::Float32MultiArray>("Joint_pos",10,std::bind(&Dir_kin::funcion,this, _1));
    double T[16] = {0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double q0[6];
    std_msgs::msg::Float32MultiArray msg;
    int pub_ind=0;
}

Dir_kin::~Dir_kin() //Destructor
{
  printf("Leaving gently\n");
}

void Dir_kin::funcion(const std_msgs::msg::Float32MultiArray::SharedPtr msgrec) {
    printf("recibo\n");
    //RECEPCION DE Q0
    q0[0]=msgrec-> data[0];
    q0[1]=msgrec-> data[1];
    q0[2]=msgrec-> data[2];
    q0[3]=msgrec-> data[3];
    q0[4]=msgrec-> data[4];
    q0[5]=msgrec-> data[5];
    //TRASFORMADA DIRECTA

    forward(q0, T);

    double x=T[3];
    double y=T[7];
    double z=T[11];
    double alpha=atan((T[1]*T[4]-T[5]*T[8])/(T[0]*T[9]-T[8]*T[1]));
    double beta=atan((T[2]*T[2]+T[7]*T[7]+T[8]*T[8]+T[9]*T[9])/(2*T[10]));
    double gamma=atan((T[2]*T[5]-T[1]*T[6])/(T[0]*T[6]-T[2]*T[4]));;
    msg.data.clear();

    msg.data.push_back(x);
    msg.data.push_back(y);
    msg.data.push_back(z);
    msg.data.push_back(alpha);
    msg.data.push_back(beta);
    msg.data.push_back(gamma);

    pub_ind=1;

}

void Dir_kin::publish_method()
{

    if (pub_ind==1){
    printf("publico\n");
    pub_-> publish(msg); //Publicación del mensaje
    }
    pub_ind=0;


}

int main (int argc,char * argv[])
{
    rclcpp::init(argc,argv);

    auto node = std::make_shared<Dir_kin>();

    rclcpp::Rate loop_rate(5);
    while (rclcpp::ok)
    {
        rclcpp::spin_some(node);
        node->publish_method();
    }
    rclcpp::shutdown();
    return 0;
}

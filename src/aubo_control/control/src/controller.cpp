#include <control/controller.hpp>
#include <control/configuracion.hpp>
#include <control/aubo_kin_mod.hpp>

using std::placeholders::_1;

Controller::Controller():Node ("controller")
{
    pub_user_=this -> create_publisher<std_msgs::msg::Float32MultiArray>("/Joint_pos",10);

    pub_sim_=this -> create_publisher<std_msgs::msg::Float32MultiArray>("/Sim_goal",10);

    sub_goal_= this -> create_subscription <std_msgs::msg::Float32MultiArray>("/Joint_goal",10,std::bind(&Controller::goal_sub_f,this, _1));

    sub_coll_= this -> create_subscription <std_msgs::msg::Bool>("/Collisions",1000,std::bind(&Controller::coll_sub_f,this, _1));

    sub_sim_= this -> create_subscription <std_msgs::msg::Bool>("/Sim_pos",1000,std::bind(&Controller::sim_sub_f,this, _1));

    std_msgs::msg::Float32MultiArray msg;
    std_msgs::msg::Float32MultiArray goal;
    std_msgs::msg::Float32MultiArray msgrec;

    int pub_ind_user=0;
    int pub_ind_sim=0;
    int pub_ind_robot=0;
}

Controller::~Controller() //Destructor
{
  printf("Leaving gently\n");
}

void Controller::goal_sub_f(const std_msgs::msg::Float32MultiArray::SharedPtr msgrec) {
    // printf("recibo pose goal\n");

	if(codo_valido(msgrec->data[0])==1 &&
	cuerpo_alante(msgrec->data[0], msgrec->data[1]) &&
	codo_arriba(msgrec->data[0], msgrec->data[2])==1 &&
	cabeza_erguida(msgrec->data[3])==1 &&
	cabeza_frente(msgrec->data[4])==1){

        goal.data.clear();
        goal.data.push_back(float(msgrec-> data[0]));
        goal.data.push_back(float(msgrec-> data[1]));
        goal.data.push_back(float(msgrec-> data[2]));
        goal.data.push_back(float(msgrec-> data[3]));
        goal.data.push_back(float(msgrec-> data[4]));
        goal.data.push_back(float(msgrec-> data[5]));
        // printf("esto es lo que pongo en goal\n");
		pub_ind_sim=1;
         RCLCPP_INFO(get_logger(), "El valor de goal es: [%f, %f, %f, %f, %f, %f]", goal.data[0], goal.data[1], goal.data[2], goal.data[3], goal.data[4], goal.data[5]);
	}
}

void Controller::sim_sub_f(const std_msgs::msg::Bool state) {

    if(state.data){
        RCLCPP_INFO(get_logger(), "Se ha podido alcanzar la posición: [%f, %f, %f, %f, %f, %f]", goal.data[0], goal.data[1], goal.data[2], goal.data[3], goal.data[4], goal.data[5]);
        pub_ind_user=1;
        pub_ind_robot=1;
    }


}

void Controller::coll_sub_f(const std_msgs::msg::Bool msg_coll_robot) {

     if(msg_coll_robot.data ){ //esta true porque ha detectado colision

        goal.data.clear();
        goal.data.push_back(0.0);
        goal.data.push_back(0.0);
        goal.data.push_back(0.0);
        goal.data.push_back(0.0);
        goal.data.push_back(0.0);
        goal.data.push_back(0.0);
        RCLCPP_ERROR(get_logger(), "NO SE HA PODIDO ALCANZAR LA POSICIÓN, VUELVE A POSICIÓN HOME");
        pub_ind_sim=1;
    }
    else{
        RCLCPP_INFO(get_logger(), "Se ha podido alcanzar la posición: [%f, %f, %f, %f, %f, %f]", goal.data[0], goal.data[1], goal.data[2], goal.data[3], goal.data[4], goal.data[5]);
        pub_ind_user=1;
        pub_ind_robot=1;
    }
}

void Controller::publish_sim(){

    if (pub_ind_sim==1){

    printf("publico simulacion\n");
    pub_sim_-> publish(goal); //Publicación del mensaje

    }
    pub_ind_sim=0;
}

void Controller::publish_user(){


    if (pub_ind_user==1){
    // printf("publico al usuario\n");
    pub_user_-> publish(goal); //Publicación del mensaje
    }
    pub_ind_user=0;
}

void Controller::publish_robot(){

    if (pub_ind_robot==1){
    // printf("publico al robot\n");

    }
    pub_ind_robot=0;
}

int main (int argc,char * argv[])
{
    rclcpp::init(argc,argv);

    auto node = std::make_shared<Controller>();

    rclcpp::Rate loop_rate(5);
    while (rclcpp::ok())
    {
        rclcpp::spin_some(node);
        node->publish_sim();
        node->publish_robot();
        node->publish_user();
    }
    rclcpp::shutdown();
    return 0;
}


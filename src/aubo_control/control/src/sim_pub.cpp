

#include <control/sim_pub.hpp>

using std::placeholders::_1;

Sim_pub::Sim_pub():Node ("sim_pub")
{
    pub_=this -> create_publisher<std_msgs::msg::Bool>("/Collisions",1000);
    previous_state_.data = false;
}

Sim_pub::~Sim_pub() //Destructor
{
  printf("Leaving gently\n");
}





gazebo::transport::NodePtr gz_node;
int pub_ind_=0;

void Sim_pub::publish_method()
{
    auto state = std::make_shared<std_msgs::msg::Bool>();
    state->data = false;

    if(pub_ind_){

        state->data = true;
        pub_ind_=0;
    }

    if (state->data != previous_state_.data)
    {
        pub_->publish(*state);
    }
    previous_state_.data = state->data; // Actualizamos el estado

}

// Function is called everytime a message is received.
void cb(ConstContactsPtr &_msg)
{
    // std::cout << _msg->DebugString();
    for(unsigned int i=0; i < _msg->contact_size(); i++){
         //for(unsigned int j=0; j < _msg->wrench_size(); j++){
            google::protobuf::uint32  id1 = _msg -> contact(i).wrench(0).body_1_id();
            google::protobuf::uint32  id2 = _msg -> contact(i).wrench(0).body_2_id();
            const std::string name1 = _msg->contact(i).wrench(0).body_1_name();
            const std::string name2 = _msg->contact(i).wrench(0).body_2_name();
            const char* name1_cstr = name1.c_str();
            const char* name2_cstr = name2.c_str();
            if(id1!=6 and id2!=6  )// and id1!=99 and id2!=99 and id1!=23 and id2!=23  nos aseguramos que no sea una rueda contra el suelo
            {
                  printf("HA COLISIONADO %s CON ID %d Y %s CON ID %d\n",name1_cstr ,id1,name2_cstr,id2);
                  printf(" ----\n" );
                 pub_ind_=1;

            }
    }


}

int main (int argc,char * argv[])
{
    rclcpp::init(argc,argv);

    gazebo::client::setup(argc, argv);

    gz_node = gazebo::transport::NodePtr(new gazebo::transport::Node());
    gz_node->Init();

    gazebo::transport::SubscriberPtr sub = gz_node->Subscribe("/gazebo/default/physics/contacts", cb);

    auto node = std::make_shared<Sim_pub>();

    rclcpp::Rate loop_rate(1);
    while (rclcpp::ok)
    {
        gazebo::common::Time::MSleep(10);
        node->publish_method();

    }

    rclcpp::shutdown();
    gazebo::client::shutdown();
    return 0;
}

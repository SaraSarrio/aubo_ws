#include <control/sim_control.hpp>
//#include "rclcpp/executors.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

Sim_control::Sim_control():Node ("sim_control")
{
  action_client_ = rclcpp_action::create_client<control_msgs::action::FollowJointTrajectory>(this,"/arm_controller/follow_joint_trajectory");

  pub_=this -> create_publisher<std_msgs::msg::Bool>("/Sim_pos",10);

  goal_ = control_msgs::action::FollowJointTrajectory::Goal();

  sub_= this -> create_subscription <std_msgs::msg::Float32MultiArray>("/Sim_goal",10,std::bind(&Sim_control::funcion,this, _1));

    float q0=0.0;
    float q1=0.0;
    float q2=0.0;
    float q3=0.0;
    float q4=0.0;
    float q5=0.0;
    int pub_int_=0;

}

Sim_control::~Sim_control() //Destructor
{
  printf("Leaving gently\n");
}
rclcpp_action::ResultCode common_resultcode = rclcpp_action::ResultCode::UNKNOWN;
int common_action_result_code = control_msgs::action::FollowJointTrajectory_Result::SUCCESSFUL;
auto state = std::make_shared<std_msgs::msg::Bool>();
int arrive;

void Sim_control::funcion(const std_msgs::msg::Float32MultiArray::SharedPtr msgrec) {
    q0=msgrec-> data[0];
    q1=msgrec-> data[1];
    q2=msgrec-> data[2];
    q3=msgrec-> data[3];
    q4=msgrec-> data[4];
    q5=msgrec-> data[5];
    pub_int_=1;

}

void Sim_control::publish_method()
{
    if(arrive){
        state->data = true; // Establece el valor bool que deseas publicar
        arrive=0;
            pub_-> publish(*state);
    }
}

void common_result_response(
  const rclcpp_action::ClientGoalHandle
  <control_msgs::action::FollowJointTrajectory>::WrappedResult & result)
{
  printf("common_result_response time: %f\n", rclcpp::Clock().now().seconds());
  common_resultcode = result.code;
  common_action_result_code = result.result->error_code;
  switch (result.code) {
    case rclcpp_action::ResultCode::SUCCEEDED:
      printf("SUCCEEDED result code\n");
      arrive=1;
      break;
    case rclcpp_action::ResultCode::ABORTED:
      printf("Goal was aborted\n");
      return;
    case rclcpp_action::ResultCode::CANCELED:
      printf("Goal was canceled\n");
      return;
    default:
      printf("Unknown result code\n");
      return;
  }
}



void common_feedback(
  rclcpp_action::ClientGoalHandle<control_msgs::action::FollowJointTrajectory>::SharedPtr,
  const std::shared_ptr<const control_msgs::action::FollowJointTrajectory::Feedback> feedback)
{
  std::cout << "feedback->desired.positions :";
  for (auto & x : feedback->desired.positions) {
    std::cout << x << "\t";
  }
  std::cout << std::endl;
  std::cout << "feedback->desired.velocities :";
  for (auto & x : feedback->desired.velocities) {
    std::cout << x << "\t";
  }
  std::cout << std::endl;
}

void Sim_control::send_goal()
  {

  using namespace std::chrono_literals; //esto es para poder ponde 2s

  auto points = std::make_shared<std::vector<trajectory_msgs::msg::JointTrajectoryPoint>>();

  auto point1_msg = std::make_shared<trajectory_msgs::msg::JointTrajectoryPoint>();

  point1_msg->positions ={q0,q1,q2,q3,q4,q5};
  point1_msg->time_from_start.sec = 2;

  auto point2_msg = std::make_shared<trajectory_msgs::msg::JointTrajectoryPoint>();

  point2_msg->positions ={0.0, 1.57, 0.5, 0.0, 0.0, 0.0};
  point2_msg->time_from_start.sec = 4;

  points->push_back(*point1_msg);
  //points->push_back(*point2_msg);

  auto joint_names = std::vector<std::string>{"shoulder_joint", "upperArm_joint", "foreArm_joint", "wrist1_joint", "wrist2_joint" , "wrist3_joint" };

  auto goal_msg = std::make_shared<control_msgs::action::FollowJointTrajectory::Goal>();
  goal_msg->goal_time_tolerance.sec = 1;
  goal_msg->trajectory.joint_names = joint_names;
  goal_msg->trajectory.points = *points;

  if (!action_client_-> wait_for_action_server(10s)) { //verificar si el servidor de acción está disponible

    return;
  }

  auto send_goal_options = rclcpp_action::Client<control_msgs::action::FollowJointTrajectory>::SendGoalOptions(); //declararión varoal configuración de encío de la meta
  send_goal_options.result_callback = std::bind(common_result_response, std::placeholders::_1);
  send_goal_options.feedback_callback = std::bind(common_feedback, std::placeholders::_1,std::placeholders::_2); //declaración función de feedback


  if(pub_int_){
    RCLCPP_INFO(get_logger(), "PUBLICO EN LA ACCIÓN");
    auto send_goal_future = action_client_->async_send_goal(*goal_msg, send_goal_options); //envio de la meta
    printf("%1.6f %1.6f %1.6f %1.6f %1.6f %1.6f\n", q0,q1,q2,q3,q4,q5);
    printf("%i\n",pub_int_);
    pub_int_=0;
  }



}


int main (int argc,char * argv[]) //apañao
{
    rclcpp::init(argc,argv);

    auto node = std::make_shared<Sim_control>();

    rclcpp::Rate loop_rate(5);
   while (rclcpp::ok)
    {
        rclcpp::spin_some(node);
        node->send_goal();
        node->publish_method();
    }
    rclcpp::shutdown();
    return 0;
}

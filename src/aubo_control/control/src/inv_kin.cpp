
#include <control/inv_kin.hpp>
#include <control/configuracion.hpp>
#include <control/aubo_kin_mod.hpp>

using std::placeholders::_1;

Inv_kin::Inv_kin():Node ("inv_kin")
{
    pub_=this -> create_publisher<std_msgs::msg::Float32MultiArray>("Joint_goal",10);
    sub_= this -> create_subscription <std_msgs::msg::Float32MultiArray>("Cartesian_goal",10,std::bind(&Inv_kin::funcion,this, _1));
    double c_goal[6]={0.097,-0.128,0.959,0.0,0.0,0.0};
    double j_goal[6]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    int pub_ind=0;
    int est_codo=1;
}

Inv_kin::~Inv_kin() //Destructor
{
  printf("Leaving gently\n");
}

void Inv_kin::funcion(const std_msgs::msg::Float32MultiArray::SharedPtr msgrec) {
    //VARIABLES
 double T[16];
 double qi[6];
 double Ti[16];
 double q_sols[8*6];
 int num_sols;
 int opt_found=0;
 double x;
 double y;
 double z;
 double alpha;
 double beta;
 double gamma;

 //RECEPCION DE CARTESIAN_GOAL
 x=msgrec-> data[0];
 y=msgrec-> data[1];
 z=msgrec -> data[2];
 alpha=msgrec-> data[3];
 beta=msgrec-> data[4];
 gamma=msgrec-> data[5];

 //CALCULO DE T
 T[0]=cos(alpha)*cos(gamma)-sin(alpha)*cos(beta)*sin(gamma);
 T[1]=-cos(alpha)*sin(gamma)-sin(alpha)*cos(beta)*cos(gamma);
 T[2]=sin(alpha)*sin(beta);
 T[3]=x;
 T[4]=sin(alpha)*cos(gamma)+cos(alpha)*cos(beta)*sin(gamma);
 T[5]=-sin(alpha)*sin(gamma)+cos(alpha)*cos(beta)*cos(gamma);
 T[6]=-cos(alpha)*sin(beta);
 T[7]=y;
 T[8]=sin(beta)*sin(gamma);
 T[9]=sin(beta)*cos(gamma);
 T[10]=cos(beta);
 T[11]=z;
 T[12]=0;
 T[13]=0;
 T[14]=0;
 T[15]=1;

 //TRANSFORMADA INVERSA
 num_sols = inverse(T,q_sols);
 printf("%d\n\n",num_sols); //Se muestran el número de soluciones
 for(int i=0;i<num_sols;i++){
    printf("%1.6f %1.6f %1.6f %1.6f %1.6f %1.6f\n",q_sols[i*6+0], q_sols[i*6+1], q_sols[i*6+2],q_sols[i*6+3], q_sols[i*6+4], q_sols[i*6+5]);
    printf("\n");

}

 //ASIGNACION DE j_goal
 if(num_sols>=1){
    for(int i=0; i<num_sols &&! opt_found;i++){
        if(codo_valido(q_sols[6*i])==1 && codo_derecho(q_sols[6*i])==est_codo && cuerpo_alante(q_sols[6*i], q_sols[6*i+1]) && codo_arriba(q_sols[6*i], q_sols[6*i+2])==1 && cabeza_erguida(q_sols[6*i+3])==1 && cabeza_frente(q_sols[6*i+4])==1){
            opt_found=1;
            for(int j=0;j<=5;j++){
                j_goal[j]=q_sols[6*i+j];
            }
            pub_ind=1;
        }
    }
 }

 //SEGUNDA VUELTA j_goal
 if(num_sols>=1 && pub_ind==0){
    for(int i=0; i<num_sols &&!opt_found;i++){
        if(codo_valido(q_sols[6*i])==1 && cuerpo_alante(q_sols[6*i], q_sols[6*i+1]) && codo_arriba(q_sols[6*i], q_sols[6*i+2])==1 && cabeza_erguida(q_sols[6*i+3])==1 && cabeza_frente(q_sols[6*i+4])==1){
            opt_found=1;
            for(int j=0;j<=5;j++){
                j_goal[j]=q_sols[6*i+j];
            }
            pub_ind=1;
            est_codo=codo_derecho(q_sols[6*i]);
        }
    }
 }

}

void Inv_kin::publish_method()
{
   if (pub_ind==1){
			std_msgs::msg::Float32MultiArray msg;
            RCLCPP_ERROR(get_logger(), "SI QUE PUBLICO");

			msg.data.clear();

			msg.data.push_back(j_goal[0]);  // tita 1
			msg.data.push_back(j_goal[1]);
			msg.data.push_back(j_goal[2]);
			msg.data.push_back(j_goal[3]);
			msg.data.push_back(j_goal[4]);
			msg.data.push_back(j_goal[5]);


			pub_-> publish(msg); //Publish the message
			pub_ind=0;
	}
}

int main (int argc,char * argv[])
{
    rclcpp::init(argc,argv);

    auto node = std::make_shared<Inv_kin>();

    rclcpp::Rate loop_rate(5);
    while (rclcpp::ok)
    {
        rclcpp::spin_some(node);
        node->publish_method();
    }
    rclcpp::shutdown();
    return 0;
}

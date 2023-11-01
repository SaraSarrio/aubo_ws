
// En este archivo se incluyen las funciones usadas para analizarla configuración del robot

int codo_valido(double tita1){
 int codo_val=0;
 if (tita1<=1.57 && tita1>=-1.57){codo_val=1;}
 return codo_val;
}

int codo_derecho(double tita1){
 int codo_der=0;
 if (tita1<=0 && tita1>=-1.57){codo_der=1;}
 return codo_der;
}

int cuerpo_alante(double tita1,double tita2){
 int cuerpo_ala=0;
 if ((tita1<=0 && tita2<=0) || (tita1>0 && tita2>=0)){cuerpo_ala=1;}
 return cuerpo_ala;
}

int codo_arriba(double tita1,double tita3){
 int codo_arr=0;
 if ((tita1<=0 && tita3<=2.4 && tita3>=0) || (tita1>0 &&tita3>=-2.4 &&tita3<=0)){
    codo_arr=1;
 }
 return codo_arr;
}

int cabeza_erguida(double tita4){
 int cabeza_erg=0;
 if (tita4<=1.57 && tita4>=-1.57){cabeza_erg=1;}
 return cabeza_erg;
}

int cabeza_frente(double tita5){
 int cabeza_fren=0;
 if (tita5<=2.5 && tita5>=-2.5){cabeza_fren=1;}
 return cabeza_fren;
}

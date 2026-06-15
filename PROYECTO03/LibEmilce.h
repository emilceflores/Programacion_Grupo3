 
#include <iostream>
using namespace std;

void menuAdministrador(){

    int opcion;

    do{
        cout<<"=================================\n";
        cout<<"        MENU ADMINISTRADOR\n";
        cout<<"=================================\n";

        cout<<"1. Registrar usuario\n";
        cout<<"2. Registrar habitacion\n";
        cout<<"3. Modificar habitacion\n";
        cout<<"4. Dar de baja habitacion\n";
        cout<<"5. Reportes financieros\n";
        cout<<"0. Cerrar Sesion\n";

        cout<<"Seleccione una opcion: ";
        cin>>opcion;


        switch(opcion){

            case 1:
                cout<<"Registrar usuario\n";
                break;

            case 2:
                cout<<"Registrar habitacion\n";
                break;

            case 3:
                cout<<"Modificar habitacion\n";
                break;

            case 4:
                cout<<"Dar de baja\n";
                break;

            case 5:
                cout<<"Reportes\n";
                break;

            case 0:
                cout<<"Salir\n";
                break;

            default:
                cout<<"Opcion incorrecta\n";
        }

    }while(opcion!=0);
}
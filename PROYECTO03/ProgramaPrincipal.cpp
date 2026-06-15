#include <iostream>
#include "LibEstructuras.h"
#include "LibDiana.h"
#include "LibEmilce.h"
#include "LibJhoel.h"

using namespace std;

void mostrarmenu();
int main() 
{
    system("chcp 65001");
    mostrarmenu();

    return 0;
}

void mostrarmenu()
{
    int opcion=0 ;

    do {
        system("cls");

        cout << "================================="<<endl;
        cout << "   SISTEMA DE GESTION HOTELERA"<<endl;
        cout << "================================="<<endl;
        cout << "\t1. Administrador"<<endl;
        cout << "\t2. Recepcionista"<<endl;
        cout << "\t3. Consulta"<<endl;
        cout << "\t0. Salir"<<endl;
        cout << "================================="<<endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) 
        {
            case 1:
                menuAdministrador();
                system("pause");
                break;

            case 2:
                break;

            case 3:
               
                break;

            case 0:
                cout << "Saliendo del sistema."<<endl;
                break;

            default:
                if (opcion != 0)
                {
                    cout << "Opcion no valida." << endl;
                    system("pause");
                }
                break;
        }

    } while(opcion != 0);

    system("pause");
}
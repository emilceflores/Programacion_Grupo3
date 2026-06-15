#include <iostream>
#include <fstream>
#include <cstring>
#include "LibEstructuras.h"

#include "LibJhoel.h"
#include "LibDiana.h"
#include "LibEmilce.h"

using namespace std;

void Mostrarmenu();

int main() 
{
    system("chcp 65001"); 
    Mostrarmenu();

    return 0;
} 

void Mostrarmenu()
{
    int opcion = 0;

    do
    {
        system("cls");

        cout << "HOTEL CAESARS PALACE" << endl;
        cout << "====================" << endl;
        cout << "\t1. Ingresar como Personal del Hotel (Login)" << endl;
        cout << "\t2. Ingresar como Invitado / Cliente (Consultas)" << endl;
        cout << "\t0. Salir" << endl;
        cout << "Seleccione una opcion: "; 
        cin >> opcion;

        switch(opcion)
        {
            case 1:
                // Llama a la función de Login dentro de la librería de Jhoel
                menuLoginPersonal(); 
                system("pause");
                break;
            
            case 2:
                // Llama al menú de consultas dentro de la librería de Jhoel
                menuConsultaClientes(); 
                system("pause");
                break;
            
            case 0:
                cout << "Saliendo del sistema de Gestión Hotelera..." << endl;
                break;
            
            default:
                if (opcion != 0)
                {
                    cout << "Opcion no valida." << endl;
                    system("pause");
                }
                break;
        }

    } while (opcion != 0);

    system("cls");
}
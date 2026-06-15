#include <iostream>
#include "LibEstructuras.h"
#include "LibDiana.h"
#include "LibEmilce.h"
#include "LibJhoel.h"

using namespace std;

int main() {

    int opcion;

    do {
        cout << "=================================\n";
        cout << "   SISTEMA DE GESTION HOTELERA\n";
        cout << "=================================\n";
        cout << "1. Administrador\n";
        cout << "2. Recepcionista\n";
        cout << "3. Consulta\n";
        cout << "0. Salir\n";
        cout << "=================================\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {

            case 1:
                menuAdministrador();
                break;

            case 2:
            
                break;

            case 3:
               
                break;

            case 0:
                cout << "Hasta luego.\n";
                break;

            default:
                cout << "Opcion invalida.\n";
        }

    } while(opcion != 0);

    return 0;
}
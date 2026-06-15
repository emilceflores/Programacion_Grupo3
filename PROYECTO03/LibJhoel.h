#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void menuLoginPersonal();
void menuConsultaClientes();

// 1 FUNCION PARA EL LOGIN DEL PERSONAL
void menuLoginPersonal()
{
    char userIngresado[50];
    char passIngresado[50];
    bool encontrado = false;
    Usuario u; // Variable temporal para leer desde el archivo binario

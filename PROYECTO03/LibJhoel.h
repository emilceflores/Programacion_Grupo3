#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib> 
//#include "LibEstructuras.h"

//Declaramos las funciones de Diana y Emilce para poder llamarlas desde tu Login
void menuAdministrador(); // Viene de LibEmilce.h
void menuRecepcionista();  // Viene de LibDiana.h

using namespace std;

// Declaración de tus funciones (con los nombres exactos que pide el ProgramaPrincipal)
void menuLoginPersonal();
void menuConsultaClientes();
void verMapaHabitaciones();
void verCatalogoPrecios();
void buscarReservaPorCI();


// CONTROL DE ACCESO (LOGIN DE PERSONAL)
void menuLoginPersonal() {
    char usuarioInput[30];
    char passwordInput[30];
    Usuario u;
    bool ingreso = false;
    
    system("cls");
    cout << "=== CONTROL DE ACCESO (LOGIN) ===" << endl;
    cout << "=================================" << endl;
    cout << "Ingrese Usuario: ";
    cin >> usuarioInput;
    cout << "Ingrese Contrasena: ";
    cin >> passwordInput;
    
    ifstream archivoUser("USUARIOS.BIN", ios::binary);
    
    if (archivoUser.is_open()) {
        // Buscamos en el archivo si coinciden las credenciales
        while (archivoUser.read((char*)&u, sizeof(Usuario)) && !ingreso) {
            if (u.activo == true && strcmp(u.login, usuarioInput) == 0 && strcmp(u.password, passwordInput) == 0) {
                ingreso = true;
                cout << "\n[OK]: Acceso concedido. Bienvenido " << u.login << endl;
                system("pause");
                
                // Redirección automática según el rol asignado por Emilce
                if (u.rol == 1) {
                    menuAdministrador(); // Abre el menú de Emilce
                } else if (u.rol == 2) {
                    menuRecepcionista(); // Abre el menú de Diana
                }
            }
        }
        archivoUser.close();
        
        if (!ingreso) {
            cout << "\n[ERROR]: Usuario o contrasena incorrectos, o usuario inactivo." << endl;
            system("pause");
        }
    } else {
        cout << "\n[AVISO]: No se pudo abrir USUARIOS.BIN. Registre un administrador primero." << endl;
        system("pause");
    }
}


// SUBMENÚ DE CONSULTA (ACCESO LIBRE PARA CLIENTES)
void menuConsultaClientes() {
    int opcConsulta = 0;
    
    do {
        system("cls");
        cout << "=== SUBMENU DE CONSULTA PARA CLIENTES ===" << endl;
        cout << "========================================" << endl;
        cout << "\t1. Ver Mapa de Habitaciones en Tiempo Real" << endl;
        cout << "\t2. Ver Catalogo de Productos y Precios" << endl;
        cout << "\t3. Buscar Estado de Reserva por CI" << endl;
        cout << "\t0. Volver al Menu Principal" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcConsulta;
        
        switch(opcConsulta) {
            case 1:
                verMapaHabitaciones();
                system("pause");
                break;
                
            case 2:
                verCatalogoPrecios();
                system("pause");
                break;
                
            case 3:
                buscarReservaPorCI();
                system("pause");
                break;
                
            case 0:
                cout << "Regresando al menu principal..." << endl;
                break;
                
            default:
                if (opcConsulta != 0) {
                    cout << "Opcion no valida." << endl;
                    system("pause");
                }
                break;
        }
    } while(opcConsulta != 0);
}

// MAPA DE HABITACIONES
void verMapaHabitaciones() {
    Habitacion h;
    ifstream archivo("HABITACIONES.BIN", ios::binary);
    
    system("cls");
    cout << "=== MAPA DE HABITACIONES EN TIEMPO REAL ===" << endl;
    cout << "===========================================" << endl;
    cout << "Numero\tTipo\t\tEstado" << endl;
    cout << "-------------------------------------------" << endl;
    
    if (archivo.is_open()) {
        bool tieneDatos = false;
        while (archivo.read((char*)&h, sizeof(Habitacion))) {
            if (h.activo == true) {
                tieneDatos = true;
                cout << h.numero << "\t" << h.tipo << "\t\t";
                if (h.estado == 0) {
                    cout << "[LIBRE]" << endl;
                } else {
                    cout << "[OCUPADA]" << endl;
                }
            }
        }
        archivo.close();
        
        if (!tieneDatos) {
            cout << "[AVISO]: No hay habitaciones registradas en el sistema." << endl;
        }
    } else {
        cout << "[ERROR]: No se pudo abrir el archivo de habitaciones." << endl;
    }
}

// CATÁLOGO DE PRECIOS
void verCatalogoPrecios() {
    Producto p;
    ifstream archivo("INVENTARIO.BIN", ios::binary);
    
    system("cls");
    cout << "=== CATALOGO DE PRODUCTOS Y PRECIOS ===" << endl;
    cout << "=======================================" << endl;
    cout << "ID\tNombre\t\tPrecio\tStock" << endl;
    cout << "---------------------------------------" << endl;
    
    if (archivo.is_open()) {
        bool tieneProductos = false;
        while (archivo.read((char*)&p, sizeof(Producto))) {
            if (p.activo == true) {
                tieneProductos = true;
                // Usamos p.nombreProd y p.precioVenta exactamente como en tu struct
                cout << p.idProducto << "\t" << p.nombreProd << "\t\t" << p.precioVenta << " Bs.\t" << p.stock << endl;
            }
        }
        archivo.close();
        
        if (!tieneProductos) {
            cout << "[AVISO]: No hay productos registrados en el catalogo." << endl;
        }
    } else {
        cout << "[ERROR]: No se pudo abrir el archivo de inventario." << endl;
    }
}

// BUSCAR RESERVA POR CI
void buscarReservaPorCI() {
    int ciBuscar; // Tipo int de acuerdo al struct Reserva de su grupo
    Reserva r;
    bool encontro = false;
    
    system("cls");
    cout << "=== BUSCAR ESTADO DE RESERVA POR CI ===" << endl;
    cout << "=======================================" << endl;
    cout << "Ingrese el CI del cliente: ";
    cin >> ciBuscar;
    
    ifstream archivoRes("RESERVAS.BIN", ios::binary);
    
    if (archivoRes.is_open()) {
        while (archivoRes.read((char*)&r, sizeof(Reserva))) {
            if (r.ciHuesped == ciBuscar && r.activo == true) {
                cout << "\n-> ¡RESERVA ENCONTRADA!" << endl;
                cout << "---------------------------------------" << endl;
                cout << "Codigo Reserva: " << r.idReserva << endl;
                cout << "Habitacion asignada: " << r.numHabitacion << endl;
                cout << "Fecha de ingreso: " << r.fechaIngreso.dia << "/" << r.fechaIngreso.mes << "/" << r.fechaIngreso.anio << endl;
                cout << "Dias de estadia: " << r.diasEstadia << endl;
                cout << "---------------------------------------" << endl;
                encontro = true;
            }
        }
        archivoRes.close();
        
        if (!encontro) {
            cout << "\n[Aviso]: No se encontro ninguna reserva activa para el CI: " << ciBuscar << endl;
        }
    } else {
        cout << "[ERROR]: No se pudo abrir el archivo de reservas." << endl;
    }
}


// Materia: Programacion I, Paralelo 4
// Grupo: 3.
// Autor: Jhoel Marco Machicado Flores.
// Nombre del proyecto : Sistema de gestion hotelera,control de inventarios y facturacionintegrada "ceasars palace"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib> 

void menuAdministrador(); 
void menuRecepcionista();  

using namespace std;

void menuLoginPersonal();
void menuConsultaClientes();
void verMapaHabitaciones();
void verCatalogoPrecios();
void buscarReservaPorCI();

// CONTROL DE ACCESO 
void menuLoginPersonal() 
{
    char usuarioInput[30];
    char passwordInput[30];
    Usuario u;
    bool ingreso = false;
    
    ifstream verificarArchivo("USUARIOS.BIN", ios::binary);
    if (!verificarArchivo) 
    {
        ofstream archivoNuevo("USUARIOS.BIN", ios::binary);
        if (archivoNuevo) 
        {
            Usuario adminPorDefecto;
            strcpy(adminPorDefecto.login, "admin");
            strcpy(adminPorDefecto.password, "123");
            adminPorDefecto.rol = 1; // 1 = Administrador
            adminPorDefecto.activo = true;
            
            archivoNuevo.write((char*)&adminPorDefecto, sizeof(Usuario));
            archivoNuevo.close();
            
            system("cls"); // OPORTUNO: Limpia para mostrar el aviso de creación limpia
            cout << "=========================================================" << endl;
            cout << "[SISTEMA]: Archivo vacio. Cuenta de Administrador activa." << endl;
            cout << "USUARIO: admin   |   CONTRASENA: 123" << endl;
            cout << "Nota: Las recepcionistas requieren registro del administrador." << endl;
            cout << "=========================================================" << endl;
            system("pause");
        }
    } 
    else 
    {
        verificarArchivo.close();
    }

    // FLUJO NORMAL DEL LOGIN
    system("cls");
    cout << "=== CONTROL DE ACCESO  ===" << endl;
    cout << "=================================" << endl;
    cout << "Ingrese Usuario: ";
    cin >> usuarioInput;
    cout << "Ingrese Contrasena: ";
    cin >> passwordInput;
    
    ifstream archivoUser("USUARIOS.BIN", ios::binary);
    
    if (archivoUser)
    {
        while (archivoUser.read((char*)&u, sizeof(Usuario)) && !ingreso) {
            if (u.activo == true && strcmp(u.login, usuarioInput) == 0 && strcmp(u.password, passwordInput) == 0) 
            {
                ingreso = true;
                system("cls"); // OPORTUNO: Limpia los datos tecleados y da una bienvenida limpia
                cout << "\n[OK]: Acceso concedido. Bienvenido " << u.login << endl;
                system("pause");
                
                if (u.rol == 1) 
                {
                    menuAdministrador(); 
                } else if (u.rol == 2) 
                {
                    menuRecepcionista(); 
                }
            }
        }
        archivoUser.close();
        
        if (!ingreso) 
        {
            system("cls"); // OPORTUNO: Limpia la pantalla para que el error resalte inmediatamente
            cout << "=== ERROR DE AUTENTICACION ===" << endl;
            cout << "==============================" << endl;
            cout << "\n[ERROR]: Usuario o contrasena incorrectos." << endl;
            cout << "[AVISO]: Si es personal nuevo (Recepcionista), requiere registro previo del Administrador." << endl;
            system("pause");
        }
    } 
    else 
    {
        system("cls"); // OPORTUNO: Muestra el error de archivo de forma aislada
        cout << "\n[ERROR]: No se pudo abrir el archivo de usuarios." << endl;
        system("pause");
    }
}

// SUBMENÚ DE CONSULTA 
void menuConsultaClientes() 
{
    int opcConsulta = 0;
    do 
    {
        system("cls");
        cout << "=== SUBMENU DE CONSULTA PARA CLIENTES ===" << endl;
        cout << "========================================" << endl;
        cout << "\t1. Ver Mapa de Habitaciones en Tiempo Real" << endl;
        cout << "\t2. Ver Catalogo de Productos y Precios" << endl;
        cout << "\t3. Buscar Estado de Reserva por CI" << endl;
        cout << "\t0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcConsulta;
        
        switch(opcConsulta) 
        {
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
                system("cls"); // OPORTUNO: Limpia antes de avisar que vuelve atrás
                cout << "Regresando al menu principal..." << endl;
                break;
                
            default:
                if (opcConsulta != 0) 
                {
                    system("cls");
                    cout << "Opcion no valida." << endl;
                    system("pause");
                }
                break;
        }
    } 
    while(opcConsulta != 0);
}

// MAPA DE HABITACIONES
void verMapaHabitaciones() 
{
    Habitacion h;
    ifstream archivo("HABITACIONES.BIN", ios::binary);
    
    system("cls");
    cout << "=== MAPA DE HABITACIONES ===" << endl;
    cout << "===========================================" << endl;
    cout << "Numero\tTipo\t\tEstado" << endl;
    cout << "-------------------------------------------" << endl;

    if (archivo) 
    {
        bool tieneDatos = false;
        while (archivo.read((char*)&h, sizeof(Habitacion))) 
        {
            if (h.activo == true) 
            {
                tieneDatos = true;
                cout << h.numero << "\t" << h.tipo << "\t\t";
                if (h.estado == 0) {
                    cout << "[LIBRE]" << endl;
                } 
                else 
                {
                    cout << "[OCUPADA]" << endl;
                }
            }
        }
        archivo.close();
        
        if (!tieneDatos) 
        {
            cout << "[AVISO]: No hay habitaciones registradas en el sistema." << endl;
        }
    } 
    else 
    {
        cout << "[ERROR]: No se pudo abrir el archivo de habitaciones." << endl;
    }
}

// CATÁLOGO DE PRECIOS
void verCatalogoPrecios() 
{
    Producto p;
    ifstream archivo("PRODUCTOS.BIN", ios::binary);
    
    system("cls");
    cout << "=== CATALOGO DE PRODUCTOS Y PRECIOS ===" << endl;
    cout << "=======================================" << endl;
    cout << "ID\tNombre\t\tPrecio\tStock" << endl;
    cout << "---------------------------------------" << endl;
    
    if (archivo) 
    {
        bool tieneProductos = false;
        while (archivo.read((char*)&p, sizeof(Producto))) 
        {
            if (p.activo == true) 
            {
                tieneProductos = true;
                cout << p.idProducto << "\t" << p.nombreProd << "\t\t" << p.precioVenta << " Bs.\t" << p.stock << endl;
            }
        }
        archivo.close();
        
        if (!tieneProductos) 
        {
            cout << "[AVISO]: No hay productos registrados en el catalogo." << endl;
        }
    } 
    else 
    {
        cout << "[ERROR]: No se pudo abrir el archivo de inventario." << endl;
    }
}

// BUSCAR RESERVA POR CI
void buscarReservaPorCI() 
{
    int ciBuscar; 
    Reserva r;
    bool encontro = false;
    
    system("cls");
    cout << "=== BUSCAR ESTADO DE RESERVA POR CI ===" << endl;
    cout << "=======================================" << endl;
    cout << "Ingrese el CI del cliente: ";
    cin >> ciBuscar;
    
    ifstream archivoRes("RESERVAS.BIN", ios::binary);
    
    if (archivoRes) 
    {
        // OPORTUNO: Limpiamos la pantalla justo después de que el usuario meta el CI, 
        // así el resultado de la búsqueda se ve perfectamente ordenado arriba.
        system("cls"); 
        
        while (archivoRes.read((char*)&r, sizeof(Reserva))) 
        {
            if (r.ciHuesped == ciBuscar && r.activo == true)
            {
                cout << "=== RESERVA ENCONTRADA ===" << endl;
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
        
        if (!encontro) 
        {
            cout << "=== BUSQUEDA FINALIZADA ===" << endl;
            cout << "\n[Aviso]: No se encontro ninguna reserva activa para el CI: " << ciBuscar << endl;
        }
    } 
    else 
    {
        system("cls");
        cout << "[ERROR]: No se pudo abrir el archivo de reservas." << endl;
    }
}

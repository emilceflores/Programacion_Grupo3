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
            
            system("cls"); 
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
                system("cls"); 
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
            system("cls"); 
            cout << "=== ERROR DE AUTENTICACION ===" << endl;
            cout << "==============================" << endl;
            cout << "\n[ERROR]: Usuario o contrasena incorrectos." << endl;
            cout << "[AVISO]: Si es personal nuevo (Recepcionista), requiere registro previo del Administrador." << endl;
            system("pause");
        }
    } 
    else 
    {
        system("cls"); 
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
                system("cls"); 
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

// MAPA DE HABITACIONES (ACTUALIZADO CON "EJECUTIVA / MATRIMONIAL")
void verMapaHabitaciones() 
{
    Habitacion h;
    ifstream archivo("HABITACIONES.BIN", ios::binary);
    
    // Matriz de 11 filas (pisos 1 al 10) y 9 columnas (habitaciones 1 al 8)
    int hotelMatriz[11][9];
    
    // Inicializamos toda la matriz en -1 (espacio vacío)
    for (int f = 1; f <= 10; f++) {
        for (int c = 1; c <= 8; c++) {
            hotelMatriz[f][c] = -1;
        }
    }
    
    int totalesLibres = 0;
    int totalesOcupadas = 0;

    system("cls");
    cout << "=========================================================================================================" << endl;
    cout << "                              CEASARS PALACE HOTEL - MAPA DE OCUPACION                                   " << endl;
    cout << "=========================================================================================================" << endl;

    if (archivo) 
    {
        while (archivo.read((char*)&h, sizeof(Habitacion))) 
        {
            if (h.activo == true) 
            {
                int piso = h.numero / 100;
                int cuarto = h.numero % 100;

                if (piso >= 1 && piso <= 10 && cuarto >= 1 && cuarto <= 8) 
                {
                    hotelMatriz[piso][cuarto] = h.estado;
                }
            }
        }
        archivo.close();
        
        cout << "PISO\t| H1\t| H2\t| H3\t| H4\t| H5\t| H6\t| H7\t| H8\t| CATEGORIA\t\t| PRECIO x NOCHE" << endl;
        cout << "---------------------------------------------------------------------------------------------------------" << endl;
        
        for (int f = 10; f >= 1; f--) 
        {
            int libresPiso = 0;
            int ocupadasPiso = 0;
            
            cout << "Piso " << f << "\t| ";
            for (int c = 1; c <= 8; c++) 
            {
                if (hotelMatriz[f][c] == -1) 
                {
                    cout << " - \t| "; 
                }
                else if (hotelMatriz[f][c] == 0) 
                {
                    cout << "[L]\t| "; 
                    libresPiso++;
                    totalesLibres++;
                }
                else if (hotelMatriz[f][c] == 1) 
                {
                    cout << "[O]\t| "; 
                    ocupadasPiso++;
                    totalesOcupadas++;
                }
            }
            
            // Colocamos las etiquetas informativas actualizadas tal como en el informe
            if (f == 10) {
                cout << " Suite Presidencial\t| 700.00 Bs.";
            }
            else if (f >= 7 && f <= 9) {
                cout << " Suite VIP\t\t| 400.00 Bs.";
            }
            else if (f >= 4 && f <= 6) {
                cout << " Ejecutiva / Matrimonial| 200.00 - 250.00 Bs.";
            }
            else if (f >= 1 && f <= 3) {
                cout << " Simple / Economica\t| 90.00 - 120.00 Bs.";
            }
            
            cout << " (L: " << libresPiso << " / O: " << ocupadasPiso << ")" << endl;
        }
        
        cout << "---------------------------------------------------------------------------------------------------------" << endl;
        cout << "MONITOR -> TOTAL LIBRES: " << totalesLibres << " | TOTAL OCUPADAS: " << totalesOcupadas << endl;
        cout << "Leyenda: [L] = Libre | [O] = Ocupada |  -  = Espacio inexistente (Habitacion mas grande)" << endl;
        cout << "=========================================================================================================" << endl;
    } 
    else 
    {
        cout << "[ERROR]: No se pudo abrir o leer el archivo HABITACIONES.BIN" << endl;
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
}                {
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
        cout << "=== CONSULTA PARA CLIENTES ===" << endl;
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

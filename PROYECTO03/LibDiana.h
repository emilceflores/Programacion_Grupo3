#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void menuRecepcionista();
void registrarHuespedCheckIn();
void crearReservaDirecta();
void buscarReservaActivaCI();  
void registrarVentaProducto();  
void calcularPagoCheckOut();
void verHabitacionesDisponibles();
//----------------------------------
void menuRecepcionista()
{
    int opcRecepcion = 0;

    do
    {
        system("cls");
        cout << "=== SUBMENU DE RECEPCION ===" << endl;
        cout << "====================================" << endl;
        cout << "\t1. Registrar Huesped (Check-In Presencial)" << endl;
        cout << "\t2. Crear Reserva Directa" << endl;
        cout << "\t3. Buscar Reserva Activa por CI" << endl;
        cout << "\t4. Registrar Venta de Producto (Consumo Extra)" << endl;
        cout << "\t5. Calcular Pago y Check-Out (Facturacion Integrada)" << endl;
        cout << "\t6. Ver Listado de Habitaciones Disponibles" << endl;
        cout << "\t0. Cerrar Sesion de Mostrador" << endl;
        cout << "------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcRecepcion;

  switch(opcRecepcion)
        {
            case 1:
                registrarHuespedCheckIn();
                system("pause");
                break;

            case 2:
                crearReservaDirecta();
                system("pause");
                break;

            case 3:
                buscarReservaActivaCI();
                system("pause");
                break;

            case 4:
                registrarVentaProducto();
                system("pause");
                break;

            case 5:
                calcularPagoCheckOut();
                system("pause");
                break;

            case 6:
                verHabitacionesDisponibles();
                system("pause");
                break;

            case 0:
                cout << "Finalizando funciones del modulo operativo..." << endl;
                break;

            default:
                if (opcRecepcion != 0)
                {
                    cout << "Opcion no valida." << endl;
                    system("pause");
                }
                break;
        }

    } while(opcRecepcion != 0);
}
//OPCION 6
//------------------------------------------------------------------------------------------
void verHabitacionesDisponibles()
{
    Habitacion h;
    ifstream archivo("HABITACIONES.BIN", ios::binary);
    
    system("cls");
    cout << "=== LISTADO DE HABITACIONES DISPONIBLES ===" << endl;
    cout << "=================================================" << endl;
    cout << "Numero\tTipo\t\tPrecio/Noche\tEstado" << endl;
    cout << "-------------------------------------------------" << endl;
    
    if (archivo.is_open())
    {
        bool encontroAlguna = false;
        while (archivo.read((char*)&h, sizeof(Habitacion)))
        {
            // Solo mostramos si la habitaciOn esta activa y desocupada (estado 0)
            if (h.activo == true && h.estado == 0)
            {
                cout << h.numero << "\t" << h.tipo << "\t\t" << h.precioNoche << " Bs.\t[DISPONIBLE]" << endl;
                encontroAlguna = true;
            }
        }
        archivo.close();
        
        if (!encontroAlguna)
        {
            cout << "[AVISO]: No hay ninguna habitacion libre en este momento." << endl;
        }
    }
}
// OPCION 2
//-----------------------------------------------------------------------------------------
void crearReservaDirecta()
{
    Reserva nuevaR;
    int maxId = 0;
    Reserva aux;
    
    int habIngresada;
    bool habValida = false;
    Habitacion h;

    system("cls");
    cout << "=== OP2: CREAR RESERVA DIRECTA ===" << endl;
    cout << "==================================" << endl;

    // busqueda secuencial del ultimo ID registrado
    ifstream archivoLectura;
    archivoLectura.open("RESERVAS.BIN", ios::binary);
    
    if (archivoLectura.good())
    {
        while (archivoLectura.read((char*)&aux, sizeof(Reserva)))
        {
            if (aux.idReserva > maxId)
            {
                maxId = aux.idReserva;
            }
        }
        archivoLectura.close();
    }
    
    nuevaR.idReserva = maxId + 1; 
    cout << "Codigo de Reserva Asignado: " << nuevaR.idReserva << endl;
    cout << "-----------------------------------------------" << endl;

    cout << "Ingrese el CI del Huesped: ";
    cin >> nuevaR.ciHuesped;
    cout << endl;
    
    // Llamada  para mostrar los cuartos libres
    verHabitacionesDisponibles(); 

    do
    {
        cout << "Seleccione el Numero de Habitacion para la reserva: ";
        cin >> habIngresada;

        ifstream archivoHab;
        archivoHab.open("HABITACIONES.BIN", ios::binary);
        
        if (archivoHab.good())
        {
            bool existe = false;
            bool libre = false;

            while (archivoHab.read((char*)&h, sizeof(Habitacion)))
            {
                if (h.numero == habIngresada && h.activo == true)
                {
                    existe = true;
                    if (h.estado == 0) 
                    {
                        libre = true;
                    }
                }
            }
            archivoHab.close();

            if (!existe)
            {
                cout << "[ERROR]: El numero ingresado no existe. Intente de nuevo." << endl;
            }
            else if (!libre)
            {
                cout << "[ERROR]: Esa habitacion no esta libre. Seleccione otra de la lista." << endl;
            }
            else
            {
                nuevaR.numHabitacion = habIngresada;
                habValida = true; // el dato es correcto, rompe el bucle
                cout << "[OK]: Habitacion seleccionada con exito." << endl;
            }
        }
        /*else
        {
            //  pruebas sin archivos iniciales
            nuevaR.numHabitacion = habIngresada;
            habValida = true;
        }*/

    } while (!habValida);

    cout << endl << "--- Fecha de Ingreso ---" << endl;
    cout << "\tDia (1-31): "; cin >> nuevaR.fechaIngreso.dia;
    cout << "\tMes (1-12): "; cin >> nuevaR.fechaIngreso.mes;
    cout << "\tAño (Ej. 2026): "; cin >> nuevaR.fechaIngreso.anio;

    cout << "Ingrese la cantidad de dias de estadia: ";
    cin >> nuevaR.diasEstadia;

    nuevaR.activo = true;

    // Guardado en RESERVAS.BIN ,añadir
    ofstream archivoEscritura;
    archivoEscritura.open("RESERVAS.BIN", ios::binary | ios::app);
    
    if (archivoEscritura.good())
    {
        archivoEscritura.write((char*)&nuevaR, sizeof(Reserva));
        archivoEscritura.close();
        cout << endl << "[OK]: Reserva guardada con exito en RESERVAS.BIN" << endl;
    }
    else
    {
        cout << endl << "[ERROR]: No se pudo abrir el archivo para escribir." << endl;
    }
}
//OPCION 1
//-------------------------------------------------------------------------------------

void registrarHuespedCheckIn()
{
    int habIngresada;
    int diaHoy, mesHoy, anioHoy;
    bool habitacionComprometida = false;
    bool habitacionExisteYLibre = false;
    
    Reserva r;
    Habitacion h;
    Huesped nuevoH; 

    system("cls");
    cout << "=== OP1: REGISTRAR HUÉSPED (CHECK-IN PRESENCIAL) ===" << endl;
    cout << "====================================================" << endl;

    // 1. Captura de la fecha actual de control (Meses 1-12, Días 1-30)
    cout << "Ingrese la Fecha de Hoy (Control de Agenda):" << endl;
    cout << "\tDia (1-30): "; cin >> diaHoy;
    cout << "\tMes (1-12): "; cin >> mesHoy;
    cout << "\tAño: "; cin >> anioHoy;

    cout << "\nIngrese el Numero de Habitacion que el huesped va a ocupar: ";
    cin >> habIngresada;

    ifstream archivoRes;
    archivoRes.open("RESERVAS.BIN", ios::binary);

    if (archivoRes.good())
    {
        while (archivoRes.read((char*)&r, sizeof(Reserva)) && !habitacionComprometida)
        {
            if (r.numHabitacion == habIngresada && r.activo == true)
            {
                if (r.fechaIngreso.mes == mesHoy && r.fechaIngreso.anio == anioHoy)
                {
                    int diaFinReserva = r.fechaIngreso.dia + r.diasEstadia;
                    
                    if (diaHoy >= r.fechaIngreso.dia && diaHoy < diaFinReserva)
                    {
                        habitacionComprometida = true; 
                    }
                }
            }
        }
        archivoRes.close();
    }

    // 3. PASO B: VERIFICAR INFRAESTRUCTURA Y MODIFICAR ESTADO EN "HABITACIONES.BIN"
    if (!habitacionComprometida)
    {
        fstream archivoHab;
        archivoHab.open("HABITACIONES.BIN", ios::in | ios::out | ios::binary);

        if (archivoHab.good())
        {
            while (archivoHab.read((char*)&h, sizeof(Habitacion)) && !habitacionExisteYLibre)
            {
                if (h.numero == habIngresada && h.activo == true)
                {
                    if (h.estado == 0) // esta libre de verdad
                    {
                        habitacionExisteYLibre = true;
                        h.estado = 1; // cambiamos el estado a Ocupada (1)

                        // RETROCEDEMOS 
                        archivoHab.seekp(-sizeof(Habitacion), ios::cur);
                        archivoHab.write((char*)&h, sizeof(Habitacion));
                    }
                }
            }
            archivoHab.close();
        }
    }
    
    if (habitacionComprometida)
    {
        cout << "\n[ERROR]: La habitacion No. " << habIngresada << " no se puede asignar." << endl;
        cout << "Motivo: Cuenta con una RESERVA VIGENTE para el dia de hoy." << endl;
    }
    else if (!habitacionExisteYLibre)
    {
        cout << "\n[ERROR]: No se pudo realizar el Check-In." << endl;
        cout << "Motivo: La habitacion no existe, no esta activa o YA SE ENCUENTRA OCUPADA." << endl;
    }
    else
    {
        cout << "\n--- FICHA DE REGISTRO DEL HUÉSPED ---" << endl;
        cout << "Ingrese el CI del Huesped: ";
        cin >> nuevoH.ci; 
        
        cin.ignore(); // Limpia el buffer
        cout << "Ingrese el Nombre Completo (Max 30 caracteres): ";
        cin.getline(nuevoH.nombre, 30);
        
        cout << "Ingrese la Procedencia (pais:) ";
        cin.getline(nuevoH.procedencia, 30); 
        
        cout << "Ingrese el Numero de Celular: ";
        cin >> nuevoH.celular;

        nuevoH.numHabitacion = habIngresada; 

        ofstream archivoHuesped;
        archivoHuesped.open("HUESPEDES.BIN", ios::binary | ios::app);
        
        if (archivoHuesped.good())
        {
            archivoHuesped.write((char*)&nuevoH, sizeof(Huesped));
            archivoHuesped.close();
        }

        cout << "\n====================================================" << endl;
        cout << "[OK]: Check-In y Datos del Huesped guardados con exito." << endl;
        cout << "La habitacion No. " << habIngresada << " paso a estado: [OCUPADA]." << endl;
        cout << "====================================================" << endl;
    }
}
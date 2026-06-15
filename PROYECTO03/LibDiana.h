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
        cout << "=== SUBMENU DE RECEPCION (DIANA) ===" << endl;
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

    // Generación automática del ID leyendo RESERVAS.BIN
    ifstream archivoLectura("RESERVAS.BIN", ios::binary);
    if (archivoLectura.is_open())
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
    
    verHabitacionesDisponibles(); 

    // habitacion seleccionada
    do
    {
        cout << "Seleccione el Numero de Habitacion para la reserva: ";
        cin >> habIngresada;

        ifstream archivoHab("HABITACIONES.BIN", ios::binary);
        if (archivoHab.is_open())
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
                habValida = true; 
                cout << "Habitacion seleccionada con exito." << endl;
            }
        }

    } while (!habValida);

    cout << endl << "--- Fecha de Ingreso ---" << endl;
    cout << "\tDia (1-31): "; cin >> nuevaR.fechaIngreso.dia;
    cout << "\tMes (1-12): "; cin >> nuevaR.fechaIngreso.mes;
    cout << "\tAño (Ej. 2026): "; cin >> nuevaR.fechaIngreso.anio;

    cout << "Ingrese la cantidad de dias de estadia: ";
    cin >> nuevaR.diasEstadia;

    nuevaR.activo = true; 

    // Guardado  en el archivo binario
    ofstream archivoEscritura("RESERVAS.BIN", ios::binary | ios::app);
    if (archivoEscritura.is_open())
    {
        archivoEscritura.write((char*)&nuevaR, sizeof(Reserva));
        archivoEscritura.close();
        cout << endl << "Reserva guardada con exito en RESERVAS.BIN" << endl;
    }
    else
    {
        cout << endl << "[ERROR]: No se pudo abrir RESERVAS.BIN" << endl;
    }
}
// Materia: Programacion I, Paralelo 4
// Grupo: 3.
// Autor: Diana Ninoska Vasquez Benitez
// Nombre del proyecto : Sistema de gestion hotelera,control de inventarios y facturacionintegrada "ceasars palace"
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
        
        cin.ignore();
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
//opcion 3
//---------------------------------------------------------------------------------------------

void buscarReservaActivaCI()
{
    int ciBuscar;
    Reserva r;
    bool encontroAlMenosUna = false; //para saber si el cliente tiene registros

    system("cls");
    cout << "=== OP3: BUSCAR RESERVA ACTIVA POR CI ===" << endl;
    cout << "=========================================" << endl;

    cout << "Ingrese el CI del Huesped a buscar: ";
    cin >> ciBuscar;

    ifstream archivoRes;
    archivoRes.open("RESERVAS.BIN", ios::binary);

    if (archivoRes.good())
    {
        cout << "\n-------------------------------------------------" << endl;
        cout << "Resultados de Busqueda para el CI: " << ciBuscar << endl;
        cout << "-------------------------------------------------" << endl;

        while (archivoRes.read((char*)&r, sizeof(Reserva)))
        {
            if (r.ciHuesped == ciBuscar && r.activo == true)
            {
                encontroAlMenosUna = true; // si hay registro

                cout << "-> ID Reserva: " << r.idReserva << endl;
                cout << "   Habitacion Asignada: " << r.numHabitacion << endl;
                cout << "   Fecha de Ingreso: " << r.fechaIngreso.dia << "/" 
                     << r.fechaIngreso.mes << "/" << r.fechaIngreso.anio << endl;
                cout << "   Dias de Estadia: " << r.diasEstadia << endl;
                cout << "-------------------------------------------------" << endl;
            }
        }
        archivoRes.close();

        if (!encontroAlMenosUna)
        {
            cout << "[AVISO]: No se encontraron reservas activas para el CI: " << ciBuscar << endl;
            cout << "Procediendo a mostrar las opciones libres en el hotel...\n" << endl;
            system("pause");
            
            verHabitacionesDisponibles(); 
        }
    }
    else
    {
        cout << "\n[AVISO]: El archivo de reservas aun no existe en el sistema." << endl;
        cout << "(Debe crear una reserva en la Opcion 2 o registrar un huesped en la Opcion 1)." << endl;
        cout << "\nMostrando habitaciones del hotel para registro directo...\n" << endl;
        system("pause");
        
        // si el archivo ni existe tambien  mostramos qu cuartos hay libres
        verHabitacionesDisponibles();
    }
}
//opcion 4
//----------------------------------------------------------------------------------------------
void registrarVentaProducto()
{
    int habBuscar, prodBuscar, cantVenta;
    int maxIdConsumo = 0;
    
    bool habitacionOcupada = false;
    bool productoExisteYStock = false;
    
    Habitacion h;
    Producto p;
    ConsumoExtra nuevoC;
    ConsumoExtra auxC;

    system("cls");
    cout << "=== OP4: REGISTRAR VENTA DE PRODUCTO (CONSUMO EXTRA) ===" << endl;
    cout << "========================================================" << endl;

    // GENERAR ID CORRELATIVO AUTOMATICO PARA EL CONSUMO
    ifstream archivoIdC;
    archivoIdC.open("CONSUMOS.BIN", ios::binary);
    if (archivoIdC.good())
    {
        while (archivoIdC.read((char*)&auxC, sizeof(ConsumoExtra)))
        {
            if (auxC.idConsumo > maxIdConsumo)
            {
                maxIdConsumo = auxC.idConsumo;
            }
        }
        archivoIdC.close();
    }
    nuevoC.idConsumo = maxIdConsumo + 1;

    // VALIDAR QUE LA HABITACION EXISTA Y ESTE OCUPADA
    cout << "Ingrese el Numero de Habitacion del huesped: ";
    cin >> habBuscar;

    ifstream archivoHab;
    archivoHab.open("HABITACIONES.BIN", ios::binary);
    if (archivoHab.good())
    {
        while (archivoHab.read((char*)&h, sizeof(Habitacion)) && !habitacionOcupada)
        {
            if (h.numero == habBuscar && h.activo == true && h.estado == 1)
            {
                habitacionOcupada = true; // confirmado ,el cuarto esta ocupado y activo
            }
        }
        archivoHab.close();
    }

    //SI LA HABITACION ES VALIDA, PROCEDEMOS CON EL PRODUCTO e INVENTARIO
    if (habitacionOcupada)
    {
        cout << "Ingrese el ID del Producto a vender: ";
        cin >> prodBuscar;
        cout << "Ingrese la Cantidad: ";
        cin >> cantVenta;

        //abrimos PRODUCTOS.BIN en modo lectura/escritura para actualizar el stock
        fstream archivoProd;
        archivoProd.open("PRODUCTOS.BIN", ios::in | ios::out | ios::binary);

        if (archivoProd.good())
        {
            while (archivoProd.read((char*)&p, sizeof(Producto)) && !productoExisteYStock)
            {
                if (p.idProducto == prodBuscar && p.activo == true)
                {
                    if (p.stock >= cantVenta)
                    {
                        productoExisteYStock = true; // Hay stock suficiente

                        // Restamos el stock
                        p.stock = p.stock - cantVenta;

                        // modificamos
                        archivoProd.seekp(-sizeof(Producto), ios::cur);
                        archivoProd.write((char*)&p, sizeof(Producto));
                    }
                }
            }
            archivoProd.close();
        }

        //SI EL PRODUCTO FUE VALIDADADO
        if (productoExisteYStock)
        {
            nuevoC.numHabitacion = habBuscar;
            nuevoC.idProducto = prodBuscar;
            nuevoC.cantidad = cantVenta;
            nuevoC.activo = true; 

            ofstream archivoConsumo;
            archivoConsumo.open("CONSUMOS.BIN", ios::binary | ios::app);
            if (archivoConsumo.good())
            {
                archivoConsumo.write((char*)&nuevoC, sizeof(ConsumoExtra));
                archivoConsumo.close();
            }

            cout << "\n====================================================" << endl;
            cout << "[OK]: Venta registrada con exito (ID Consumo: " << nuevoC.idConsumo << ")." << endl;
            cout << "Cargado a la Habitacion No. " << habBuscar << "." << endl;
            cout << "Inventario actualizado correctamente en el Almacen." << endl;
            cout << "====================================================" << endl;
        }
        else
        {
            cout << "\n[ERROR]: No se pudo realizar la venta." << endl;
            cout << "Motivo: El ID del producto no existe, esta inactivo o NO CUENTA CON STOCK SUFICIENTE." << endl;
        }
    }
    else
    {
        cout << "\n[ERROR]: No se puede cargar consumos a la Habitacion No. " << habBuscar << "." << endl;
        cout << "Motivo: La habitacion no existe o actualmente se encuentra [DISPONIBLE]." << endl;
        cout << "(Solo se pueden cargar consumos a habitaciones ocupadas)." << endl;
    }
}
//OPCION 5
//------------------------------------------------------------------------------------------------------------------------------
void calcularPagoCheckOut()
{
    long int ciBuscar;
    int habOcupada = 0;
    int diasDeEstadia = 0;
    float precioPorNoche = 0.0;
    
    float subtotalHospedaje = 0.0;
    float subtotalConsumos = 0.0;
    float totalFactura = 0.0;

    bool huespedEncontrado = false;
    bool reservaEncontrada = false;
    bool habitacionEncontrada = false;

    Huesped hu;
    Reserva reserv;
    Habitacion habit;
    ConsumoExtra consumos;
    Producto prroductos;
    PagoFactura pago; // Estructura oficial del grupo

    system("cls");
    cout << "=== OP5: CALCULAR PAGO Y CHECK-OUT (CERRAR ESTADÍA) ===" << endl;
    cout << "=======================================================" << endl;

    cout << "Ingrese el CI del Huesped que realiza el Check-Out: ";
    cin >> ciBuscar;

    // LOCALIZAR AL HUESPED ACTIVO EN EL HOTEL
    fstream archivoHues;
    archivoHues.open("HUESPEDES.BIN", ios::in | ios::out | ios::binary);
    if (archivoHues.good())
    {
        while (archivoHues.read((char*)&hu, sizeof(Huesped)) && !huespedEncontrado)
        {
            if (hu.ci == ciBuscar && hu.activo == true)
            {
                huespedEncontrado = true;
                habOcupada = hu.numHabitacion; //extraemos la habitacion que usaba

                // BAJA LOGICA modificamos el registro 
                hu.activo = false; 
                
                // SOBREESCRITURA
                archivoHues.seekp(-sizeof(Huesped), ios::cur);
                archivoHues.write((char*)&hu, sizeof(Huesped));
            }
        }
        archivoHues.close();
    }

    // SI EL HUESPED EXISTE 
    if (huespedEncontrado)
    {
        //BUSCAR LA DURACION EN RESERVAS.BIN
        ifstream archivoRes;
        archivoRes.open("RESERVAS.BIN", ios::binary);
        if (archivoRes.good())
        {
            while (archivoRes.read((char*)&reserv, sizeof(Reserva)) && !reservaEncontrada)
            {
                if (reserv.ciHuesped == ciBuscar && reserv.numHabitacion == habOcupada && reserv.activo == true)
                {
                    reservaEncontrada = true;
                    diasDeEstadia = reserv.diasEstadia;
                }
            }
            archivoRes.close();
        }

        // CONTROL DE SEGURIDAD: Si entro sin reserva previa por la Opcion 1, contamos minimo 1 día
        if (!reservaEncontrada)
        {
            cout << "\n[SISTEMA]: El cliente no tenia reserva previa (Ingreso directo por Opcion 1)." << endl;
            cout << "Ingrese la cantidad de dias que se hospedo el cliente: ";
            cin >> diasDeEstadia;
            
            if(diasDeEstadia <= 0) {
                diasDeEstadia = 1; // Validamos que al menos cobre un día
            }
        }

        // OBTENER EL PRECIO DE LA HABITACION Y LIBERARLA (estado = 0)
        fstream archivoHab;
        archivoHab.open("HABITACIONES.BIN", ios::in | ios::out | ios::binary);
        if (archivoHab.good())
        {
            while (archivoHab.read((char*)&habit, sizeof(Habitacion)) && !habitacionEncontrada)
            {
                if (habit.numero == habOcupada && habit.activo == true)
                {
                    habitacionEncontrada = true;
                    precioPorNoche = habit.precioNoche;

                    // LIBERACION: Cambiar estado a Disponible (0) 
                    habit.estado = 0; 
                    
                    // MODIFICACION 
                    archivoHab.seekp(-sizeof(Habitacion), ios::cur);
                    archivoHab.write((char*)&habit, sizeof(Habitacion));
                }
            }
            archivoHab.close();
        }

        //LIQUIDACION DE CONSUMOS EXTRAS
        ifstream archivoCons;
        archivoCons.open("CONSUMOS.BIN", ios::binary);
        if (archivoCons.good())
        {
            while (archivoCons.read((char*)&consumos, sizeof(ConsumoExtra)))
            {
                if (consumos.numHabitacion == habOcupada && consumos.activo == true)
                {
                    // Por cada articulo consumido, buscamos su precio en PRODUCTOS.BIN 
                    ifstream archivoProd;
                    archivoProd.open("PRODUCTOS.BIN", ios::binary);
                    if (archivoProd.good())
                    {
                        bool prodEncontrado = false;
                        while (archivoProd.read((char*)&prroductos, sizeof(Producto)) && !prodEncontrado)
                        {
                            if (prroductos.idProducto == consumos.idProducto)
                            {
                                prodEncontrado = true;
                                subtotalConsumos += (consumos.cantidad * prroductos.precioVenta);
                            }
                        }
                        archivoProd.close();
                    }
                }
            }
            archivoCons.close();
        }

        //OPERACIONES FINALES Y DESPLIEGUE DE LA FACTURA
        subtotalHospedaje = diasDeEstadia * precioPorNoche;
        totalFactura = subtotalHospedaje + subtotalConsumos;

        cout << "\n=======================================================" << endl;
        cout << "               FACTURA DE CHECK-OUT                    " << endl;
        cout << "=======================================================" << endl;
        cout << " CLIENTE CI:      " << ciBuscar << endl;
        cout << " HABITACION NO.:  " << habOcupada << endl;
        cout << " TIEMPO ESTADIA:  " << diasDeEstadia << " dia(s)." << endl;
        cout << " PRECIO POR NOCHE:" << precioPorNoche << " Bs." << endl;
        cout << "-------------------------------------------------------" << endl;
        cout << " Subtotal Hospedaje:  " << subtotalHospedaje << " Bs." << endl;
        cout << " Subtotal Consumos:    " << subtotalConsumos << " Bs." << endl;
        cout << "-------------------------------------------------------" << endl;
        cout << " TOTAL NETO A PAGAR:  " << totalFactura << " Bs." << endl;
        cout << "=======================================================" << endl;
        cout << " [OK]: El cuarto ha sido DESALOJADO y queda [DISPONIBLE]." << endl;
        cout << " [OK]: Baja logica del huesped efectuada correctamente." << endl;
        cout << "=======================================================" << endl;

        // GUARDAR EN EL ARCHIVO (PAGOS.BIN)
        // =========================================================================

        int idContador = 1;
        ifstream archivoLeerPagos("PAGOS.BIN", ios::binary);
        if (archivoLeerPagos.good())
        {
            PagoFactura pAux;
            while(archivoLeerPagos.read((char*)&pAux, sizeof(PagoFactura)))
            {
                idContador++;
            }
            archivoLeerPagos.close();
        }

        // Cargamos los datos en la estructura 
        pago.idFactura = idContador;
        pago.ciHuesped = ciBuscar;
        pago.numHabitacion = habOcupada;
        pago.montoTotal = totalFactura;
        
        // Pedimos los datos de la fecha 
        cout << "\n--- REGISTRO OFICIAL DE CAJA ---" << endl;
        cout << "Ingrese el Dia de pago (1-30): "; cin >> pago.fechaPago.dia;
        cout << "Ingrese el Mes de pago (1-12): "; cin >> pago.fechaPago.mes;
        cout << "Ingrese el Anio de pago: "; cin >> pago.fechaPago.anio;
        pago.activo = true;

        // Escritura en PAGOS.BIN 
        ofstream archivoEscribirPagos("PAGOS.BIN", ios::binary | ios::app);
        if (archivoEscribirPagos.good())
        {
            archivoEscribirPagos.write((char*)&pago, sizeof(PagoFactura));
            archivoEscribirPagos.close();
            cout << "\n[SISTEMA]: Registro financiero guardado exitosamente en PAGOS.BIN" << endl;
        }
    }
    else
    {
        cout << "\n[ERROR]: No se puede procesar el Check-Out." << endl;
        cout << "Motivo: El CI " << ciBuscar << " no corresponde a ningun huesped ALOJADO actualmente." << endl;
    }
}
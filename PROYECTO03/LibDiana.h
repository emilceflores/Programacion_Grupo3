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
        cout << "\t3. Buscar Cliente / Reserva por CI" << endl; 
        cout << "\t4. Registrar Venta de Producto (Consumo Extra)" << endl;
        cout << "\t5. Calcular Pago y Check-Out (Facturacion Integrada)" << endl;
        cout << "\t6. Ver Listado de Habitaciones Disponibles" << endl;
        cout << "\t0. Cerrar Sesion de Mostrador" << endl;
        cout << "====================================" << endl;
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
                cout << "\nCerrando sesion de recepcion. Volviendo al menu de acceso..." << endl;
                break;
            default:
                cout << "\n[ERROR]: Opcion no valida dentro del mostrador." << endl;
                system("pause");
                break;
        }
    } while (opcRecepcion != 0);
}
// OPCIÓN 6: VER LISTADO DE HABITACIONES DISPONIBLES
void verHabitacionesDisponibles()
{
    Habitacion hab;
    bool hayDisponibles = false;

    system("cls");
    cout << "=== OP6: LISTADO DE HABITACIONES DISPONIBLES ===" << endl;
    cout << "================================================" << endl;
    cout << "No.\tTipo de Cuarto\t\tPrecio/Noche\tEstado" << endl;
    cout << "------------------------------------------------" << endl;

    ifstream archivo;
    archivo.open("HABITACIONES.BIN", ios::binary);
    
    if (archivo.good())
    {
        while (archivo.read((char*)&hab, sizeof(Habitacion)))
        {
            // solo mostramos las habitaciones que estén operativas (activo == true) y libres (estado == 0)
            if (hab.activo == true && hab.estado == 0)
            {
                hayDisponibles = true;
                
                // Imprimimos el numero y el tipo de cuarto
                cout << hab.numero << "\t" << hab.tipo;
                
                // CONTROL DE ESTÉTICA: Tabulaciones dinámicas según el largo del texto
                if (strlen(hab.tipo) < 8) 
                {
                    cout << "\t\t\t";
                }
                else if (strlen(hab.tipo) < 16) 
                {
                    cout << "\t\t";
                }
                else 
                {
                    cout << "\t";
                }

                // Imprimimos el precio y el estado final
                cout << hab.precioNoche << " Bs.\tDisponible" << endl;
            }
        }
        archivo.close();
    }
    else
    {
        cout << "\n[ERROR]: No se pudo abrir el archivo de habitaciones o está vacío." << endl;
    }

    if (!hayDisponibles)
    {
        cout << "\n[AVISO]: El hotel se encuentra lleno en este momento. No hay cuartos disponibles." << endl;
    }
    cout << "------------------------------------------------" << endl;
}

// OPCION 2: CREAR RESERVA DIRECTA
//-----------------------------------------------------------------------
void crearReservaDirecta()
{
    Reserva r;
    Habitacion hab;
    int idContador = 1;
    bool habVerificada = false;
    bool errorHabitacion = false; 

    system("cls");
    cout << "=== OP2: CREAR NUEVA RESERVA DIRECTA ===" << endl;
    cout << "========================================" << endl;

    // Primero contamos cuantas reservas hay para generar el ID automatico
    ifstream archivoLeerRes;
    archivoLeerRes.open("RESERVAS.BIN", ios::binary);
    
    if (archivoLeerRes.good())
    {
        Reserva rAux;
        while (archivoLeerRes.read((char*)&rAux, sizeof(Reserva)))
        {
            idContador++; 
        }
        archivoLeerRes.close();
    }

    r.idReserva = idContador; 
    cout << "Ingrese el CI del Cliente: "; cin >> r.ciHuesped;
    cout << "Ingrese el Numero de Habitacion a Reservar: "; cin >> r.numHabitacion;

    // Abrimos el archivo de habitaciones en modo lectura/escritura para bloquearla
    fstream archivoHab;
    archivoHab.open("HABITACIONES.BIN", ios::in | ios::out | ios::binary);

    if (archivoHab.good())
    {
        while (archivoHab.read((char*)&hab, sizeof(Habitacion)) && !habVerificada && !errorHabitacion)
        {
            if (hab.numero == r.numHabitacion && hab.activo == true)
            {
                if (hab.estado == 0) 
                {
                    habVerificada = true;
                    hab.estado = 1; 
                    
                    archivoHab.seekp(-sizeof(Habitacion), ios::cur);
                    archivoHab.write((char*)&hab, sizeof(Habitacion));
                }
                else
                {
                    errorHabitacion = true; // SE ACTIVA LA BANDERA DE ERROR
                    cout << "\n[ERROR]: La habitacion ya se encuentra comprometida u ocupada." << endl;
                }
            }
        }
        archivoHab.close();
    }

    // SI LA HABITACION FUE VERIFICADA Y NO HUBO ERRORES RECIEN DEJA TERMINAR LA RESERVA
    if (habVerificada && !errorHabitacion)
    {
        cout << "Ingrese Dia previsto de ingreso (1-31): "; cin >> r.fechaIngreso.dia;
        cout << "Ingrese Mes previsto de ingreso (1-12): "; cin >> r.fechaIngreso.mes;
        cout << "Ingrese Anio previsto de ingreso: "; cin >> r.fechaIngreso.anio;
        cout << "Ingrese la cantidad de dias planeados de estadia: "; cin >> r.diasEstadia;
        r.activo = true; 

        ofstream archivoEscribirRes;
        archivoEscribirRes.open("RESERVAS.BIN", ios::binary | ios::app);
        
        if (archivoEscribirRes.good())
        {
            archivoEscribirRes.write((char*)&r, sizeof(Reserva));
            archivoEscribirRes.close();
            cout << "\n[OK]: Reserva #" << r.idReserva << " registrada exitosamente en el sistema." << endl;
        }
    }
    else if (!habVerificada && !errorHabitacion)
    {
        cout << "\n[ERROR]: El numero de habitacion elegido no existe o no esta disponible." << endl;
    }
}
// OPCION 1: REGISTRAR HUESPED DIRECTO O COMPLEMENTAR RESERVA (CHECK-IN)
//-----------------------------------------------------------------------
void registrarHuespedCheckIn()
{
    Huesped h;
    Habitacion hab;
    Reserva r;
    int numHabDeseada;
    bool habitacionValida = false;
    bool tieneReservaPrevia = false;
    bool errorHabitacion = false; 

    system("cls");
    cout << "=== OP1: REGISTRAR HUESPED (CHECK-IN FÍSICO) ===" << endl;
    cout << "=================================================" << endl;

    cout << "Ingrese el CI del Huesped: "; cin >> h.ci;
    cin.ignore(); // limpiar el buffer antes de leer cadenas con espacios
    cout << "Ingrese Nombre Completo: "; cin.getline(h.nombre, 30);
    cout << "Ingrese Procedencia: "; cin.getline(h.procedencia, 30);
    cout << "Ingrese Numero de Celular: "; cin >> h.celular;
    cout << "Ingrese el numero de habitacion a asignar: "; cin >> numHabDeseada;

    // VALIDACION, el cuarto está bloqueado por una reserva de este mismo cliente?
    ifstream archivoRes;
    archivoRes.open("RESERVAS.BIN", ios::binary);
    
    if (archivoRes.good())
    {
        while (archivoRes.read((char*)&r, sizeof(Reserva)))
        {
            if (r.numHabitacion == numHabDeseada && r.ciHuesped == h.ci && r.activo == true)
            {
                tieneReservaPrevia = true; // confirmado... el bloqueo de la habitaciOn es suyo
            }
        }
        archivoRes.close();
    }

    // ahora pasamos a verificar y actualizar el estado de la habitacion
    fstream archivoHab;
    archivoHab.open("HABITACIONES.BIN", ios::in | ios::out | ios::binary);
    
    if (archivoHab.good())
    {
        while (archivoHab.read((char*)&hab, sizeof(Habitacion)) && !habitacionValida && !errorHabitacion)
        {
            if (hab.numero == numHabDeseada && hab.activo == true)
            {
                if (hab.estado == 0) // Caso A: La habitacion esta libre para cualquiera (ingreso directo sin reserva)
                {
                    habitacionValida = true;
                    hab.estado = 1; // la ocupamos oficialmente
                    
                    archivoHab.seekp(-sizeof(Habitacion), ios::cur);
                    archivoHab.write((char*)&hab, sizeof(Habitacion));
                }
                else if (hab.estado == 1 && tieneReservaPrevia) // Caso B: Estaba en estado 1 pero pertenece a su reserva
                {
                    habitacionValida = true;
                    cout << "\n[SISTEMA]: Reserva previa detectada y validada para este CI. Acceso autorizado." << endl;
                }
                else
                {
                    errorHabitacion = true; // ACTIVAMOS ERROR 
                    cout << "\n[AVISO]: La habitacion " << numHabDeseada << " ya esta OCUPADA o Reservada por otro cliente." << endl;
                }
            }
        }
        archivoHab.close();
    }
    else
    {
        errorHabitacion = true; // SI EL ARCHIVO NO ABRE TAMBIEN ES UN ERROR DE CONTROL
        cout << "[ERROR]: El archivo de habitaciones no esta configurado." << endl;
    }

    // SI LA HABITACION FUE APROBADA Y NO HAY ERRORES, GUARDAMOS AL HUESPED
    if (habitacionValida && !errorHabitacion)
    {
        h.numHabitacion = numHabDeseada;
        h.activo = true; // El huesped entra en estado activo en el hotel

        ofstream archivoHues;
        archivoHues.open("HUESPEDES.BIN", ios::binary | ios::app);
        
        if (archivoHues.good())
        {
            archivoHues.write((char*)&h, sizeof(Huesped));
            archivoHues.close();
            cout << "\n[OK]: Check-In completado con exito. El Huesped esta oficialmente alojado." << endl;
        }
    }
    else if (!habitacionValida && !errorHabitacion)
    {
        cout << "\n[ERROR]: El numero de habitacion ingresado no existe o no esta operativa." << endl;
    }
}
// OPCION 3: BUSQUEDA INTEGRADA DE CLIENTE (CON CUARTO ASIGNADO)
//---------------------------------------------------------------------
void buscarReservaActivaCI()
{
    long int ciBuscar;
    Reserva r;
    Huesped h;
    bool encontradoReserva = false;
    bool encontradoHuesped = false;

    system("cls");
    cout << "=== OP3: BUSQUEDA INTEGRADA DE CLIENTE (CON CUARTO ASIGNADO) ===" << endl;
    cout << "=================================================================" << endl;
    cout << "Ingrese el CI del Cliente a consultar: "; cin >> ciBuscar;

    cout << "\nVerificando registros en el sistema..." << endl;

    // primero buscamos si el cliente ya esta hospedado (check-in activo)
    ifstream archivoHues;
    archivoHues.open("HUESPEDES.BIN", ios::binary);
    
    if (archivoHues.good())
    {
        while (archivoHues.read((char*)&h, sizeof(Huesped)))
        {
            if (h.ci == ciBuscar && h.activo == true)
            {
                encontradoHuesped = true;
                cout << "\n-------------------------------------------------------" << endl;
                cout << " [ESTADO DE CONTROL]: HUESPED ACTIVO (HOSPEDADO ACTUALMENTE)" << endl;
                cout << " NOMBRE COMPLETO:     " << h.nombre << endl;
                cout << " HABITACION ACTUAL:   " << h.numHabitacion << endl; // AQUI LE MUESTRAS SU CUARTO
                cout << " CELULAR DE CONTACTO: " << h.celular << endl;
                cout << " PROCEDENCIA:         " << h.procedencia << endl;
                cout << "-------------------------------------------------------" << endl;
            }
        }
        archivoHues.close();
    }

    // segundo ,buscamos si tiene alguna reserva guardada en espera
    ifstream archivoRes;
    archivoRes.open("RESERVAS.BIN", ios::binary);
    
    if (archivoRes.good())
    {
        while (archivoRes.read((char*)&r, sizeof(Reserva)))
        {
            if (r.ciHuesped == ciBuscar && r.activo == true)
            {
                encontradoReserva = true;
                cout << "\n-------------------------------------------------------" << endl;
                cout << " [ESTADO DE CONTROL]: RESERVA VIGENTE (EN ESPERA DE ARRIBO)" << endl;
                cout << " CODIGO DE RESERVA:   " << r.idReserva << endl;
                cout << " HABITACION RESERVADA:" << r.numHabitacion << endl; // TAMBIEN EN RESERVAS MUESTRA EL CUARTO
                cout << " FECHA ESTIMADA:      " << r.fechaIngreso.dia << "/" << r.fechaIngreso.mes << "/" << r.fechaIngreso.anio << endl;
                cout << " NOCHES CALCULADAS:   " << r.diasEstadia << " noche(s)." << endl;
                cout << "-------------------------------------------------------" << endl;
            }
        }
        archivoRes.close();
    }

    //si no existe en ningun lado avisamos 
    if (!encontradoHuesped && !encontradoReserva)
    {
        cout << "\n[SISTEMA]: El cliente con CI " << ciBuscar << " no figura en lista de huespedes ni en reservas activas." << endl;
    }
}

// OPCION 4: REGISTRAR CONSUMO EXTRA (VENTAS CON VALIDACION DE HUESPED)
//---------------------------------------------------------------------------
void registrarVentaProducto()
{
    ConsumoExtra nuevoConsumo;
    Producto prod;
    Huesped h;
    int idBuscarProd, cantPedida;
    int idContadorConsumo = 1;
    bool productoEncontrado = false;
    bool habitacionOcupadaPorHuesped = false; 
    bool ventaExitosa = false; 

    system("cls");
    cout << "=== OP4: REGISTRAR CONSUMO EXTRA (FRIGOBAR / ROOM SERVICE) ===" << endl;
    cout << "==============================================================" << endl;

    cout << "Ingrese el numero de habitacion que consume: "; cin >> nuevoConsumo.numHabitacion;

    // PRIMERO VALIDAMOS QUE HAYA UN HUESPED FISICO VIVIENDO EN ESA HABITACION
    ifstream archivoHues;
    archivoHues.open("HUESPEDES.BIN", ios::binary);
    
    if (archivoHues.good())
    {
        while (archivoHues.read((char*)&h, sizeof(Huesped)) && !habitacionOcupadaPorHuesped)
        {
            if (h.numHabitacion == nuevoConsumo.numHabitacion && h.activo == true)
            {
                habitacionOcupadaPorHuesped = true;
            }
        }
        archivoHues.close();
    }

    // SI PASO EL FILTRO DEL HUESPED, RECIEN PROCESAMOS TODO
    if (habitacionOcupadaPorHuesped)
    {
        cout << "Ingrese el ID del Producto solicitado: "; cin >> idBuscarProd;

        // abrimos productos para revisar stock y precios
        fstream archivoProd;
        archivoProd.open("PRODUCTOS.BIN", ios::in | ios::out | ios::binary);
        
        if (archivoProd.good())
        {
            while (archivoProd.read((char*)&prod, sizeof(Producto)) && !productoEncontrado)
            {
                if (prod.idProducto == idBuscarProd && prod.activo == true)
                {
                    productoEncontrado = true;
                    cout << "\nProducto: " << prod.nombreProd << " | Precio: " << prod.precioVenta << " Bs. | Stock: " << prod.stock << endl;
                    cout << "Ingrese la cantidad a consumir: "; cin >> cantPedida;

                    // verificamos que tengamos suficiente mercaderia en inventario
                    if (cantPedida <= prod.stock)
                    {
                        prod.stock -= cantPedida; // DESCONTAMOS EL STOCK 
                        
                        // retrocedemos para actualizar el archivo 
                        archivoProd.seekp(-sizeof(Producto), ios::cur);
                        archivoProd.write((char*)&prod, sizeof(Producto));

                        nuevoConsumo.idProducto = idBuscarProd;
                        nuevoConsumo.cantidad = cantPedida;
                        nuevoConsumo.activo = true;
                        ventaExitosa = true; // LA VENTA FUE APROBADA
                    }
                    else
                    {
                        cout << "\n[ERROR]: Stock insuficiente. No se puede procesar el consumo." << endl;
                    }
                }
            }
            archivoProd.close();
        }

        if (!productoEncontrado)
        {
            cout << "\n[ERROR]: El ID de producto ingresado no existe o no esta habilitado." << endl;
        }

        // SI LA VENTA FUE EXITOSA RECIEN GUARDAMOS EN EL ARCHIVO
        if (ventaExitosa)
        {
            // contamos consumos anteriores para sacar el ID AUTOMATICO CORRELATIVO
            ifstream archivoLeerCons;
            archivoLeerCons.open("CONSUMOS.BIN", ios::binary);
            
            if (archivoLeerCons.good())
            {
                ConsumoExtra cAux;
                while (archivoLeerCons.read((char*)&cAux, sizeof(ConsumoExtra)))
                {
                    idContadorConsumo++;
                }
                archivoLeerCons.close();
            }

            nuevoConsumo.idConsumo = idContadorConsumo;

            // guardamos el consumo al final del archivo consumos
            ofstream archivoEscribirCons;
            archivoEscribirCons.open("CONSUMOS.BIN", ios::binary | ios::app);
            
            if (archivoEscribirCons.good())
            {
                archivoEscribirCons.write((char*)&nuevoConsumo, sizeof(ConsumoExtra));
                archivoEscribirCons.close();
                cout << "\n[OK]: Consumo registrado correctamente cargado a la Factura de la habitacion " << nuevoConsumo.numHabitacion << endl;
            }
        }
    }
    else
    {
        cout << "\n[ERROR]: No se pueden cargar consumos a la habitacion " << nuevoConsumo.numHabitacion << "." << endl;
        cout << "MOTIVO: La habitacion no tiene un huesped hospedado actualmente (puede estar vacia o solo reservada)." << endl;
    }
}
// OPCION 5: CALCULAR PAGO, CHECK-OUT Y CONTROL DE NO-SHOW
//---------------------------------------------------------------------------
void calcularPagoCheckOut()
{
    long int ciBuscar;
    Huesped h;
    Reserva r;
    Habitacion hab;
    ConsumoExtra con;
    Producto prod;
    PagoFactura pago;

    int habOcupada = 0;
    int diasEstadia = 0;
    float precioNoche = 0.0;
    float subtotalHospedaje = 0.0;
    float subtotalConsumos = 0.0;
    float totalFactura = 0.0;

    bool clienteHospedado = false;
    bool tieneReserva = false;
    bool habitacionEncontrada = false;
    bool procederFactura = true; 

    system("cls");
    cout << "=== OP5: CALCULAR PAGO Y CHECK-OUT (FACTURACION INTEGRADA) ===" << endl;
    cout << "==============================================================" << endl;
    cout << "Ingrese el CI del Cliente para liquidar cuenta: "; cin >> ciBuscar;

    // BUSCAMOS SI EL CLIENTE ESTA HOSPEDADO ACTUALMENTE
    fstream archivoHues;
    archivoHues.open("HUESPEDES.BIN", ios::in | ios::out | ios::binary);
    
    if (archivoHues.good())
    {
        while (archivoHues.read((char*)&h, sizeof(Huesped)) && !clienteHospedado)
        {
            if (h.ci == ciBuscar && h.activo == true)
            {
                clienteHospedado = true;
                habOcupada = h.numHabitacion; 
                
                h.activo = false; // BAJA LOGICA
                archivoHues.seekp(-sizeof(Huesped), ios::cur);
                archivoHues.write((char*)&h, sizeof(Huesped));
            }
        }
        archivoHues.close();
    }

    // BUSCAMOS SI EXISTE UNA RESERVA ASOCIADA
    ifstream archivoRes;
    archivoRes.open("RESERVAS.BIN", ios::binary);
    if (archivoRes.good())
    {
        while (archivoRes.read((char*)&r, sizeof(Reserva)) && !tieneReserva)
        {
            if (r.ciHuesped == ciBuscar && r.activo == true)
            {
                tieneReserva = true;
                diasEstadia = r.diasEstadia;
                
                if (!clienteHospedado)
                {
                    habOcupada = r.numHabitacion; 
                }
            }
        }
        archivoRes.close();
    }

    // VERIFICACION DE CASOS 
    if (!clienteHospedado && tieneReserva)
    {
        cout << "\n[SISTEMA]: DETECTADO NO-SHOW (El cliente tenia reserva pero nunca hizo Check-In)." << endl;
        cout << "[SISTEMA]: Se procedera a cobrar una penalizacion equivalente a 1 noche de estadia." << endl;
        diasEstadia = 1; 
    }
    else if (!clienteHospedado && !tieneReserva)
    {
        procederFactura = false; // NO SE PROCESA NADA
        cout << "\n[ERROR]: El CI ingresado no corresponde a ningun huesped alojado ni a reservas activas." << endl;
    }

    // SI TODO ESTA EN ORDEN, RECIEN SE CORREN LOS PROCESOS FINALES
    if (procederFactura)
    {
        // OBTENEMOS PRECIO Y LIBERAMOS HABITACION
        fstream archivoHab;
        archivoHab.open("HABITACIONES.BIN", ios::in | ios::out | ios::binary);
        
        if (archivoHab.good())
        {
            while (archivoHab.read((char*)&hab, sizeof(Habitacion)) && !habitacionEncontrada)
            {
                if (hab.numero == habOcupada && hab.activo == true)
                {
                    habitacionEncontrada = true;
                    precioNoche = hab.precioNoche;

                    hab.estado = 0; // LIBERACION CAMBIANDO ESTADO A 0
                    archivoHab.seekp(-sizeof(Habitacion), ios::cur);
                    archivoHab.write((char*)&hab, sizeof(Habitacion));
                }
            }
            archivoHab.close();
        }

        // SI LLEGO A HOSPEDARSE CALCULAMOS LOS EXTRAS
        if (clienteHospedado)
        {
            ifstream archivoCons;
            archivoCons.open("CONSUMOS.BIN", ios::binary);
            if (archivoCons.good())
            {
                while (archivoCons.read((char*)&con, sizeof(ConsumoExtra)))
                {
                    if (con.numHabitacion == habOcupada && con.activo == true)
                    {
                        ifstream archivoProd;
                        archivoProd.open("PRODUCTOS.BIN", ios::binary);
                        if (archivoProd.good())
                        {
                            bool prodEncontrado = false;
                            while (archivoProd.read((char*)&prod, sizeof(Producto)) && !prodEncontrado)
                            {
                                if (prod.idProducto == con.idProducto)
                                {
                                    prodEncontrado = true;
                                    subtotalConsumos += (con.cantidad * prod.precioVenta);
                                }
                            }
                            archivoProd.close();
                        }
                    }
                }
                archivoCons.close();
            }
        }

        // CALCULOS Y FACTURACION
        subtotalHospedaje = diasEstadia * precioNoche;
        totalFactura = subtotalHospedaje + subtotalConsumos;

        cout << "\n=======================================================" << endl;
        cout << "               FACTURA DE SALIDA (CHECK-OUT)           " << endl;
        cout << "=======================================================" << endl;
        cout << " CLIENTE CI:       " << ciBuscar << endl;
        cout << " HABITACION NO.:   " << habOcupada << endl;
        cout << " DIAS DE ESTADIA:  " << diasEstadia << " noche(s)." << endl;
        cout << " PRECIO POR NOCHE: " << precioNoche << " Bs." << endl;
        cout << "-------------------------------------------------------" << endl;
        cout << " Subtotal Hospedaje: " << subtotalHospedaje << " Bs." << endl;
        cout << " Subtotal Consumos:  " << subtotalConsumos << " Bs." << endl;
        cout << "-------------------------------------------------------" << endl;
        cout << " TOTAL NETO A PAGAR: " << totalFactura << " Bs." << endl;
        cout << "=======================================================" << endl;
        cout << " [OK]: LA HABITACION QUEDA LIBRE Y EN ESTADO [DISPONIBLE]." << endl;
        cout << "=======================================================" << endl;

        // ID AUTOMATICO CORRELATIVO DE PAGOS
        int idContadorPagos = 1;
        ifstream archivoLeerPagos("PAGOS.BIN", ios::binary);
        if (archivoLeerPagos.good())
        {
            PagoFactura pAux;
            while(archivoLeerPagos.read((char*)&pAux, sizeof(PagoFactura)))
            {
                idContadorPagos++;
            }
            archivoLeerPagos.close();
        }

        pago.idFactura = idContadorPagos;
        pago.ciHuesped = ciBuscar;
        pago.numHabitacion = habOcupada;
        pago.montoTotal = totalFactura;
        
        cout << "\n--- REGISTRO OFICIAL DE CAJA RECEPTORA ---" << endl;
        cout << "Ingrese Dia actual de pago (1-31): "; cin >> pago.fechaPago.dia;
        cout << "Ingrese Mes actual de pago (1-12): "; cin >> pago.fechaPago.mes;
        cout << "Ingrese Anio actual de pago: "; cin >> pago.fechaPago.anio;
        pago.activo = true;

        ofstream archivoEscribirPagos;
        archivoEscribirPagos.open("PAGOS.BIN", ios::binary | ios::app);
        if (archivoEscribirPagos.good())
        {
            archivoEscribirPagos.write((char*)&pago, sizeof(PagoFactura));
            archivoEscribirPagos.close();
            cout << "\n[SISTEMA]: EXITO. Datos financieros almacenados en PAGOS.BIN" << endl;
        }
    }
}
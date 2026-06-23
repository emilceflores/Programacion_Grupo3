 
#include <iostream>
#include <fstream>
#include "LibEstructuras.h"

using namespace std;

void RegistrarUsuario();
void RegistrarHabitacion();
void ModificarHabitacion();
void DarBajaHabitacion();
void ReportesFinancieros();
void menuAdministrador();

void RegistrarUsuario()
{
    Usuario usuario;
    ofstream archivo;

    cout << "REGISTRAR USUARIO" << endl;
    cout << "=================" << endl;
    archivo.open("USUARIOS.BIN", ios::binary | ios::app);

    if (archivo.good())
    {
        //para iniciar sesión
        cout << "Ingrese el login: ";
        cin >> usuario.login;
        //la contraseña del usuario
        cout << "Ingrese la password: ";
        cin >> usuario.password;
        //el rol del usuario
        cout << "Ingrese el rol (1=Administrador, 2=Recepcionista): ";
        cin >> usuario.rol;
        
        usuario.activo = true;

        archivo.write((char*)&usuario, sizeof(Usuario));
        cout << "\nUsuario registrado correctamente." << endl;
       
    }
    else
    {
        cout << "Error: No se pudo abrir el archivo USUARIOS.BIN" << endl;
    } 
    archivo.close();
}

void RegistrarHabitacion()
{
    Habitacion habitacion;
    ofstream archivo;

    cout << "REGISTRAR HABITACION" << endl;
    cout << "====================" << endl;

    archivo.open("HABITACIONES.BIN",ios::binary | ios::app);
    if (archivo.good())
    {
        cout << "Ingrese el numero de habitacion: ";
        cin >> habitacion.numero;
        cin.ignore();
        cout << "Ingrese el tipo de habitacion: ";
        cin.getline(habitacion.tipo,30);
        cout << "Ingrese el precio por noche: ";
        cin >> habitacion.precioNoche;
        // La habitación inicia disponible
        habitacion.estado = 0;
        habitacion.activo = true;

        archivo.write((char*)&habitacion,sizeof(Habitacion));

        cout << "\nHabitacion registrada correctamente."<< endl;
    }
    else
    {
        cout << "Error: No se pudo abrir el archivo HABITACIONES.BIN"<< endl;
    }

    archivo.close();

}

void ModificarHabitacion()
{
    Habitacion habitacion;
    fstream archivo;
    int numeroBuscado;
    // Indica si se encontró la habitación
    bool encontrado = false;

    cout << "MODIFICAR HABITACION" << endl;
    cout << "====================" << endl;

    cout << "Ingrese el numero de habitacion a modificar: ";
    cin >> numeroBuscado;

    // archivo en modo BINARIO para leer y escribir 
    archivo.open("HABITACIONES.BIN",ios::in | ios::out | ios::binary);

    // Verificar si el archivo se abrió correctamente
    if (archivo.good())
    {
        while (archivo.read((char*)&habitacion,sizeof(Habitacion)) && !encontrado)
        {
            // Verificar si el número ingresado coincide
            if (habitacion.numero == numeroBuscado && habitacion.activo)
            {
                encontrado = true;

                // Mostrar los datos actuales
                cout << "\nHABITACION ENCONTRADA" << endl;
                cout << "------------------------" << endl;
                cout << "Numero de habitacion: "
                     << habitacion.numero << endl;
                cout << "Tipo de habitacion: "
                     << habitacion.tipo << endl;
                cout << "Precio por noche: "
                     << habitacion.precioNoche << endl;
                cout << "------------------------" << endl;

                // Mover el puntero de escritura al inicio del registro encontrado
                archivo.seekp(-sizeof(Habitacion),ios::cur);
            
                cout << "\nIngrese el nuevo precio: ";
                cin >> habitacion.precioNoche;

                // Escribir los nuevos datos en el archivo
                archivo.write((char*)&habitacion,sizeof(Habitacion));
                cout << "\nHabitacion modificada correctamente."<< endl;
            }
        }
        if (!encontrado)// Verificar si no se encontró la habitación
        {
            cout << "\nHabitacion no encontrada."<< endl;
        }
        archivo.close();
    }
    else
    {
        cout << "Error: No se pudo abrir el archivo HABITACIONES.BIN"<< endl;
    }
}

void DarBajaHabitacion()
{
    Habitacion habitacion;
    fstream archivo;
    int numeroBuscado;
    bool encontrado = false;

    cout << "DAR DE BAJA HABITACION" << endl;
    cout << "======================" << endl;

    cout << "Ingrese el numero de habitacion: ";
    cin >> numeroBuscado;
    // Abrir el archivo en modo BINARIO para leer y escribir
    archivo.open("HABITACIONES.BIN",ios::in | ios::out | ios::binary);
    // Verificar si el archivo se abrió correctamente
    if (archivo.good())
    {
        while (archivo.read((char*)&habitacion,sizeof(Habitacion)) && !encontrado)
        {
            if (habitacion.numero == numeroBuscado && habitacion.activo)
            {
                encontrado = true;
                cout << "\nHABITACION ENCONTRADA" << endl;
                cout << "------------------------" << endl;
                cout << "Numero de habitacion: "
                     << habitacion.numero << endl;
                cout << "Tipo de habitacion: "
                     << habitacion.tipo << endl;
                cout << "------------------------" << endl;

                // Volver al inicio del registro encontrado
                archivo.seekp(-sizeof(Habitacion),ios::cur);
                habitacion.activo = false;// Realizar la baja lógica

                archivo.write((char*)&habitacion,sizeof(Habitacion));

                cout << "\nHabitacion dada de baja correctamente."<< endl;
            }
        }
        // Verificar si no se encontró la habitación
        if (!encontrado)
        {
            cout << "\nHabitacion no encontrada."<< endl;
        }
        archivo.close();
    }
    else
    {
        cout << "Error: No se pudo abrir el archivo HABITACIONES.BIN"<< endl;
    }
}

void ReportesFinancieros()
{
    PagoFactura factura;
    ifstream archivo;

    // acumula el dinero ingresado
    float totalIngresos = 0;

    cout << "REPORTE FINANCIERO" << endl;
    cout << "==================" << endl;

    // Abrir el archivo en modo BINARIO para lectura
    archivo.open("FACTURAS.BIN", ios::binary);

    // Verificar si el archivo se abrió correctamente
    if (archivo.good())
    {
        // Leer todos los registros del archivo
        while (archivo.read((char*)&factura, sizeof(PagoFactura)))
        {
            // Verificar que la factura esté activa
            if (factura.activo)
            {
                // Mostrar los datos de la factura
                cout << "Factura Nro: " << factura.idFactura << endl;
                cout << "CI Huesped: " << factura.ciHuesped << endl;
                cout << "Habitacion: " << factura.numHabitacion << endl;
                cout << "Monto Total: Bs. " << factura.montoTotal << endl;
                cout << "Fecha de Pago: "<< factura.fechaPago.dia << "/"<< factura.fechaPago.mes << "/"<< factura.fechaPago.anio << endl;
                cout << "------------------------" << endl;
                // Acumular el monto total
                totalIngresos += factura.montoTotal;
            }
        }
        // Mostrar el total general de ingresos
        cout << "\nTOTAL DE INGRESOS DEL HOTEL: Bs. "<< totalIngresos << endl;

        archivo.close();
    }
    else
    {
        cout << "Error: No se pudo abrir el archivo FACTURAS.BIN" << endl;
    }
}

void menuAdministrador()
{
    int opcion = 0;

    do
    {
        cout << "\nMENU ADMINISTRADOR\n";
        cout << "==================\n";
        cout << "1. Registrar usuario\n";
        cout << "2. Registrar habitacion\n";
        cout << "3. Modificar habitacion\n";
        cout << "4. Dar de baja habitacion\n";
        cout << "5. Reportes financieros\n";
        cout << "0. Cerrar sesion\n";

        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion)
        {
            case 1:
                RegistrarUsuario();
                break;

            case 2:
                RegistrarHabitacion();
                break;

            case 3:
                ModificarHabitacion();
                break;

            case 4:
                DarBajaHabitacion();
                break;

            case 5:
                ReportesFinancieros();
                break;
        }

    }while(opcion != 0);
}
//Materia:Programa I, Paralelo 4
//Grupo 3
//Autor:Emilce Jael Flores Gutierrez
//Nombre del proyecto: Sistema de gestion hotelera, 
//control de inventarios y facturaion integrada "Ceasars palace"
#include <iostream>
#include <fstream>

using namespace std;

void RegistrarUsuario();
void RegistrarHabitacion();
void RegistrarProducto();
void ModificarDatosGenerales();
void menuReportes();
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
        do
        {
            cout << "Ingrese el rol (1=Administrador, 2=Recepcionista): ";
            cin >> usuario.rol;
            if (usuario.rol != 1 && usuario.rol != 2)
            {
                cout << "Error: Debe ingresar 1 o 2." << endl;
            }
            
        } while (usuario.rol != 1 && usuario.rol != 2);
        
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
        do
        {
            cout << "Ingrese el numero de habitacion: ";
            cin >> habitacion.numero;
            if (habitacion.numero <= 0)
            {
                cout << "Error: El numero debe ser mayor que cero." << endl;
            }
        } while (habitacion.numero <= 0);
        
        cin.ignore();
        cout << "Ingrese el tipo de habitacion: ";
        cin.getline(habitacion.tipo,30);

        do
        {
            cout << "Ingrese el precio por noche: ";
            cin >> habitacion.precioNoche;
            if (habitacion.precioNoche < 0)
            {
                cout << "Error: El precio no puede ser negativo." << endl;
            }
        } while (habitacion.precioNoche < 0);
        
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

void RegistrarProducto()
{
    Producto p;
    ofstream archivo("PRODUCTOS.BIN", ios::binary | ios::app);

    cout << "\nREGISTRAR PRODUCTO\n";
    cout << "==================\n";

    cout << "ID Producto: ";
    cin >> p.idProducto;

    cin.ignore();
    cout << "Nombre: ";
    cin.getline(p.nombreProd, 30);

    cout << "Precio Venta: ";
    cin >> p.precioVenta;

    cout << "Stock: ";
    cin >> p.stock;

    p.activo = true;

    archivo.write((char*)&p, sizeof(Producto));
    archivo.close();

    cout << "Producto registrado correctamente.\n";
}

void ModificarDatosGenerales()
{
    int opc;

    cout << "\nMODIFICAR DATOS GENERALES\n";
    cout << "=========================\n";
    cout << "1. Modificar Usuario\n";
    cout << "2. Modificar Habitacion\n";
    cout << "3. Modificar Producto\n";
    cout << "Seleccione: ";
    cin >> opc;

    if (opc == 1)
    {
        Usuario u;
        fstream archivo("USUARIOS.BIN", ios::in | ios::out | ios::binary);
        char login[30];

        cout << "Login del usuario: ";
        cin >> login;

        while (archivo.read((char*)&u, sizeof(Usuario)))
        {
            if (strcmp(u.login, login) == 0 && u.activo)
            {
                archivo.seekp(-sizeof(Usuario), ios::cur);

                cout << "Nuevo password: ";
                cin >> u.password;

                do {
                    cout << "Nuevo rol (1 Admin / 2 Recepcionista): ";
                    cin >> u.rol;
                } while (u.rol != 1 && u.rol != 2);

                archivo.write((char*)&u, sizeof(Usuario));
            }
        }
        archivo.close();
    }
    if (opc == 2)
    {
        Habitacion h;
        fstream archivo("HABITACIONES.BIN", ios::in | ios::out | ios::binary);
        int num;

        cout << "Numero de habitacion: ";
        cin >> num;

        while (archivo.read((char*)&h, sizeof(Habitacion)))
        {
            if (h.numero == num && h.activo)
            {
                archivo.seekp(-sizeof(Habitacion), ios::cur);

                do {
                    cout << "Nuevo precio por noche: ";
                    cin >> h.precioNoche;
                } while (h.precioNoche < 0);

                archivo.write((char*)&h, sizeof(Habitacion));
            }
        }

        archivo.close();
    }

    if (opc == 3)
    {
        Producto p;
        fstream archivo("PRODUCTOS.BIN", ios::in | ios::out | ios::binary);
        int id;

        cout << "ID producto: ";
        cin >> id;

        while (archivo.read((char*)&p, sizeof(Producto)))
        {
            if (p.idProducto == id && p.activo)
            {
                archivo.seekp(-sizeof(Producto), ios::cur);

                do {
                    cout << "Nuevo precio: ";
                    cin >> p.precioVenta;
                } while (p.precioVenta < 0);

                do {
                    cout << "Nuevo stock: ";
                    cin >> p.stock;
                } while (p.stock < 0);

                archivo.write((char*)&p, sizeof(Producto));
            }
        }
        archivo.close();
    }
}
void menuReportes()
{
    int opcion;
    do
    {
        cout << "\nPANEL DE REPORTES\n";
        cout << "=========================\n";
        cout << "1. Reporte de Habitaciones\n";
        cout << "2. Reporte de Productos\n";
        cout << "3. Reporte de Ingresos\n";
        cout << "0. Volver\n";
        cout << "Seleccione: ";
        cin >> opcion;

        if (opcion == 1)
        {
            Habitacion h;
            ifstream archivo("HABITACIONES.BIN", ios::binary);
            cout << "\nHABITACIONES\n";
            cout << "---------------------\n";
            while (archivo.read((char*)&h, sizeof(Habitacion)))
            {
                if (h.activo)
                {
                    cout << "Nro: " << h.numero << endl;
                    cout << "Tipo: " << h.tipo << endl;
                    cout << "Precio: " << h.precioNoche << endl;
                    cout << "Estado: ";
                    if (h.estado == 0)
                    {
                        cout << "Libre" << endl;
                    }
                    else
                    {
                        cout << "Ocupada" << endl;
                    }
                    cout << "---------------------\n";
                }
            }
            archivo.close();
        }

        if (opcion == 2)
        {
            Producto p;
            ifstream archivo("PRODUCTOS.BIN", ios::binary);

            cout << "\nPRODUCTOS\n";
            cout << "---------------------\n";

            while (archivo.read((char*)&p, sizeof(Producto)))
            {
                if (p.activo)
                {
                    cout << "ID: " << p.idProducto << endl;
                    cout << "Nombre: " << p.nombreProd << endl;
                    cout << "Precio: " << p.precioVenta << endl;
                    cout << "Stock: " << p.stock << endl;
                    cout << "---------------------\n";
                }
            }
            archivo.close();
        }
        if (opcion == 3)
        {
            PagoFactura factura;
            ifstream archivo("FACTURAS.BIN", ios::binary);

            float total = 0;

            cout << "\nINGRESOS DEL HOTEL\n";
            cout << "---------------------\n";
            while (archivo.read((char*)&factura, sizeof(PagoFactura)))
            {
                if (factura.activo)
                {
                    cout << "Factura: " << factura.idFactura << endl;
                    cout << "CI: " << factura.ciHuesped << endl;
                    cout << "Habitacion: " << factura.numHabitacion << endl;
                    cout << "Monto: " << factura.montoTotal << endl;
                    cout << "---------------------\n";

                    total += factura.montoTotal;
                }
            }
            cout << "\nTOTAL INGRESOS: "<< total <<" Bs" << endl;
            archivo.close();
        }

    } while (opcion != 0);
}

void menuAdministrador()
{
    int opcion = 0;

    do
    {
        system("cls"); // si estás en Linux usa system("clear");

        cout << "\nMENU ADMINISTRADOR" << endl;
        cout << "============================" << endl;
        cout << "1. Registrar Usuario" << endl;
        cout << "2. Registrar Habitacion" << endl;
        cout << "3. Registrar Producto en Inventario" << endl;
        cout << "4. Modificar Datos Generales" << endl;
        cout << "5. Visualizar Panel de Reportes" << endl;
        cout << "0. Cerrar Sesion" << endl;
        cout << "============================" << endl;

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
                RegistrarProducto(); 
                break;

            case 4:
                ModificarDatosGenerales(); 
                break;

            case 5:
                menuReportes(); 
                break;

            case 0:
                cout << "Opcion no valida." << endl;
            break;
        }

        system("pause");

    } while(opcion != 0);
}
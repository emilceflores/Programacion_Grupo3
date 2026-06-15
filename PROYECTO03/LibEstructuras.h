struct structFecha 
{
    int dia;
    int mes;
    int anio;
};

struct structUsuario 
{
    char Usuario[30];
    char contrasenia[30];
    int rol; //1=administrador; 2=recepcionista; 3= consulta.
    bool activo; // true=habilitado, false=deshabilitado
};
struct structHuesped 
{
    int CI;
    char nombre[30];
    char procedencia[30]; //// ciudad o pais de origen
    int celurlar;
    int num_Habitacion;// Habitación asignada (0 si no tiene)
    bool activo; // true=activo, false=baja
};
struct structHabitacion
{
    int numero;
    char tipo[20]; // Simple, doble, matrimonial, suite
    int precio_Noche;
    int estado; //0=disponible 1=ocupado
    bool activo; // trues = habilitada, false = deshabilitada
};
struct structReserva
{
    int ID_reserva;
    int CI_huesped;
    int num_habitacion;

    structFecha fechaIngreso;
    int diasEstadia; // Cantidad de días reservados
    bool activo; // true=reserva activa, false=cancelada
};
struct structPago {

    int ID_Factura;
    int CI_Huesped;
    int num_habitacion;
    double montoTotal; // Monto total a pagar

    structFecha fechaPago;      
    bool activo;  // true=válida, false=anulada

};
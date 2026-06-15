// LIBRERÍA GENERAL DE ESTRUCTURAS 

struct Fecha {
    int dia;   
    int mes;
    int anio;
};


struct Usuario {
    char login[30]; 
    char password[30]; 
    int rol;            // 1 = Administrador, 2 = Recepcionista
    bool activo;
};

struct Huesped {
    long int ci;           // Cédula de Identidad única
    char nombre[30];      // Nombres y apellidos completos
    char procedencia[30];  // Lugar de origen
    long int celular;
    int numHabitacion;     // Número asignado (0 si no está alojado)
    bool activo;
};

struct Habitacion {
    int numero;         // Código identificador físico (ej. 101)
    char tipo[30];      // Categoría (Simple, Doble, Matrimonial, Suite)
    float precioNoche;
    int estado;         // 0 = Disponible, 1 = Ocupada
    bool activo;        // true = Operativa, false = Mantenimiento
};

struct Producto {
    int idProducto;       // Código único del artículo
    char nombreProd[50]; 
    float precioVenta;
    int stock; 
    bool activo;          // true = Disponible, false = no Disponible
};

struct ConsumoExtra {
    int idConsumo;      // Código correlativo automático
    int numHabitacion; 
    int idProducto;
    int cantidad;
    bool activo;        // true = Válido, false = Anulado
};

struct Reserva {
    int idReserva;
    int ciHuesped;
    int numHabitacion;
    Fecha fechaIngreso; // Estructura anidada de tiempo
    int diasEstadia;
    bool activo;        // true = Vigente, false = Cancelada
};

struct PagoFactura {
    int idFactura;
    int ciHuesped;
    int numHabitacion;
    float montoTotal;   // (diasEstadia * precioNoche) + Consumos Extras
    Fecha fechaPago;    // Estructura anidada de tiempo
    bool activo;        // true = Transacción válida, false = Anulada
};
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
    bool activo;
};
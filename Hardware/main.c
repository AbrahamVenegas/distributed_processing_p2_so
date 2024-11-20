#include "Biblioteca.h"
#include <stdio.h>

int main(){
    const char *portname = "/dev/ttyARDUINO0";
    int baud_rate = B9600;

    int serial_port = iniciarComunicacion(portname);
    if (serial_port < 0){
        return -1;
    }
    char read_buf[1024]; //Buffer para almacenar los datos recibidos

    int num_bytes = leerRespuesta(serial_port, read_buf, sizeof(read_buf)); //Leer del puerto serial
    if (num_bytes > 0){
        printf ("Recibido: %s\n", read_buf);
    }

    char test[] = "100011110100";
    enviarComando(serial_port, test);
    printf("ENVIADO PAPU");


    cerrarComunicacion(serial_port);

    return 0;
}
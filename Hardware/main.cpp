#include "Biblioteca.h"
#include <stdio.h>

int main(){
    const char *portname = "/dev/ttyACM0";
    int baud_rate = B9600;

    int serial_port = serial_open(portname, baud_rate);
    if (serial_port < 0){
        return -1;
    }
    char read_buf[1024]; //Buffer para almacenar los datos recibidos

    int num_bytes = serial_read(serial_port, read_buf, sizeof(read_buf)); //Leer del puerto serial
    if (num_bytes > 0){
        printf ("Recibido: %s\n", read_buf);
    }

    closePort(serial_port);

    return 0;
}
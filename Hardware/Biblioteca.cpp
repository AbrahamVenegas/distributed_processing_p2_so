/**
 * @file Biblioteca.cpp
 * @author Kendall M (kendallmc@estudiantec.cr)
 * @brief Esta biblioteca se encarga de implementar todas 
 * las funciones ncesarias para comunicarse con el arduino
 * @version 0.1
 * @date 2024-11-12
 * 
 * @copyright Copyright (c) 2024
 */
#include "Biblioteca.h"

int serial_open(const char *portname, int baud_rate){
    int serial_port = open(portname, O_RDWR | O_NOCTTY);
    if (serial_port == -1){
        perror("Error al abrir el puerto serial");
        return -1;
    }

    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(serial_port, &tty) != 0){
        perror("Error obteniendo los atributos del puerto serial");
        close(serial_port);
        return -1;
    }

    tty.c_cflag &= ~PARENB; // Sin paridad
    tty.c_cflag &= ~CSTOPB; // 1 bit de parada
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;     // 8 bits de datos
    tty.c_cflag &= ~CRTSCTS; // Sin control de hardware
    tty.c_cflag |= CREAD | CLOCAL; // Activar la lectura y sin control de línea

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    tty.c_lflag &= ~ICANON;  // Modo no canónico
    tty.c_lflag &= ~ECHO;    // Sin eco
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ISIG;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);  // Sin control de flujo por software
    tty.c_oflag &= ~OPOST;  // Sin procesamiento de salida

    tty.c_cc[VMIN] = 1;     // Leer al menos 1 byte
    tty.c_cc[VTIME] = 10;   // Timeout de 1 segundo

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        perror("Error configurando el puerto serial");
        close(serial_port);
        return -1;
    }
    return serial_port;    
}

int serial_read(int serial_port, char *buffer, size_t buffer_size){
    memset(buffer, 0, buffer_size);
    int num_bytes = read(serial_port, buffer, buffer_size-1);
    if (num_bytes < 0){
        perror("Eerror al ler del puerto serial");
        return -1;
    }

    return num_bytes;
}

int sendToArduino(int serial_port, const char *message){
    int num_bytes = write(serial_port, message, strlen(message));
    if (num_bytes < 0){
        perror("Error al enviar datos al puerto serial");
        return -1;
    }

    int delim_bytes = write(serial_port, "\n",1);
    if (delim_bytes < 0){
        perror("Error al enviar el delimitador");
        return -1;
    }

    return num_bytes + delim_bytes;
}

void closePort(int serial_port){
    close(serial_port);
}
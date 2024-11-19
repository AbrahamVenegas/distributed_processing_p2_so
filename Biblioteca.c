#include "Biblioteca.h"

#ifndef CRTSCTS
#define CRTSCTS 020000000000 // Definir manualmente si no está disponible
#endif

/**
 * Esta función inicia la comunicación con el Arduino.
 * @param puerto_serial Una cadena con el puerto donde está el Arduino.
 * @return Descriptor del archivo (fd) o -1 si hay un error.
 */
int iniciarComunicacion(const char *puerto_serial) {
    int fd = open(puerto_serial, O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("Error al abrir el puerto serial");
        return -1;
    }

    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        perror("Error obteniendo los atributos del puerto serial");
        close(fd);
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

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Error configurando el puerto serial");
        close(fd);
        return -1;
    }

    return fd;
}

/**
 * Esta función se encarga de leer la respuesta de la comunicación serial.
 * @param fd Descriptor del archivo.
 * @param buffer Cadena para almacenar la respuesta.
 * @param buffer_size Tamaño del buffer.
 * @return Número de bytes leídos o -1 si hay un error.
 */
int leerRespuesta(int fd, char *buffer, int buffer_size) {
    memset(buffer, 0, buffer_size);  // Limpiar buffer de respuesta

    size_t total_bytes = 0;

    while (total_bytes < buffer_size - 1) {
        int num_bytes = read(fd, buffer + total_bytes, 1);

        if (num_bytes < 0) {
            perror("Error al leer del puerto serial");
            return -1;
        }
        if (num_bytes == 0) break;

        total_bytes += num_bytes;

        if (buffer[total_bytes - 1] == '\n') break;
    }

    buffer[total_bytes] = '\0';

    return total_bytes;
}

/**
 * Esta función envía un comando por el puerto serial.
 * @param fd Descriptor del archivo.
 * @param comando Cadena con el comando a enviar al Arduino.
 */
void enviarComando(int fd, const char *comando) {
    // Añadir un salto de línea al comando si es necesario
    char comando_con_salto[100];
    snprintf(comando_con_salto, sizeof(comando_con_salto), "%s\n", comando);

    int n_written = write(fd, comando_con_salto, strlen(comando_con_salto));
    if (n_written < 0) {
        perror("Error enviando el comando al puerto serial");
    }
}

/**
 * Esta función cierra la comunicación serial.
 * @param fd Descriptor del archivo.
 */
void cerrarComunicacion(int fd) {
    close(fd);
}

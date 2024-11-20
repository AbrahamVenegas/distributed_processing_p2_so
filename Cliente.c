#include <stdio.h>
#include <stdlib.h>
#include "Biblioteca.h"

int main() {
    // Nombre del archivo
    const char *filename = "combinacion.txt";

    //Iniciar comunicacion con el arduino
    const char *portname = "/dev/ttyACM0";
    int serial_port = iniciarComunicacion(portname);

    //Buffer para la combinacion que se envió del arduino
    char read_buf[1024];

    //Variable para almacenar los bytes de la respuesta del arduino
    int num_bytes = leerRespuesta(serial_port, read_buf, sizeof(read_buf)); 

    // Abrir el archivo en modo escritura
    FILE *archivo = fopen(filename, "w");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    // Escribir la cadena en el archivo
    fprintf(archivo, "%s\n", read_buf);

    // Cerrar el archivo
    fclose(archivo);

    printf("Combinacion escrita exitosamente en el archivo '%s'.\n", filename);

    return EXIT_SUCCESS;
}

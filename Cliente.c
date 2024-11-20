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

    sleep(30);

    const char *resultao = "resultados.txt";

    FILE* result = fopen(resultao, "r");
    if (result == NULL){
        printf("No se encontro el archivo combinacion.txt \n");
    }
    char combi_str[1024];
    if (fgets(combi_str, sizeof(combi_str), result)!= NULL){
        combi_str[strcspn(combi_str, "\n")] = '\0';
        printf("Resultado leido: %s \n", combi_str);
        enviarComando(serial_port,combi_str);
    } else {
        printf("Error al leer el archivo\n");
    }

    // Cerrar el archivo
    fclose(result);
    cerrarComunicacion(serial_port);

    printf("Combinacion leida exitosamente del archivo '%s'.\n", filename);

    return EXIT_SUCCESS;
}

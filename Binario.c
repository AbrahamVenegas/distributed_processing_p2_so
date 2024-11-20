#include <stdio.h>
#include <stdlib.h>
#include "Biblioteca.h"

int main() {
    // Nombre del archivo
    const char *filename = "resultado.txt";

    //Iniciar comunicacion con el arduino
    const char *portname = "/dev/ttyACM0";
    int serial_port = iniciarComunicacion(portname);

    //Leer la combinación

    FILE* result = fopen(filename, "r");
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

    printf("Combinacion leida exitosamente del archivo '%s'.\n", filename);

    return EXIT_SUCCESS;
}
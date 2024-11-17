#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define DIRECTORY_PATH "combinaciones/" // Carpeta donde están los archivos

// Función para buscar el archivo correspondiente a la combinación
FILE* buscar_archivo_combinacion(const char* combinacion) {
    char archivo[256];
    snprintf(archivo, sizeof(archivo), "%s%s.txt", DIRECTORY_PATH, combinacion);

    FILE* f = fopen(archivo, "r");
    if (f == NULL) {
        printf("No se encontró el archivo para la combinación: %s\n", combinacion);
        return NULL;
    }

    return f;
}

void decimal_a_binario(int n) {
    if (n == 0) {
        printf("0\n");
        return;
    }

    char binario[32];
    int index = 0;

    while (n > 0) {
        binario[index++] = (n % 2) ? '1' : '0';
        n /= 2;
    }

    // Imprimir el número binario
    for (int i = index - 1; i >= 0; i--) {
        putchar(binario[i]);
    }
    printf("\n");
}

int main() {
    // Simulación de la combinación descifrada
    const char* combinacion_descifrada = "2413"; // Ejemplo de combinación descifrada

    // Busca el archivo correspondiente a la combinación
    FILE* archivo = buscar_archivo_combinacion(combinacion_descifrada);

    if (archivo != NULL) {
        printf("Archivo encontrado para la combinación %s. Leyendo contenido...\n", combinacion_descifrada);

        // Leer el contenido del archivo
        int numero_decimal;
        if (fscanf(archivo, "%d", &numero_decimal) == 1) {
            printf("Número decimal leído del archivo: %d\n", numero_decimal);

            decimal_a_binario(numero_decimal);
        } else {
            printf("Error al leer el número decimal del archivo.\n");
        }

        fclose(archivo);
    } else {
        printf("No se pudo encontrar el archivo correspondiente.\n");
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <openssl/aes.h>

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

// Función para convertir un número decimal a binario
void decimal_a_binario(int n, char *binario_resultado) {
    if (n == 0) {
        strcpy(binario_resultado, "0");
        return;
    }

    char binario[32];
    int index = 0;

    while (n > 0) {
        binario[index++] = (n % 2) ? '1' : '0';
        n /= 2;
    }
    binario[index] = '\0';

    // Revertir la cadena
    for (int i = 0; i < index; i++) {
        binario_resultado[i] = binario[index - i - 1];
    }
    binario_resultado[index] = '\0';
}

// Función para encriptar con AES
void encryptAES(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key) {
    AES_KEY encryptKey;
    AES_set_encrypt_key(key, 128, &encryptKey); // Llave de 128 bits
    AES_encrypt(plaintext, ciphertext, &encryptKey);
}

// Función para desencriptar con AES
void decryptAES(const unsigned char *ciphertext, unsigned char *decryptedtext, const unsigned char *key) {
    AES_KEY decryptKey;
    AES_set_decrypt_key(key, 128, &decryptKey); // Llave de 128 bits
    AES_decrypt(ciphertext, decryptedtext, &decryptKey);
}

int main() {
    // Llave para AES-128
    unsigned char key[16] = "1234567890123456";

    char combinacion[16];
    char binario[32];

    // Abrir el puerto serial
    FILE *serial = fopen("/dev/ttyUSB0", "r+"); // Ajustar puerto
    if (serial == NULL) {
        perror("Error abriendo el puerto serial");
        return 1;
    }

    printf("Esperando combinación desde el Arduino...\n");

    // Leer la combinación desde el Arduino
    if (fgets(combinacion, sizeof(combinacion), serial) == NULL) {
        printf("Error leyendo la combinación del Arduino.\n");
        fclose(serial);
        return 1;
    }

    // Eliminar caracteres de salto de línea
    combinacion[strcspn(combinacion, "\r\n")] = '\0';

    printf("Combinación recibida: %s\n", combinacion);

    // Buscar archivo basado en la combinación
    FILE* archivo = buscar_archivo_combinacion(combinacion);

    if (archivo != NULL) {
        printf("Archivo encontrado para la combinación %s. Leyendo contenido...\n", combinacion);

        // Leer el contenido del archivo
        int numero_decimal;
        if (fscanf(archivo, "%d", &numero_decimal) == 1) {
            printf("Número decimal leído del archivo: %d\n", numero_decimal);

            // Convertir el número decimal a binario
            decimal_a_binario(numero_decimal, binario);
            printf("Número binario: %s\n", binario);

            // Enviar el número binario de regreso al Arduino
            fprintf(serial, "%s\n", binario);
            fflush(serial); // Asegurar que se envíe
        } else {
            printf("Error al leer el número decimal del archivo.\n");
        }

        fclose(archivo);
    } else {
        printf("No se pudo encontrar el archivo correspondiente.\n");
        fprintf(serial, "ERROR\n");
        fflush(serial);
    }

    fclose(serial);
    return 0;
}

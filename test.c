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

    
    // Línea para setear manualmente la combinación fija
    const char* combinacion_predefinida = "2413";

    // Línea para ingresar la combinación por input
    char combinacion_input[16];

    // Alternar
    //#define USAR_COMBINACION_FIJA  // Descomentar para usar la combinación fija

    #ifdef USAR_COMBINACION_FIJA
        const char* combinacion = combinacion_predefinida;
    #else
        printf("Ingresa la combinación de 4 dígitos: ");
        scanf("%4s", combinacion_input);
        const char* combinacion = combinacion_input;
    #endif

    // Encriptar la combinación
    unsigned char plaintext[16] = {0};
    snprintf((char *)plaintext, sizeof(plaintext), "%s", combinacion);

    unsigned char ciphertext[16] = {0};
    unsigned char decryptedtext[16] = {0};

    encryptAES(plaintext, ciphertext, key);
    printf("Texto cifrado: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    // Desencriptar la combinación
    decryptAES(ciphertext, decryptedtext, key);
    printf("Texto desencriptado: %s\n", decryptedtext);

    // Buscar archivo basado en la combinación desencriptada
    FILE* archivo = buscar_archivo_combinacion((const char*)decryptedtext);

    if (archivo != NULL) {
        printf("Archivo encontrado para la combinación %s. Leyendo contenido...\n", decryptedtext);

        // Leer el contenido del archivo
        int numero_decimal;
        if (fscanf(archivo, "%d", &numero_decimal) == 1) {
            printf("Número decimal leído del archivo: %d\n", numero_decimal);

            // Convertir el número decimal a binario
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

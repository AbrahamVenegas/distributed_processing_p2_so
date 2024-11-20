#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <openssl/aes.h>
#include "Biblioteca.h"
#include <mpi.h>

#define DIRECTORY_PATH "combinaciones/" // Carpeta donde están los archivos

//Funcion para limpiar un problema con la cadena
void limpiar_cadena(char* cadena){
    char* p = cadena;
    while (*p) {
        if (*p == '\n' || *p == '\r'){
            *p = '\0';
        }
        p++;
    }
    
}

// Función para buscar el archivo correspondiente a la combinación
FILE* buscar_archivo_combinacion(const char* combinacion) {
    char archivo[256];
    char combinacion_limpia[256];
    strncpy(combinacion_limpia, combinacion, sizeof(combinacion_limpia));
    limpiar_cadena(combinacion_limpia);

    snprintf(archivo, sizeof(archivo), "combinaciones/%s.txt", combinacion_limpia);
    
    printf("Intentado abrir el archivo: %s \n", archivo);

    FILE* f = fopen(archivo, "r");
    if (f == NULL) {
        printf("No se encontró el archivo para la combinación: %s\n", combinacion);
        return NULL;
    }

    return f;
}

void decimal_a_binario_char(const char *decimal_str, char *binary_str, size_t binary_size) {
    int n = atoi(decimal_str); // Convierte la cadena decimal a entero
    if (n == 0) {
        snprintf(binary_str, binary_size, "0");
        return;
    }

    char temp[32]; // Búfer temporal para construir el binario
    int index = 0;

    while (n > 0) {
        temp[index++] = (n % 2) ? '1' : '0';
        n /= 2;
    }

    // Invierte los bits y guarda en binary_str
    int j = 0;
    for (int i = index - 1; i >= 0 && j < binary_size - 1; i--, j++) {
        binary_str[j] = temp[i];
    }
    binary_str[j] = '\0'; // Termina la cadena con '\0'
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

void intToBinaryString(int num, char *buffer, int size) {
    buffer[size - 1] = '\0'; // Fin de la cadena
    for (int i = size - 2; i >= 0; i--) {
        buffer[i] = (num % 2) + '0'; // Obtiene el bit menos significativo y lo convierte a carácter
        num /= 2;
    }
}

int main(int argc, char** argv){
    //Store an identifier for each of the parallel processes and the number of processes running in the cluster, respectively.
    int process_Rank, size_Of_Cluster;

    // Llave para AES-128
    unsigned char key[16] = "1234567890123456";
    char binaryString[1024];
    //Initializes the MPI environment. It takes in the addresses of the C++ command line arguments argc and argv.
    MPI_Init(&argc, &argv);

    //Returns the total size of the environment via quantity of processes. The function takes in the MPI environment,
    //and the memory address of an integer variable.
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);

    //Function returns the process ID of the processor that called the function. The function takes in the MPI 
    //environment, and the memory address of an integer variable.
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    //Leer la combinación

    FILE* combi = fopen("combinacion.txt", "r");
    if (combi == NULL){
        printf("No se encontro el archivo combinacion.txt \n");
    }
    char combi_str[16];
    if (fgets(combi_str, sizeof(combi_str), combi)!= NULL){
        combi_str[strcspn(combi_str, "\n")] = '\0';
        printf("COmbinacion leida: %s \n", combi_str);
    }

    // Encriptar la combinación
    unsigned char plaintext[16] = {0};
    snprintf((char *)plaintext, sizeof(plaintext), "%s", combi_str);

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

    // Broadcast of the combination to all the nodes
    MPI_Bcast(decryptedtext, 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    //Holds each process at a certain line of code until all processes have reached here
    MPI_Barrier(MPI_COMM_WORLD);

    // Buscar archivo basado en la combinación desencriptada
    FILE* archivo = buscar_archivo_combinacion((const char*)decryptedtext);

    if (archivo != NULL) {
    printf("Archivo encontrado para la combinación %s. Leyendo contenido...\n", decryptedtext);

    // Leer el contenido del archivo como cadena
    char decimal_str[16];
    if (fgets(decimal_str, sizeof(decimal_str), archivo) != NULL) {
        // Remueve el salto de línea si está presente
        decimal_str[strcspn(decimal_str, "\n")] = '\0';

        printf("Número decimal leído del archivo: %s\n", decimal_str);

        // Convertir la cadena decimal a binario
        decimal_a_binario_char(decimal_str, binaryString, sizeof(binaryString));
        printf("Número binario: %s\n", binaryString);

        // Enviar binario al result.txt
        printf("Binario escrito en el result.txt\n");
        MPI_Send(&binaryString, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        const char *filename = "resultados.txt";
        FILE *result = fopen(filename, "w");
        if (result == NULL){
            perror("Error al abrir el archivo \n");
            return EXIT_FAILURE;
        }
        //Escribir el binario en el archivo
        fprintf(result, "%s\n", binaryString);
         

    } else {
        printf("Error al leer el contenido del archivo.\n");
    }

    fclose(archivo);
} else {
    printf("No se pudo encontrar el archivo correspondiente.\n");
}

    //Cleans up the MPI environment and ends MPI communications.
    MPI_Finalize();
    
    return 0;
}

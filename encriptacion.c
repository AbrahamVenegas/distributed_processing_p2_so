#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>

void encryptAES(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key) {
    AES_KEY encryptKey;
    AES_set_encrypt_key(key, 128, &encryptKey); // Llave de 128 bits
    AES_encrypt(plaintext, ciphertext, &encryptKey);
}

void decryptAES(const unsigned char *ciphertext, unsigned char *decryptedtext, const unsigned char *key) {
    AES_KEY decryptKey;
    AES_set_decrypt_key(key, 128, &decryptKey); // Llave de 128 bits
    AES_decrypt(ciphertext, decryptedtext, &decryptKey);
}

int main() {
    // Define la llave (16 bytes para AES-128)
    unsigned char key[16] = "1234567890123456";

    // Valor numérico de 4 dígitos a encriptar
    int value = 1234;
    unsigned char plaintext[16] = {0}; // Bloque de 16 bytes inicializado en 0
    snprintf((char *)plaintext, sizeof(plaintext), "%04d", value); // Convierte el número a string de 4 dígitos

    // Buffers para el texto cifrado y descifrado
    unsigned char ciphertext[16] = {0};
    unsigned char decryptedtext[16] = {0};

    // Encriptar
    encryptAES(plaintext, ciphertext, key);
    printf("Texto cifrado: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", ciphertext[i]); // Imprime cada byte en formato hexadecimal
    }
    printf("\n");

    // Desencriptar
    decryptAES(ciphertext, decryptedtext, key);
    printf("Texto desencriptado: %s\n", decryptedtext);

    return 0;
}

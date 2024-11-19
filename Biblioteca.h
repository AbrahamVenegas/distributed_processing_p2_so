#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

// Prototipos de funciones
int iniciarComunicacion(const char *puerto_serial);
void enviarComando(int fd, const char *comando);
int leerRespuesta(int fd, char *buffer, int buffer_size);
void cerrarComunicacion(int fd);

#endif

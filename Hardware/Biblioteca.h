#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <termios.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


using namespace std;

int iniciarComunicacion(const char *puerto_serial);

void enviarComando(int fd, const char *comando);

int leerRespuesta(int fd, char *buffer, int buffer_size);

void cerrarComunicacion(int fd);


#endif
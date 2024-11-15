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

int serial_open(const char *portname, int baud_rate);

void closePort(int serial_port);

int sendToArduino(int serial_port, const char *message);

int serial_read(int serial_port, char *buffer, size_t buffer_size);


#endif
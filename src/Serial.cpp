/*
 *    @author  : Maxime Chretien
 *    @mail    : mchretien@linuxmail.org
 *    @project : TUT Infobus
 *    @summary : Implementation de l'objet Serial
 *    @version : v1.1
 */

#include "Serial.h"

#include <cstring>
#include <errno.h> // Error integer and strerror() function
#include <fcntl.h> // Contains file controls like O_RDWR
#include <iostream>
#include <unistd.h> // write(), read(), close()

Serial::Serial(const char* port, int baud): _port(port), _baud(baud) {
	_serial = open(_port, O_RDWR);

	if(_serial < 0)
		throw std::runtime_error(strerror(errno));

	if(tcgetattr(_serial, &_tty) != 0) {
		std::string error = strerror(errno);

		cleanExit();

		throw std::runtime_error(error);
	}


	// Serial settings
	_tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity
	_tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication
	_tty.c_cflag &= ~CSIZE; // Clear all the size bits, then use one of the statements below
	_tty.c_cflag |= CS8;	// 8 bits per byte
	_tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control
	_tty.c_cflag |= CREAD | CLOCAL;	// Turn on READ & ignore ctrl lines (CLOCAL = 1)

	_tty.c_lflag &= ~ICANON;
	_tty.c_lflag &= ~ECHO; // Disable echo
	_tty.c_lflag &= ~ECHOE; // Disable erasure
	_tty.c_lflag &= ~ECHONL; // Disable new-line echo
	_tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

	_tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	_tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
	_tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	_tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

	_tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	_tty.c_cc[VMIN] = 0;

	// Set in/out baud rate to be 9600
	cfsetispeed(&_tty, _baud);
	cfsetospeed(&_tty, _baud);

	// Save tty settings, also checking for error
	if (tcsetattr(_serial, TCSANOW, &_tty) != 0) {
		std::string error = strerror(errno);

		cleanExit();

		throw std::runtime_error(error);
	}
}

Serial::~Serial() {
	cleanExit();
}

void Serial::cleanExit() {
	close(_serial);
}

void Serial::send(const uint8_t* text, size_t size) {
	write(_serial, text, size);
}

int Serial::receive(uint8_t* text, size_t size) {
	return read(_serial, text, size);
}

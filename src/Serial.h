/*
 *    @author  : Maxime Chretien (MixLeNain)
 *    @mail    : mchretien@linuxmail.org
 *    @project : TUT Infobus
 *    @summary : Definition de l'objet Girouette
 *    @version : v1.0
 */

#ifndef SERIAL_H
#define SERIAL_H

#include <cstdint>
#include <cstring>
#include <termios.h> // Contains POSIX terminal control definitions

class Serial {

public:
	Serial(const char* port, int baud);
	~Serial();

	void send(const uint8_t* text, size_t size);
	int receive(uint8_t* text, size_t size);

private:
	void cleanExit();

	const char* _port;
	int _baud;
	int _serial;
	struct termios _tty;

};

#endif

/*
 *    @author  : Maxime Chretien
 *    @mail    : mchretien@linuxmail.org
 *    @project : TUT Infobus
 *    @summary : Definition de l'objet Girouette
 *    @version : v2.1
 */

#ifndef GIROUETTE_H
#define GIROUETTE_H

#include "Serial.h"

#include <cstdint>
#include <string>

class Girouette {
public:
	Girouette(uint8_t addr, std::string serial = "/dev/ttyUSB0", uint8_t baud = B9600);
	~Girouette();

	bool ping();
	void clear();
	bool sendMsg(std::string text, uint8_t type, uint8_t typeArg1, uint8_t typeArg2, uint8_t duree, bool secondary);
	void sendColors(uint8_t (*colors)[3]);
	void adjPower();

private:
	bool getAnswer();

	Serial* _serial;
	uint8_t _addr;
};

#endif

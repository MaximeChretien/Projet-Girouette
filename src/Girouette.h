/*
 *    @author  : Maxime Chretien (MixLeNain)
 *    @mail    : mchretien@linuxmail.org
 *    @project : TUT Infobus
 *    @summary : Definition de l'objet Girouette
 *    @version : v1.0
 */

#ifndef GIROUETTE_H
#define GIROUETTE_H

#include "Serial.h"

#include <cstdint>

#define BUFF_SIZE 512

class Girouette {
public:
	Girouette(uint8_t addr);
	~Girouette();

	bool ping();
	void clear();
	bool sendMsg(const char* text, uint8_t type, uint8_t typeArg1, uint8_t typeArg2, uint8_t duree, bool secondary);
	void sendColors(uint8_t colors[][3]);
	void adjPower();

private:
	bool getAnswer();

	Serial* _serial;
	uint8_t _addr;
};

#endif

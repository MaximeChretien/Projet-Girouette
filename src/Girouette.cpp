/*
 *    @author  : Maxime Chretien (MixLeNain)
 *    @mail    : mchretien@linuxmail.org
 *    @project : TUT Infobus
 *    @summary : Implementation de l'objet Girouette
 *    @version : v2.0
 */

#include "Girouette.h"

#include <cstring>
#include <iostream>
#include <vector>

using std::string;
using std::vector;

Girouette::Girouette(uint8_t addr): _addr(addr) {
	_serial = new Serial("/dev/ttyUSB0", B9600);
}

Girouette::~Girouette() {
	delete _serial;
}

bool Girouette::ping() {
	const uint8_t msg[] = {0x00, _addr, 0x00, 0x01, 0x41, 0x41};

	_serial->send(msg, sizeof(msg));

	return getAnswer();
}

void Girouette::clear() {
	const uint8_t msg[] = {0x00, 0x7e};

	_serial->send(msg, sizeof(msg));
}

bool Girouette::sendMsg(string text, uint8_t type, uint8_t typeArg1, uint8_t typeArg2, uint8_t duree, bool secondary) {
	vector<uint8_t> msg = {0x00, _addr, 0, 0, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, text.size()};

	msg.insert(msg.end(), text.begin(), text.end());

	if(type == 0x00)
		msg.insert(msg.end(), {type, duree});
	else
		msg.insert(msg.end(), {type, typeArg1, typeArg2, duree});

	uint16_t dataSize = msg.size()-4;
	msg.at(2) = (uint8_t)(dataSize >> 8);
	msg.at(3) = (uint8_t)dataSize;

	uint8_t checksum = 0;
	for (auto it = (msg.begin() + 4); it != msg.end(); ++it)
		checksum ^= *it;

	msg.push_back(checksum);

	std::vector<uint8_t>::size_type size = msg.size();
	for (std::vector<uint8_t>::size_type i = 4; i < size; ++i) {
		if(msg.at(i) == 0x00) {
			msg.insert(msg.begin() +i, 0x00);
			++size;
			++i;
		}
	}

	std::cout << std::endl;
	if(secondary) {
		msg.insert(msg.begin()+1, 0xF0);
	}

	_serial->send(msg.data(), msg.size());

	return getAnswer();
}

void Girouette::sendColors(uint8_t colors[][3]) {
	uint8_t msg[BUFF_SIZE];
	size_t size = sizeof(colors);
	uint8_t msgBase[] = {0x00, 0xFF, (uint8_t)(size >> 8), (uint8_t)size};
	uint8_t checksum = 0;

	for(size_t i = 0; i < size; i++) {
		checksum ^= colors[i/3][i%3];
	}

	size_t pos = 0;
	for(size_t i = 0; i < sizeof(msgBase) && pos < BUFF_SIZE; i++, pos++) {
		msg[pos] = msgBase[i];
	}

	for(size_t i = 0; i < size && pos < BUFF_SIZE; i++, pos++) {
		uint8_t b = colors[i/3][i%3];

		msg[pos] = b;

		// Send 0x00 two times
		if(b == 0x00) {
			pos++;
			msg[pos] = b;
		}
	}


	if(pos >= BUFF_SIZE-2)
		throw std::runtime_error("Buffer overflow");

	msg[pos] = checksum;
	// Send 0x00 two times
	if(checksum == 0x00) {
		pos++;
		msg[pos] = checksum;
	}

	_serial->send((const uint8_t*)msg, pos+1);
}

void Girouette::adjPower() {
	const uint8_t msg[] = {0x00, 0xee, 0x00, 0xaa, 0xaa};

	_serial->send(msg, sizeof(msg));
}

bool Girouette::getAnswer() {
	uint8_t text[6] = {0};
	uint8_t i = 0;
	uint8_t timeout = 0;

	while(i < 6 && timeout < 10) {
		i += _serial->receive(text, 6);
		timeout++;
	}

	if(text[0] == 0x00 && text[1] == 0x01 && text[2] == 0x00 && text[3] == 0x01 && text[4] == 0xFF && text[5] == 0xFF)
		return true;

	return false;
}


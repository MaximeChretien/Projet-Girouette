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

using std::begin;
using std::end;
using std::string;
using std::vector;

Girouette::Girouette(uint8_t addr, string serial, uint8_t baud): _addr(addr) {
	_serial = new Serial(serial.c_str(), baud);
}

Girouette::~Girouette() {
	delete _serial;
}

bool Girouette::ping() {
	// Send ping message
	const uint8_t msg[] = {0x00, _addr, 0x00, 0x01, 0x41, 0x41};

	_serial->send(msg, sizeof(msg));

	// Check answer
	return getAnswer();
}

void Girouette::clear() {
	// Send clear message
	const uint8_t msg[] = {0x00, 0x7e};

	_serial->send(msg, sizeof(msg));
}

bool Girouette::sendMsg(string text, uint8_t type, uint8_t typeArg1, uint8_t typeArg2, uint8_t duree, bool secondary) {
	vector<uint8_t> msg = {0x00, _addr, 0, 0, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, text.size()};

	// Add text to vector
	msg.insert(msg.end(), text.begin(), text.end());

	// Add parameters depending on the display mode
	if(type == 0x00)
		msg.insert(msg.end(), {type, duree});
	else
		msg.insert(msg.end(), {type, typeArg1, typeArg2, duree});

	// Compute data size
	uint16_t dataSize = msg.size()-4;
	msg.at(2) = (uint8_t)(dataSize >> 8);
	msg.at(3) = (uint8_t)dataSize;

	// Compute checksum
	uint8_t checksum = 0;
	for (auto it = (msg.begin() + 4); it != msg.end(); ++it)
		checksum ^= *it;

	msg.push_back(checksum);

	// Double all 0x00 in the message
	std::vector<uint8_t>::size_type size = msg.size();
	for (std::vector<uint8_t>::size_type i = 4; i < size; ++i) {
		if(msg.at(i) == 0x00) {
			msg.insert(msg.begin() +i, 0x00);
			++size;
			++i;
		}
	}

	// Insert secondary message identifier if needed
	if(secondary) {
		msg.insert(msg.begin()+1, 0xF0);
	}

	// Send message
	_serial->send(msg.data(), msg.size());

	// Check return value
	return getAnswer();
}

void Girouette::sendColors(uint8_t colors[][3]) {
	size_t dataSize = sizeof(colors);
	vector<uint8_t> msg = {0x00, 0xFF, (uint8_t)(dataSize >> 8), (uint8_t)dataSize};

	// Insert colors into vector
	for(size_t i = 0; i < dataSize/3; ++i)
		msg.insert(msg.end(), begin(colors[i]), end(colors[i]));

	// Compute checksum
	uint8_t checksum = 0;
	for(size_t i = 0; i < dataSize; i++) {
		checksum ^= colors[i/3][i%3];
	}
	msg.push_back(checksum);

	// Double all 0x00 in the message
	std::vector<uint8_t>::size_type size = msg.size();
	for (std::vector<uint8_t>::size_type i = 4; i < size; ++i) {
		if(msg.at(i) == 0x00) {
			msg.insert(msg.begin() +i, 0x00);
			++size;
			++i;
		}
	}

	_serial->send(msg.data(), msg.size());
}

void Girouette::adjPower() {
	// Send adjusting power message
	const uint8_t msg[] = {0x00, 0xee, 0x00, 0xaa, 0xaa};

	_serial->send(msg, sizeof(msg));
}

bool Girouette::getAnswer() {
	uint8_t text[6] = {0};
	uint8_t i = 0;
	uint8_t timeout = 0;

	// Wait for answer
	while(i < 6 && timeout < 10) {
		i += _serial->receive(text, 6);
		timeout++;
	}

	// Check answer
	if(text[0] == 0x00 && text[1] == 0x01 && text[2] == 0x00 && text[3] == 0x01 && text[4] == 0xFF && text[5] == 0xFF)
		return true;

	return false;
}


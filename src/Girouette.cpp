
#include "Girouette.h"

#include <cstring>
#include <iostream>

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

bool Girouette::sendMsg(const char* text, uint8_t type, uint8_t typeArg1, uint8_t typeArg2, uint8_t duree) {
	uint8_t msg[BUFF_SIZE];
	uint8_t data[BUFF_SIZE];
	size_t textSize = strlen(text);
	size_t dataSize = 0;
	uint8_t msgBase[] = {0x00, _addr, 0, 0};
	uint8_t dataBase[] = {0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, (uint8_t)textSize};
	uint8_t dataEnd[4] {type, typeArg1, typeArg2, duree};
	size_t dataEndSize = 4;
	uint8_t checksum = 0;

	// If basic message, fix dataEnd
	if(type == 0x00) {
		dataEnd[1] = duree;
		dataEndSize = 2;
	}

	dataSize = sizeof(dataBase) + textSize + dataEndSize;

	// Build data array
	size_t posData = 0;
	for(size_t i = 0; i < sizeof(dataBase) && posData < BUFF_SIZE; i++, posData++) {
		data[posData] = dataBase[i];
	}

	for(size_t i = 0; i < textSize && posData < BUFF_SIZE; i++, posData++) {
		data[posData] = text[i];
	}

	for(size_t i = 0; i < dataEndSize && posData < BUFF_SIZE; i++, posData++) {
		data[posData] = dataEnd[i];
	}

	if (dataSize != posData) {
		// If that occurs, the code above here is not good
		throw std::runtime_error("Error computing data");
	}

	// Compute checksum
	for(size_t i = 0; i < dataSize; i++) {
		checksum ^= data[i];
	}

	// Update data size
	msgBase[2] = (uint8_t)(dataSize >> 8);
	msgBase[3] = (uint8_t)dataSize;

	// Build message array
	size_t pos = 0;
	for(size_t i = 0; i < sizeof(msgBase) && pos < BUFF_SIZE; i++, pos++) {
		msg[pos] = msgBase[i];
	}

	for(uint i = 0; i < dataSize && pos < BUFF_SIZE; i++, pos++) {
		uint8_t b = data[i];

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

	return getAnswer();
}

bool Girouette::sendSpecialMsg(const char* text) {

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
	//TODO
	return true;
}


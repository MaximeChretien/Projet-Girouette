#include <iostream>

#include "Girouette.h"

using namespace std;

uint8_t colors[][3] = {
	{0, 0, 0},
	{100,100,100},
	{255,255,255}
};

int main() {
	bool ok;
	setlocale(LC_ALL, "CP437");
	Girouette* gir = new Girouette(0x02);

	gir->clear();
	gir->sendColors(colors);
	ok = gir->sendMsg("abcdefghijklmnopqrstuvwxyz", 0x01, 0x30, 0x28, 0x01);

	cout << ok << endl;

	delete gir;
	exit(0);
}

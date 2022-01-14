/*
 *    @author  : Maxime Chretien (MixLeNain)
 *    @mail    : mchretien@linuxmail.org
 *    @project : TUT Infobus
 *    @summary : Main
 *    @version : v1.0
 */

#include <iostream>

#include "Girouette.h"

using namespace std;

uint8_t colors[][3] = {
	{0, 0, 0},
	{100,100,100},
	{255,255,255}
};

int main() {
	bool ok = false;
	setlocale(LC_ALL, "CP437");
	Girouette* gir = new Girouette(0x02);

	gir->clear();
	gir->sendColors(colors);

	do {
		ok = gir->sendMsg("Hello World !", 0x01, 0x08, 0x28, 0x03, false);
		cout << ok << endl;
	} while(!ok);

	do {
		ok = gir->sendMsg("Coucou toi !", 0x01, 0x04, 0x28, 0x01, true);
		cout << ok << endl;
	} while (!ok);

	delete gir;
	exit(0);
}

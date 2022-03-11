/*
 *    @author  : Maxime Chretien
 *    @mail    : mchretien@linuxmail.org
 *    @project : TUT Infobus
 *    @summary : Main
 *    @version : v2.1
 */

#include <iostream>
#include <limits>

#include "Girouette.h"

using namespace std;

uint8_t colors[][3] = {
	{0, 0, 0},
	{100,100,100},
	{255,255,255}
};

int main() {
	bool ok = false;
	int choix;
	bool exitBool = false;
	bool secondary = false;
	string msg = "";
	setlocale(LC_ALL, "CP437");
	Girouette* gir = new Girouette(0x02, "/dev/ttyUSB0", B9600);

	while(!exitBool) {
		cout << "Menu : " << endl;
		cout << "1. Envoyer un msg principal" << endl;
		cout << "2. Envoyer un msg secondaire" << endl;
		cout << "3. Remise a zero" << endl;
		cout << "4. Envoi couleurs" << endl;
		cout << "5. Ping" << endl;
		cout << "6. Quitter" << endl;
		cout << "Choix : ";
		cin >> choix;

		cin.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );

		switch(choix) {
			case 2:
				secondary = true;
			case 1:
				cout << "Message : ";
				getline(cin, msg);

				do {
					ok = gir->sendMsg(msg.c_str(), 0x01, 0x08, 0x28, 0x01, secondary);
				} while(!ok);
				cout << "Envoi réussi !" << endl;
				secondary = false;
				break;
			case 3:
				gir->clear();
				cout << "Reset !" << endl;
				break;
			case 4:
				gir->sendColors(colors);
				cout << "Envoi réussi !" << endl;
				break;
			case 5:
				ok = gir->ping();
				cout << "Réponse : " << ok << endl;
				break;
			case 6:
				exitBool = true;
				break;
			default:
				cout << "Choix invalide !" << endl;
		}

		cout << endl;

	}
	delete gir;
	exit(0);
}

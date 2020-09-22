#include "header.h"
using namespace std;

int main() {
	string f = "";
	f = format(f, T_CD);
	read_formatted(f);
	send_(f);

	//lendo o nack:
	string r = receive_();
	read_formatted(r);
	return 0;
}

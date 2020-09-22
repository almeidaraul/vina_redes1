#include "header.h"
using namespace std;

char a[] = "lo";
int c = ConexaoRawSocket(a);

int main() {
	int k = 1;
	string f;
	while (k < 20) {
		f = _itos(k, 2); //f = ""; funciona perfeitamente
		f = format(f, T_CD);
		//read_formatted(f);
		send_(f);
		k++;
	}
	return 0;
}

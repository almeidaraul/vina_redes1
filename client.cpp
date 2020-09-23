#include "header.h"
using namespace std;

char a[] = "lo";
int c = ConexaoRawSocket(a);

int main() {
	int k = 0;
	string f, r;
	while (k < 31) {
		f = _itos(k+1, 2); //f = ""; funciona perfeitamente
		f = format(f, T_CD);
		send_(f);

		f = receive_();
		k = stoi(get_data(f, get_size(f)+8));
		cout << k << endl;
	}
	return 0;
}

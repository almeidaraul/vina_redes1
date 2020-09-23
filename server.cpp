#include "header.h"
using namespace std;

char a[] = "lo";
int c = ConexaoRawSocket(a);

int main() {
	int k = 1;
	string r;
	while (k < 30) {
		r = receive_();
		k = stoi(get_data(r, get_size(r)+8));
		cout << k << endl;

		r = _itos(k+1, 2);
		r = format(r, T_CD);
		send_(r);
	}
	return 0;
}

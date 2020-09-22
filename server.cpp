#include "header.h"
using namespace std;

char a[] = "lo";
int c = ConexaoRawSocket(a);

int main() {
	int k = 1;
	string r;
	while (k < 20) {
		k++;
		r = receive_();
		read_formatted(r);
	}
	return 0;
}

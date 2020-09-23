#include "header.h"
using namespace std;

char a[] = "lo";
int c = ConexaoRawSocket(a);

int main() {
	string r;
	int type;
	string args;
	while (true) {
		r = receive_();
		type = get_type(r);
		args = get_data(r, get_size(r)+8);
		if (type == T_LINHA || type == T_LINHAS || type == T_EDIT)
			cout << "hmmm" << endl;
		cout << type << endl;
		cout << args << endl;
	}
	return 0;
}

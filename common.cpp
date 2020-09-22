#include "header.h"
using namespace std;

void send_(string s) {
	char a[] = "lo";
	int c = ConexaoRawSocket(a);

	while (s.length() < 16) s += " ";
	const char *m = s.c_str();
	cout << "sending string of size " << s.length() << ": " << s << endl;

	write(c, m, strlen(m));
}

string receive_() {
	char a[] = "lo";
	int c = ConexaoRawSocket(a);

	char buffer[1024] = {0};
	int valread;
	read(c, buffer, 1024);

	string buffer_string(buffer);
	return buffer_string;
}

string format(string s) {
	string f = "~";
	unsigned char sz = s.length();
	unsigned char sequence = 16;
	unsigned char type = 2;
	f.push_back(sz);
	f.push_back(sequence);
	f.push_back(type);
	f += s;
	unsigned char parity = '^';
	f.push_back(parity);
	
	return f;
}

void read_formatted(string s) {
	cout << "String: " << s << endl;
	cout << "String size/length: " << s.size() << "/" << s.length() << endl;
	cout << "Begin marker: " << s[0] << endl;
	cout << "Size: " << int(s[1]) << endl;
	cout << "Sequence: " << int(s[2]) << endl;
	cout << "Type: " << int(s[3]) << endl;

	cout << "Data: ";
	for (int i = 4; i < 4+s[1]; i++)
		cout << s[i];
	cout << endl;

	cout << "Parity: " << int(s[4+s[1]]) << endl;
}

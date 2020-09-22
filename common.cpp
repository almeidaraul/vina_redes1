#include "header.h"
using namespace std;

int server_sequence = 0;
int client_sequence = 0;

void send_(string s) {
	char a[] = "lo";
	int c = ConexaoRawSocket(a);

	while (s.length() < 22) s += " ";
	const char *m = s.c_str();

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
	string sz = "05";
	string sequence = "066";
	string type = "07";
	f += sz + sequence + type + s;
	string parity = "880";
	f += parity;
	
	return f;
}

void read_formatted(string s) {
	cout << "String: " << s << endl;
	cout << "String size/length: " << s.size() << "/" << s.length() << endl;
	cout << "Begin marker: " << s[0] << endl;

	string size_string = "";
	size_string.push_back(s[1]);
	size_string.push_back(s[2]);
	cout << "Size: " << stoi(size_string) << endl;

	string sequence_string = "";
	sequence_string.push_back(s[3]);
	sequence_string.push_back(s[4]);
	sequence_string.push_back(s[5]);
	cout << "Sequence: " << stoi(sequence_string) << endl;

	string type_string = "";
	type_string.push_back(s[6]);
	type_string.push_back(s[7]);
	cout << "Type: " << stoi(type_string) << endl;

	int end_of_data = stoi(size_string)+8;

	cout << "Data: ";
	for (int i = 8; i < end_of_data; i++)
		cout << s[i];
	cout << endl;

	string parity_string = "";
	parity_string.push_back(s[end_of_data]);
	parity_string.push_back(s[end_of_data+1]);
	parity_string.push_back(s[end_of_data+2]);
	cout << "Parity: " << stoi(parity_string) << endl;
}

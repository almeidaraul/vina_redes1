#include "header.h"
using namespace std;

int my_seq = 0;
int their_seq = 0;

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

	string received(buffer);

	if (get_sequence(received) != their_seq) {
		send_status("nack");
	}
	their_seq = (their_seq+1)%256;

	return received;
}

void send_status(string s) {
	int type = s == "nack" ? T_NACK : T_ACK;
	string m = format(s, type);

	char a[] = "lo";
	int c = ConexaoRawSocket(a);

	while (m.length() < 22) m += " ";
	const char *msg = m.c_str();

	write(c, msg, strlen(msg));
}

string format(string s, int type) {
	string f = "~";
	string sz = _itos(s.length(), 2); //"05";

	string sequence = _itos(my_seq, 3); //"066";
	my_seq = (my_seq+1)%256;

	string type_string = _itos(type, 2); //"07";
	f += sz + sequence + type_string + s;
	string parity = _itos(880, 3); //"880";
	f += parity;
	
	return f;
}

void read_formatted(string s) {
	cout << "String: " << s << endl;
	cout << "String size/length: " << s.size() << "/" << s.length() << endl;
	cout << "Begin marker: " << s[0] << endl;

	cout << "Size: " << get_size(s) << endl;

	cout << "Sequence: " << get_sequence(s) << endl;

	cout << "Type: " << get_type(s) << endl;

	int end_of_data = get_size(s)+8;

	cout << "Data: ";
	for (int i = 8; i < end_of_data; i++)
		cout << s[i];
	cout << endl;

	cout << "Parity: " << get_parity(s, end_of_data) << endl;
}

int get_parity(string m, int end_of_data) {
	string parity_string = "";
	parity_string.push_back(m[end_of_data]);
	parity_string.push_back(m[end_of_data+1]);
	parity_string.push_back(m[end_of_data+2]);
	return stoi(parity_string);
}

int get_type(string m) {
	string type_string = "";
	type_string.push_back(m[6]);
	type_string.push_back(m[7]);
	return stoi(type_string);
}

int get_sequence(string m) {
	string sequence_string = "";
	sequence_string.push_back(m[3]);
	sequence_string.push_back(m[4]);
	sequence_string.push_back(m[5]);
	return stoi(sequence_string);
}

int get_size(string m) {
	string size_string = "";
	size_string.push_back(m[1]);
	size_string.push_back(m[2]);
	return stoi(size_string);
}

string _itos(int val, int sz) {
	string r;
	char centena, dezena, unidade;
	centena = (val/100)+'0';
	dezena = ((val/10)%10)+'0';
	unidade = (val%10)+'0';

	if (sz == 2) {
		r = "00";
		r[0] = dezena;
		r[1] = unidade;
	} else {
		r = "000";
		r[0] = centena;
		r[1] = dezena;
		r[2] = unidade;
	}
	return r;
}

#include "header.h"
using namespace std;

vector<string> split(string& split_me) {
	string temp = "";
	vector<string> return_me;
	for (int i = 0; i < split_me.length(); i++) {
		if ((split_me[i] == ' ') && (temp.length())) {
			return_me.push_back(temp);
			temp = "";
		}
		else
			temp.push_back(split_me[i]);
	}
	if (temp != "")
		return_me.push_back(temp);
	return return_me;
}

string process_for_edit(string& s) {
	int i = 0;
	int seen_spaces = 0;
	while (seen_spaces < 2) {
		if (s[i] == ' ') seen_spaces++;
		i++;
	}
	while (s[i] != '\"') i++;
	i++;
	string t = "";
	while (i < s.length()-1) t += s[i++];
	return t;
}

string msg(unsigned char sequence, unsigned int type, string data) {
	/*
	*	generates message inside protocol rules with given data
	* 
	* NOTES ON PARAMS
	* data should have 0 to 15 bytes
	*/
	string msg = "~";
	unsigned char sz = data.length();
	/*
	unsigned char first_byte = (sz << 4) + (sequence >> 4);
	unsigned char second_byte = ((sequence & 15) << 4) + type;
	*/
	msg.push_back(sz);
	msg.push_back(sequence);
	msg.push_back(type);
	//msg.push_back(first_byte);
	//msg.push_back(second_byte);
	msg += data;

	unsigned char parity = 0;
	for (int i = 1; i < msg.length(); i++) {
		parity ^= msg[i];
	}
	msg.push_back(parity);
	return msg;
}

string cut_first_word(string s) {
	int i = 0;
	while (s[i] == ' ') i++;
	while ((i < s.length()) && (s[i] != ' ')) i++;
	i++;
	string r = "";
	while (i < s.length()) {
		r.push_back(s[i]);
		i++;
	}
	return r;
}

void send_message(string msg) {
	//manda mensagem enquanto receber nack
	char a[] = "lo";
	int c = ConexaoRawSocket(a);
	while (msg.length() < 16) msg += " ";
	/*
	for (int i = 0; i < msg.length(); i++)
		cout << msg[i] << '.' << endl;
	*/
	const char *s = msg.c_str();
	string response;
	do {
		write(c, s, strlen(s));
		cout << "vou pegar response, se liga:" << endl;
		response = receive_message();
		cout << "peguei :)" << endl;
	} while (get_type(response) == 9);
}

string receive_message() {
	//manda ack quando recebe mensagem
	char a[] = "lo";
	int c = ConexaoRawSocket(a);

	char buffer[1024] = {0};
	int valread = 0;
	read(c, buffer, 1024);
	valread = read(c, buffer, 1024);
	send_status("ack");

	string message(buffer);
	return message;
}

void send_in_pieces(unsigned char type, string data) {
	vector<string> split_data;
	if (data.length() > 15) {
		int i = 0;
		while (i < data.length()) {
			string temp = "";
			for (int j = i; (j < i+15) && (j < data.length()); j++)
				temp.push_back(data[j]);
			split_data.push_back(temp);
			i += 15;
		}
	} else {
		split_data.push_back(data);
	}

	for (auto piece : split_data) {
		string message = msg(0xff, type, piece);
		send_message(message);
	}
	string message = msg(0xff, 13, "");
	send_message(message);
}

vector<string> receive_in_pieces() {
	vector<string> split_data;
	string received = receive_message();
	unsigned char type = get_type(received);
	while (type != 13) {
		split_data.push_back(received);
		received = receive_message();
		type = get_type(received);
	}
}

unsigned char get_type(string msg) {
	return (msg[3]);
}

void send_status(string status) {
	char a[] = "lo";
	int c = ConexaoRawSocket(a);
	unsigned int st = (status == "ack" ? 8 : 9);
	string _msg = msg(0xff, st, "");
	while (_msg.length() < 16) _msg += " ";
	const char *s = _msg.c_str();
	write(c, s, strlen(s));
}

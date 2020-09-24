#include "header.h"
using namespace std;

int my_seq = 0;
int their_seq = 0;

void send_(string s) {
	while (s.length() < 22) s += " ";
	const char *m = s.c_str();

	do {
		write(c, m, strlen(m));
	} while (receive_status() == "nack");
}

string receive_() {
	char buffer[1024] = {0};
	bool keep_receiving = true;
	const clock_t begin_time = clock();
	while (keep_receiving) {
		recv(c, buffer, 1024, 0);
		bool buffer_empty =  !strlen(buffer);
		bool old_seq = get_sequence(string(buffer)) != their_seq;
		bool time_ok = float(clock() - begin_time)/CLOCKS_PER_SEC < 5;
		if (!time_ok) {
			send_status("nack");
			return receive_();
		}
		keep_receiving = time_ok && (buffer_empty || old_seq);
	}

	string received(buffer);
	
	bool sequence_ok = get_sequence(received) == their_seq;
	bool parity_ok = parity(received) == get_parity(received, get_size(received)+8);
	bool type_ok = (get_type(received) != T_ACK) && (get_type(received) != T_NACK);

	if (!sequence_ok || !parity_ok) {
		cout << "seq recebida: " << get_sequence(received) << ", esperada: " << their_seq << endl;
		cout << "erro em " << received << endl;
		cout << "s ok: " << sequence_ok << ", p ok: " << parity_ok << endl;
		send_status("nack");
		return receive_();
	}
	if (!type_ok)
		return receive_();
	their_seq = (their_seq+1)%256;
	send_status("ack");
	return received;
}

void send_status(string s) {
	int type = s == "nack" ? T_NACK : T_ACK;
	string m = format(s, type);

	while (m.length() < 22) m += " ";
	const char *msg = m.c_str();

	write(c, msg, strlen(msg));
	//my_seq = (my_seq+1)%256;
}

string receive_status() {
	char buffer[1024] = {0};
	bool keep_receiving = true;
	bool time_ok = true;
	const clock_t begin_time = clock();
	while (keep_receiving && time_ok) {
		recv(c, buffer, 1024, 0);
		int received_type = get_type(string(buffer));
		keep_receiving = !strlen(buffer) || ((received_type != T_ACK) && (received_type != T_NACK));
		if (float(clock()-begin_time)/CLOCKS_PER_SEC > 5)
			time_ok = false;
	}
	if (!time_ok) return "nack";
	string received(buffer);
	their_seq = (their_seq+1)%256;

	int type = get_type(received);
	return (type == T_ACK ? "ack" : "nack");
}

string format(string s, int type) {
	string f = "~";
	string sz = _itos(s.length(), 2); //"05";

	string sequence = _itos(my_seq, 3); //"066";
	my_seq = (my_seq+1)%256;

	string type_string = _itos(type, 2); //"07";
	f += sz + sequence + type_string + s;
	f.push_back('p');
	int parity_calc = parity(f);
	string parity = _itos(parity_calc, 3); //"880";
	f.pop_back();
	f += parity;
	
	return f;
}

int parity(string s) {
	unsigned char p = 0;
	for (int i = 1; i < 8+get_size(s)-1; i++)
		p ^= s[i];
	return int(p);
}

void read_formatted(string s) {
	cout << "String: " << s << endl;
	cout << "String size/length: " << s.size() << "/" << s.length() << endl;
	cout << "Begin marker: " << s[0] << endl;

	cout << "Size: " << get_size(s) << endl;

	cout << "Sequence: " << get_sequence(s) << endl;

	cout << "Type: " << get_type(s) << endl;

	int end_of_data = get_size(s)+8;

	cout << "Data: " << get_data(s, end_of_data) << endl;

	cout << "Parity: " << get_parity(s, end_of_data) << endl;
}

string get_data(string s, int end_of_data) {
	string m = "";
	for (int i = 8; i < end_of_data; i++)
		m.push_back(s[i]);
	return m;
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

vector<string> divide(string s, int sz) {
	int i = 0;
	vector<string> pieces;
	while (i < s.length()) {
		string p = "";
		for (int j = i; (j < s.length()) && (j < i+sz); j++) {
			p.push_back(s[j]);
		}
		pieces.push_back(p);
		i += sz;
	}

	return pieces;
}

vector<string> ls() {
	vector<string> output;

	DIR* dirp = opendir(".");
	if (!dirp) {
		output.push_back("~E1");
		return output;
	}
	struct dirent * dp;
	vector<string> files;
	while ((dp = readdir(dirp)) != NULL) {
		files.push_back(dp->d_name);
	}
	closedir(dirp);
	for (auto f : files) {
		if ((f != ".") && (f != ".."))
			output.push_back(f+'\n');
	}
	return output;
}

int cd(string dir) {
	int cv = chdir(dir.c_str());
	if (cv) {
		if (errno == EACCES)
			return 1;
		else
			return 2;
	}
	else
		return 0;
}

#include "header.h"
using namespace std;

char a[] = "lo";
int c = ConexaoRawSocket(a);

int main() {
	string r;
	int type;
	string args;
	while (true) {
		//get command
		r = receive_();

		//decided if it should send output
		type = get_type(r);
		bool has_output = (type != T_CD) && (type != T_EDIT);

		//get args from data
		string filename, line1, extra;
		filename = get_data(r, get_size(r)+8);

		//get args from more messages
		if (type > 2) { //more messages
			//get lines
			r = receive_();
			r = get_data(r, get_size(r)+8);
			line1 = "";
			int i = 0;
			while ((i < r.length()) && (r[i] != ' '))
				line1.push_back(r[i++]);
			i++;
			if (type == T_LINHAS) 
				while ((i < r.length()) && (r[i] != ' '))
					extra.push_back(r[i++]);
			//get filename
			else if (type == T_EDIT) {
				extra = "";
				bool keep_receiving;
				do {
					r = receive_();
					keep_receiving = get_type(r) != T_FIM;
					extra += get_data(r, get_size(r)+8);
				} while (keep_receiving);
			}
		} else { 
			args = get_data(r, get_size(r)+8);
			//cout << type << endl;
			//cout << args << endl;
		}

		//prepare for output 
		vector<string> output;
		int output_type = (type == T_LS ? T_LSC : T_ARQC);
		int error = 0;
		//execute command and get output
		switch (type) {
			case T_LS:
				output.push_back("die erfrischungsgetränke");
				break;
			case T_VER:
				output.push_back("eeeeeeeeita coisa boaaaaaaaaaaa\nmas tem ate duas linhas po que coisa boaaaaaa\nbom demais ohm...\nhehe");
				break;
			case T_LINHA:
				output.push_back("eeeeeeeeita coisa boaaaaaaaaaaa\nmas tem ate duas linhas po que coisa boaaaaaa\nbom demais ohm...\nhehe");
				break;
			case T_LINHAS:
				for (int i = 0; i < 100; i++)
				output.push_back("eeeeeeeeita coisa boaaaaaaaaaaa\nmas tem ate duas linhas po que coisa boaaaaaa\nbom demais ohm...\nhehe");
				break;
			default:
				break;
		}
		//send response to client
		string response = "";
		if (error) {
			response = format(_itos(error, 2), T_ERR);
			send_(response);
		} else if (!has_output) { //cd or edit
			response = format("ok", T_FIM);
			send_(response);
		} else if (has_output) { //not cd or edit
			for (int i = 0; i < output.size(); i++) {
				//for each string on output, divide it into 15 byte pieces and send
				vector<string> divided_text = divide(output[i], 15);
				for (int p = 0; p < divided_text.size(); p++) {
					response = format(divided_text[p], output_type);
					send_(response);
				}
			}
			//then, send an end of transmission message
			response = format("", T_FIM);
			send_(response);
		}
	}
	return 0;
}

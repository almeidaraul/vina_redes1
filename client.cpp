#include "header.h"
using namespace std;

char a[] = "lo";
int c = ConexaoRawSocket(a);

int main() {
	string input;
	cout << "[client]: ";
	while (getline(cin, input)) {

		//get cmd and args
		string cmd = "";
		int i = 0;
		while ((i < input.length()) && (input[i] != ' '))
			cmd.push_back(input[i++]);

		string args = "";
		if ((i < input.length()) && (input[i] == ' '))
			i++;
		while (i < input.length())
			args.push_back(input[i++]);

		//do something with cmd and args
		if (cmd == "lls" || cmd == "lcd") {
			cout << cmd << args << endl;
		}
		else if (cmd == "linha" || cmd == "linhas" || cmd == "edit") {
			//get type from cmd
			int type;
			if (cmd == "linha")
				type = T_LINHA;
			else if (cmd == "linhas")
				type = T_LINHAS;
			else if (cmd == "edit")
				type = T_EDIT;

			string linha1, extra, file;
			file = "";
			linha1 = "";
			extra = "";

			//get line from args
			int j = 0;
			while ((j < args.length()) && (args[j] != ' '))
				linha1.push_back(args[j++]);
			if ((j < args.length()) && (args[j] == ' '))
				j++;

			//get filename from args
			while ((j < args.length()) && (args[j] != ' '))
				file.push_back(args[j++]);
			if ((j < args.length()) && (args[j] == ' '))
				j++;

			//get second line from args
			if (cmd == "linhas") {
				extra = file;
				file =  "";
				while ((j < args.length()) && (args[j] != ' '))
					file.push_back(args[j++]); 
			} //or new text for given line:
			else if (cmd == "edit") {
				j = 0;
				while ((j < args.length()) && (args[j++] != '"'));
				while ((j < args.length()) && (args[j] != '"'))
					extra.push_back(args[j++]);
			}

			//send file + type
			string msg = format(file, type);
			send_(msg);

			//send lines
			string lines = linha1 + (cmd == "linhas" ? " "+extra : "");
			msg = format(lines, T_LINHASIF);
			send_(msg);

			//send new text
			if (cmd == "edit") {
				vector<string> divided_text = divide(extra, 15);
				for (int p = 0; p < divided_text.size(); p++) {
					msg = format(divided_text[p], T_ARQC);
					send_(msg); 
				}
				msg = format("", T_FIM);
				send_(msg);
			}
		}
		else {
			//get type for cmd
			int type;
			if (cmd == "cd")
				type = T_CD;
			else if (cmd == "ls")
				type = T_LS;
			else if (cmd == "ver")
				type = T_VER;
			else if (cmd == "linha")
				type = T_LINHA;
			else if (cmd == "linhas")
				type = T_LINHAS;
			else if (cmd == "edit")
				type = T_EDIT;

			//send message w/ args and type
			string msg = format(args, type);
			send_(msg);
		}
		cout << "[client]: ";
	}
	cout << "(end of input)" << endl;
	/*
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
	*/
	return 0;
}

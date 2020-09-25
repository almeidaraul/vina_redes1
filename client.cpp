#include "header.h"
using namespace std;

char a[] = "lo";
int c = ConexaoRawSocket(a);

int main() {
	cout << "VINA - VIm Nas Alturas" << endl;
	string input;
	cout << "[client]: ";
	while (getline(cin, input)) {

		//get cmd and args
		string cmd = "";
		int i = 0;
		while ((i < input.length()) && (input[i] != ' '))
			cmd.push_back(input[i++]);
		
		bool has_output = (cmd != "cd") && (cmd != "edit");

		string args = "";
		if ((i < input.length()) && (input[i] == ' '))
			i++;
		while (i < input.length())
			args.push_back(input[i++]);

		//do something with cmd and args
		if (cmd == "lls" || cmd == "lcd") {
			if (cmd == "lls") {
				vector<string> output = ls();
				for (auto part : output)
					cout << part;
			}
			else
				cd(args);
		}
		else { if (cmd == "linha" || cmd == "linhas" || cmd == "edit") {
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

			//send message w/ args and type
			string msg = format(args, type);
			send_(msg);
		}
		//get output
		string response = receive_();
		//check for error
		if (get_type(response) == T_ERR) {
			int ei = 0;
			string ed = get_data(response, get_size(response)+8);
			while ((ed[ei] <= '0') || (ed[ei] >= '5'))
				ei++;
			ei = ed[ei];
			switch (ei) {
				case '1':
					cout << "E1: acesso proibido / sem permissão" << endl;
					break;
				case '2':
					cout << "E2: diretório inexistente" << endl;
					break;
				case '3':
					cout << "E3: arquivo inexistente" << endl;
					break;
				case '4':
					cout << "E4: linha inexistente" << endl;
					break;
				default:
					cout << "Erro" << endl;
					break;
			}
		} else if (has_output) { //not cd and not edit
			string output_data = "";
			//while not at end of transmission, get data
			do {
				output_data += get_data(response, get_size(response)+8);
				response = receive_();
			} while (get_type(response) != T_FIM);
			cout << output_data << endl;
		}
		}
		cout << "[client]: ";
	}
	cout << "(end of input)" << endl;
	return 0;
}

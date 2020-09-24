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
		}

		//prepare for output 
		vector<string> output;
		int output_type = (type == T_LS ? T_LSC : T_ARQC);
		int error = 0;
		//execute command and get output
		switch (type) {
			case T_CD:
				error = cd(args);
				break;
			case T_LS:
				output = ls();
				break;
			case T_VER:
				output = ver(filename);
				if (output.size() == 1) {
					if ((output[0][0] == '~') && (output[0][1] == 'E'))
						error = output[0][1]-'0';
				}
				break;
			case T_LINHA: {
				int linha_numero_1 = stoi(line1);
				output = linhas(linha_numero_1, linha_numero_1, filename);
				break;
			}
			case T_LINHAS: {
				int linha_numero_1 = stoi(line1);
				int linha_numero_2 = stoi(extra);
				output = linhas(linha_numero_1, linha_numero_2, filename);
				break;
			}
			case T_EDIT: {
				int linha_numero_1 = stoi(line1);
				error = edit(extra, linha_numero_1, filename);
				break;
			}
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

vector<string> ver(string user_input) {
	string filename = user_input;
	string line;
	ifstream file(filename);

	vector<string> output;

	if (file.is_open()) {
		while (getline(file, line)) {
			output.push_back(line+'\n');
		}
		file.close();
	}
	else {
		output.push_back("~E"+3);
	}
	return output;
}

vector<string> linhas(int line_1, int line_N, string filename) {
	string line;
	ifstream file(filename);

	vector<string> output;

	int current_line = 1;

	if (file.is_open()) {
		while (getline(file, line)) {
			if (current_line >= line_1 && current_line <= line_N)
				output.push_back(line+'\n');
			current_line++;
		}
		file.close();
	}
	else {
		cerr << "Unable to open file " << filename << endl;
	}
	return output;
}

int edit(string user_input, int edited_line, string filename) {
	string new_text = user_input;
	string line;
	vector<string> file_text;

	//read whole file into file_text
	ifstream rfile(filename);
	if (rfile.is_open()) {
		while (getline(rfile,line)) {
			file_text.push_back(line);
		}
		rfile.close();
	}
	else {
		cerr << "Unable to open file " << filename << endl;
	}

	//edit desired line
	file_text[edited_line-1] = new_text;

	//write edited file
	ofstream wfile(filename);
	if (wfile.is_open()) {
		for (auto line : file_text)
			wfile << line << endl;
		wfile.close();
	}
	else {
		cerr << "Unable to open file " << filename << endl;
	}
	return 0;
}

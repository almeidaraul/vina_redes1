#include "header.h"
using namespace std;

bool get_input(string& user_input, bool am_client) {
	/*
		get input from client (if !am_client) or from stdin (else)
	*/
	if (am_client) {
		cout << "[client]: "; 
		bool retval = getline(cin, user_input) ? true : false;
		return retval;
	}
	else {
		string received = receive_message();
		unsigned char type = get_type(received);
		int sz = received[1];
		if (type == 0)
			user_input = "cd ";
		else if (type == 1)
			user_input = "ls ";
		else if (type == 2)
			user_input = "ver ";
		else if (type == 3)
			user_input = "linha ";
		else if (type == 4)
			user_input = "linhas ";
		else if (type == 5)
			user_input = "edit ";
		//se for edit, linha ou linhas tem mais mensagem
		if (type > 2 && type < 6) {
			//pega nome do arquivo
			received = receive_message();
			for (int i = 3; i < 3+sz; i++)
				user_input.push_back(received[i]);
			user_input.push_back(' ');
			//pega linhas
			received = receive_message();
			for (int i = 3; i < 3+sz; i++)
				user_input.push_back(received[i]);
			if (type == 5) { //edit, precisa receber o arquivo tbm
				user_input.push_back(' ');
				vector<string> file_content = receive_in_pieces();
				for (auto file_piece : file_content)
					user_input += file_piece;
			}
		}
		else {//n é edit, linha ou linhas
			for (int i = 4; i < 4+sz; i++) {
				user_input.push_back(received[i]);
			}
		}
		return true;
	}
}

void send_output(vector<string>& output, bool am_client, bool is_ls_content) {
	/*
		send output to stdout (if client) or to client (if server)
	*/
	if (am_client)
		for (auto line : output)
			cout << line << endl;
	else {
		unsigned char type = is_ls_content ? 11 : 12;
		string complete_data = "";
		for (auto piece : output)
			cout << piece << endl;
			//complete_data += piece;
		//send_in_pieces(type, complete_data);
	}
}

void exec_input(string user_input, bool am_client) {
	/*
		do what user wants you to do
			client: sends request to server and outputs response
			server: sends requested information to client
	*/
	vector<string> split_input = split(user_input);
	string cmd = split_input[0];
	vector<string> output;
	bool is_ls_content = false;
	if (!am_client) {
		cout << user_input << endl;
		return;
		if (cmd == "cd")
			cd(split_input[1]);
		else if (cmd == "ls") {
			output = ls();
			is_ls_content = true;
		}
		else if (cmd == "ver") {
			output = ver(split_input[1]);
		}
		else if (cmd == "linha")
			output = linhas(stoi(split_input[1]), stoi(split_input[1]), split_input[2]);
		else if (cmd == "linhas")
			output = linhas(stoi(split_input[1]), stoi(split_input[2]), split_input[3]);
		else if (cmd == "edit")
			edit(user_input, stoi(split_input[1]), split_input[2]);
	}
	else { //am_client == true
		if (cmd == "lcd")
			cd(split_input[1]);
		else if (cmd == "lls")
			output = ls();
		//comandos de servidor:
		else {
			unsigned char type;
			if (cmd == "cd")
				type = 0;
			else if (cmd == "ls")
				type = 1;
			else if (cmd == "ver")
				type = 2;
			else if (cmd == "linha")
				type = 3;
			else if (cmd == "linhas")
				type = 4;
			else if (cmd == "edit")
				type = 5;

			//edit, linha e linhas: transforma em pedaços e manda
			if (cmd == "edit" || cmd == "linha" || cmd == "linhas") {
				//envia nome do arquivo
				string file = split_input[split_input.size()-1];
				string message = msg(0xff, type, file);
				cout << "EMPACOTEI COM TYPE " << int(type) << endl;
				send_message(message);

				//envia linhas do arquivo
				string lines = split_input[1];
				if (cmd == "linhas") //needs two lines
					lines += " " + split_input[2];
				message = msg(0xff, 10, lines);
				send_message(message);

				//edit: envia conteúdo
				if (cmd == "edit") {
					string content = user_input;
					for (int i = 0; i < 3; i++)
						content = cut_first_word(content);
					send_in_pieces(12, content);
				}
			} else { //todos os outros: manda 1 só
				string command = msg(0xff, type, cut_first_word(user_input));
				send_message(command);
			}
			//todos menos cd e edit: recebe output
			/*
			if (cmd != "cd" && cmd != "edit")
				output = receive_in_pieces();
			*/
		}
	}
	/*
	if (output.size())
		send_output(output, am_client, is_ls_content);
	*/
}

vector<string> ver(string user_input) {
	cout << "tranquilo\n";
	string filename = user_input;
	string line;
	ifstream file(filename);

	vector<string> output;

	if (file.is_open()) {
		while (getline(file, line))
			output.push_back(line);
		file.close();
	}
	else {
		cerr << "Unable to open file " << filename << endl;
	}
	return output;
}

void edit(string user_input, int edited_line, string filename) {
	string new_text = process_for_edit(user_input);
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
}

void cd(string dir) {
	chdir(dir.c_str());
}

vector<string> ls() {
	DIR* dirp = opendir(".");
	struct dirent * dp;
	vector<string> files;
	while ((dp = readdir(dirp)) != NULL) {
		files.push_back(dp->d_name);
	}
	closedir(dirp);
	vector<string> output;
	for (auto f : files)
		output.push_back(f);
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
				output.push_back(line);
			current_line++;
		}
		file.close();
	}
	else {
		cerr << "Unable to open file " << filename << endl;
	}
	return output;
}

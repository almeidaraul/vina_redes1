#include "header.h"
using namespace std;

int main(int argc, char *argv[]) {
	bool am_client = (argc > 1 && argv[1][0] == 'c');
	string user_input;

	/*
	string msg = "hello";
	if (am_client) {
		send_message(msg);
	} else {
		string r = receive_message();
		cout << "received: " << r << endl;
	}
	*/
	/*
	if (!am_client) {
		while (true) {
			cout << receive_message() << endl;
		}
	}
	else {
		*/
	while (get_input(user_input, am_client)) {
		exec_input(user_input, am_client);
	};
	if (am_client) {
		cout << "(end of input)" << endl;
	}
	return 0;
}

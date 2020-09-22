#include <bits/stdc++.h>
using namespace std;

int teste(string& e) {
	if (cin >> e) return 1;
	else return 0;
}

int main() {
	unsigned char sz = 8; //0000.1000
	unsigned char sequence = 2; //0000.0010
	unsigned char type = 4; //0000.0100
	unsigned char first_byte = (sz << 4) + (sequence >> 4); //1000.0000 - 128
	unsigned char second_byte = ((sequence & 15) << 4) + type; //0010.0100 - 36
	cout << int(first_byte) << ' ' << int(second_byte) << endl;
	cout << int(first_byte >> 4) << endl;
}

#include "header.h"
using namespace std;

int main() {
	string f = "hello";
	f = format(f);
	read_formatted(f);
	send_(f);
	return 0;
}

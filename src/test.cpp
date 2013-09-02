#include <stdafx.h>
#include <lambda.hpp>

using namespace std;

int main() {
	cout << "lal" << endl;
	(cout << ~_1 << " lawl" << endl<char, char_traits<char>>)((5 + _1)(6));
	cout << (_1 + _1)(3, 4) << endl;
	cout << _1("lel", "lol") << endl;

	int x = 4;
	(cout << _1 << " " << ++_1 << " " << _1)(x) << " " << x << endl;

	return 0;
}

#include <stdafx.h>
#include <lambda.hpp>

using namespace std;

int main() {
	cout << "lal" << endl;
	(cout << _1 << endl<char, char_traits<char>>)("trolol");
	(cout << ~_1 << " lawl" << endl<char, char_traits<char>>)((5 + _1)(6));
	cout << (_1 + _2)(3, 4) << endl;
	cout << _2("lel", "lol") << endl;

	int x = 4;
	(cout << constant("must print 5 5 4 5: ") << _1 << " " << ++_1 << " " << _1)(x) << " " << x << endl;

	auto l = if_then_else(_1,
		cout << constant("true") << endl<char, char_traits<char>>,
		cout << constant("false") << endl<char, char_traits<char>>
		);
	l(true);
	l(false);

	return 0;
}

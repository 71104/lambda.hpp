#include <stdafx.h>
#include <lambda.hpp>

using namespace std;

int main() {
	cout << "lal" << boolalpha << endl;

	(cout << _1)(3) << endl;
	(cout << _1)(false) << endl;
	(cout << _1)(3.14) << endl;
	(cout << _1)((char const*)"trolol") << endl;
	//(cout << _1)("trolol") << endl;

	int x = 0;
	(_1l += _2)(x, 5);
	cout << x << endl;

	//(cout << ~_1 << " lawl" << endl<char, char_traits<char>>)((5 + _1)(6));
	//cout << (_1 + _2)(3, 4) << endl;
	//cout << _2("lel", "lol") << endl;

	//int x = 4;
	//(cout << constant("must print x 5 x 5: ") << _1 << " " << ++_1 << " " << _1)(x);
	//cout << " " << x << endl;

	//auto l = if_then_else(_1,
	//	cout << constant("true") << endl<char, char_traits<char>>,
	//	cout << constant("false") << endl<char, char_traits<char>>
	//	);
	//l(true);
	//l(false);

	//int y = 5;
	//cout << (_1 += 3)(y) << endl;
	//cout << (y += _1)(3) << endl;
	//cout << (var(y) += _1)(3) << endl;

	//int z = 1;
	//cout << "must print 3: " << (var(z) = 2, _1 + z)(z) << endl;

	//struct {
	//	void operator () (int x, int y) {
	//		cout << x << " " << y << endl;
	//	}
	//} f;
	//bind(f, 1)(2);

	return 0;
}

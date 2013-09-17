#include <lambda.hpp>
#include <stdafx.h>
#include <cassert>

using namespace std;

int main() {
	cout << "Running tests..." << endl;

	assert(3 == _1(3));
	assert(false == _1(false));
	assert(3.14 == _1(3.14));
	//assert(string("ok") == _1("ok"));
	//assert(string("ok") == _1((char const[])"ok"));
	//assert(string("ok") == string(_1((char const*)"ok")));
	//assert(string("ok") == _1(string("ok")));
	//{
	//	char const sz[] = "ok";
	//	assert(string("ok") == _1(sz));
	//}
	//{
	//	char const *const psz = "ok";
	//	assert(string("ok") == (char const[])(_1(psz)));
	//}

	// TODO

	{
		int x = 3;
		assert(8 == (_1 + _2)(x, 5));
		assert(3 == x);
	}
	{
		int x = 3;
		assert(8 == (_1l += _2)(x, 5));
		assert(8 == x);
	}

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

	cout << "Ok, everything\'s fine" << endl;
	return 0;
}

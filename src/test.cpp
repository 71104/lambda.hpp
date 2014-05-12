#include <iostream>
#include <lambda.hpp>
#include <cassert>

using namespace std;
using namespace Lambda;

template<typename _ID, typename _Expression>
void test(_ID const id, _Expression &&rre) {
	assert(rre);
	cout << "test " << id << " ok" << endl;
}

int main() {
	cout << boolalpha;

	{
		test("0001", !IsFunctor<int>::s_Value);
		test("0002", IsFunctor<Functor>::s_Value);
		test("0003", IsFunctor<Constant<int>>::s_Value);
		test("0004", IsFunctor<Bind<0>>::s_Value);
	}

	{
		int x = 0;
		test("0101", Bind<0>()(0) == 0);
		test("0102", Bind<0>()(1) == 1);
		test("0103", Bind<0>()(x) == 0);
		test("0104", Bind<0>()(x + 1) == 1);
		test("0105", Bind<0>()(0, 1) == 0);
		test("0106", Bind<0>()(x, x + 1) == 0);
		test("0107", Bind<0>()(x + 1, x + 2) == 1);
		test("0108", Bind<1>()(0, 1) == 1);
		test("0109", Bind<1>()(0, 2) == 2);
		test("0110", Bind<1>()(2, 1) == 1);
		test("0111", Bind<1>()(x, 1) == 1);
		test("0112", Bind<1>()(1, x) == 0);
		test("0113", Bind<1>()(x, x + 1) == 1);
		test("0114", Bind<1>()(x + 1, x + 2) == 2);
		// TODO
	}

	{
		struct {} x;
		cout << &x << " " << (5 + 5) << endl;
	}

	{
		cout << (!_1)(false) << " " << (!_1)(true) << " " << (!!_1)(true) << endl;
		(cout << (_1 + 3))(5) << endl;
	}

	return 0;
}

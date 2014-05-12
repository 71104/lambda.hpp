#include <iostream>
#include <lambda.hpp>

using namespace std;
using namespace Lambda;

#define TEST(id, expression) \
	cout << "test " << id << " ..."; \
	if (expression) { \
		cout << " ok" << endl; \
	} else { \
		cerr << " failed!" << endl; \
		cerr << "at " << __FILE__ << ":" << __LINE__ << endl; \
		cerr << "\t" << #expression << endl; \
		return 1; \
	}

int main() {
	{
		TEST("0001", !IsFunctor<int>::s_Value);
		TEST("0002", IsFunctor<Functor>::s_Value);
		TEST("0003", IsFunctor<Constant<int>>::s_Value);
		TEST("0004", IsFunctor<Bind<0>>::s_Value);
	}

	{
		int x = 0;
		TEST("0101", Bind<0>()(0) == 0);
		TEST("0102", Bind<0>()(1) == 1);
		TEST("0103", Bind<0>()(x) == 0);
		TEST("0104", Bind<0>()(x + 1) == 1);
		TEST("0105", Bind<0>()(0, 1) == 0);
		TEST("0106", Bind<0>()(1, x) == 1);
		TEST("0107", Bind<0>()(0, x + 1) == 0);
		TEST("0108", Bind<0>()(x, 1) == 0);
		TEST("0109", Bind<0>()(x, x) == 0);
		TEST("0110", Bind<0>()(x, x + 1) == 0);
		TEST("0111", Bind<0>()(x + 1, 0) == 1);
		TEST("0112", Bind<0>()(x + 1, x) == 1);
		TEST("0113", Bind<0>()(x + 1, x + 1) == 1);
		TEST("0114", Bind<1>()(0, 1) == 1);
		TEST("0115", Bind<1>()(0, 2) == 2);
		TEST("0116", Bind<1>()(2, 1) == 1);
		TEST("0117", Bind<1>()(x, 1) == 1);
		TEST("0118", Bind<1>()(1, x) == 0);
		TEST("0119", Bind<1>()(x, x + 1) == 1);
		TEST("0120", Bind<1>()(x + 1, x + 2) == 2);
		TEST("0121", Bind<1>()(1, 2, 3) == 2);
	}

	{
		int x = 0;
		TEST("0201", _0(0)() == 0);
		TEST("0202", _0("lala")() == string("lala"));
		TEST("0203", _0(0)(1) == 0);
		TEST("0204", _0(1)(x) == 1);
		TEST("0205", _0(0)(x + 1) == 0);
		TEST("0206", _0(2)(0, 1) == 2);
		TEST("0207", _0(2)(1, x) == 2);
		TEST("0208", _0(2)(0, x + 1) == 2);
		TEST("0209", _0(2)(x, 1) == 2);
		TEST("0210", _0(1)(x, x) == 1);
		TEST("0211", _0(2)(x, x + 1) == 2);
		TEST("0212", _0(2)(x + 1, 0) == 2);
		TEST("0213", _0(2)(x + 1, x) == 2);
		TEST("0214", _0(0)(x + 1, x + 1) == 0);
		TEST("0215", _1(true, false));
	}

	{
		int x = 0;
		TEST("0301", (&_0(x))() == &x);
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

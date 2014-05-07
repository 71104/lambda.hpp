#include <iostream>
#include <lambda.hpp>
#include <cassert>

using namespace std;
using namespace Lambda;

int main() {
	cout << boolalpha;

	{
		assert(!IsFunctor<int>::s_Value);
		assert(IsFunctor<Functor>::s_Value);
		assert(IsFunctor<Constant<int>>::s_Value);
		assert(IsFunctor<Bind<0>>::s_Value);
	}

	{
		int x = 0;
		assert(Bind<0>()(x + 1, x, x, x, x) == 1);
		assert(Bind<0>()(1, 2, 3, 4, 5) == 1);
		assert(Bind<2>()(x + 1, x, x, x, x) == 0);
		assert(Bind<2>()(1, 2, 3, 4, 5) == 3);
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

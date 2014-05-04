#include <iostream>
#include <lambda.hpp>

using namespace std;
using namespace Lambda;

int main() {
	cout << boolalpha;

	{
		int x;
		Bind<0>()(x, x, x, x, x);
		Bind<0>()(1, 2, 3, 4, 5);
		Bind<2>()(x, x, x, x, x);
		Bind<2>()(1, 2, 3, 4, 5);
	}

	{
		cout << IsFunctor<int>::s_Value << endl;
		cout << IsFunctor<Functor>::s_Value << endl;
		cout << IsFunctor<Constant<int>>::s_Value << endl;
		cout << IsFunctor<Bind<0>>::s_Value << endl;
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

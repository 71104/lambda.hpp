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
		cout << IsFunctor<int>::s_f << endl;
		cout << IsFunctor<Functor>::s_f << endl;
		cout << IsFunctor<Bind<0>>::s_f << endl;
	}

	{
		struct {} x;
		bool y = &x;
	}

	return 0;
}

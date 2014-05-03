#include <iostream>
#include <lambda.hpp>

using namespace std;
using namespace Lambda;

int main() {
	int x;
	Bind<0>()(x, x, x, x, x);
	Bind<0>()(1, 2, 3, 4, 5);
	Bind<2>()(x, x, x, x, x);
	Bind<2>()(1, 2, 3, 4, 5);
	return 0;
}

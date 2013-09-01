#include <stdafx.h>
#include <lambda.hpp>

using namespace std;

int main() {
	cout << "lal" << endl;
	(cout << _1 << " lawl")((5 + _1)(6)) << endl;
	cout << (_1 + _1)(3, 4);
	return 0;
}

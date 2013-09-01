#include <stdafx.h>
#include <lambda.hpp>

using namespace std;

auto endl2 = endl<char, char_traits<char>>;

int main() {
	cout << "lal" << endl;
	(cout << _1 << " lawl" << endl2)((5 + _1)(6));
	cout << (_1 + _1)(3, 4) << endl;
	return 0;
}

#include <ct/metafunction.h>
#include <iostream>

using namespace std;

int main() {
	cout << ct::is_signed<char>::value << endl;
	cout << ct::has_sizeof<1>::func<char>::value << endl;
	cout << ct::logical_not<ct::is_signed>::func<char>::value << endl;
}
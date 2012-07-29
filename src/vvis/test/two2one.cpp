// two2one.cpp
// Bing-Chang Lai
// Test program for two-to-one transformations.
// Please note that true is ~0 and false is 0. This is slightly different
// to the scalar operation where true is 1 and false is 0.
#include <iostream>
#include <functional>
#include <vvis/vvis.h>

using namespace std;
using namespace vvis;
using namespace vvm;

void print(const unsigned char *begin, const unsigned char *end)
{
	int i = 1;
	for(const unsigned char *pi = begin; pi != end; ++pi, ++i)
	{
		cout << (unsigned short)*pi << " " ;
		if(i % 10 == 0)
			cout << endl;
	}
}

template<typename T>
Vector<T> assign(const Vector<T> &a)
{
	return a;
}

int main(int argc, char *argv[])
{
	const int size = 100;
	unsigned char source1[size], source2[size], output[size];

	// Fill source1 with 0..size-1
	// Fill source2 with size-1..0
	for(int i = 0; i < size; ++i)
	{
		source1[i] = i;
		source2[i] = size - 1 - i;
	}

	cout << ">> Source1 [0-" << size << "]:" << endl;
	print(&source1[0], &source1[size]);

	cout << ">> Source2 [0-" << size << "]:" << endl;
	print(&source2[0], &source2[size]);

	for(int i = 0; i < 40; ++i)
	{
		for(int j = 0; j < 40; ++j)
		{
			// Clear output
			std::fill(&output[0], &output[size], 1);

			vvis::transform(&source1[j], &source1[size - i], &source2[j], &output[i], std::plus<Vector<unsigned char> >());

			cout << ">> Output Offset:" << i << " Input Offset:" << j << " " << "[" << i << ":" << j << "]" << endl;
			print(&output[0], &output[size]);
		}
	}
	
	return 0;
}

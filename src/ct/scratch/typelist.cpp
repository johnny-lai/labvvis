#include <iostream>
#include <common/type_name.h>
#include <ct/typelist.h>
#include <ct/tuple.h>
#include <ct/metafunction.h>

using namespace std;
using namespace ct;

int main(int argc, char* argv[]) {
	typedef cons_tl<int, float, double>::type tl1;
	cout << "Checking index_of ... " << endl;
	cout << "   char: " << index_of<tl1, char>::value << endl;
	cout << "    int: " << index_of<tl1, int>::value << endl;
	cout << "  float: " << index_of<tl1, float>::value << endl;
	cout << " double: " << index_of<tl1, double>::value << endl;

	cout << "Checking type_at ... " << endl;
	cout << " 0: " << type_name(typeid(type_at<tl1, 0>::type)) << endl;
	cout << " 1: " << type_name(typeid(type_at<tl1, 1>::type)) << endl;
	cout << " 2: " << type_name(typeid(type_at<tl1, 2>::type)) << endl;

	typedef apply<tl1, boost::add_pointer>::type tl2;
	cout << "Checking apply ... " << endl;
	cout << " 0: " << type_name(typeid(type_at<tl2, 0>::type)) << endl;
	cout << " 1: " << type_name(typeid(type_at<tl2, 1>::type)) << endl;
	cout << " 2: " << type_name(typeid(type_at<tl2, 2>::type)) << endl;	
	
	typedef cons_tl<int, int, int>::type tl3;
	cout << "Checking tuple ... " << endl;
	tuple<tl3> t;
	t.get<0>() = 10;
	t.get<1>() = 20;
	t.get<2>() = 30;
	cout << "0: " << t.get<0>() << endl;
	cout << "1: " << t.get<1>() << endl;
	cout << "2: " << t.get<2>() << endl;
	
	cout << "Checking tuple ... " << endl;
	const tuple<tl3> t2 = t;
	cout << "0: " << t2.get<0>() << endl;
	cout << "1: " << t2.get<1>() << endl;
	cout << "2: " << t2.get<2>() << endl;
	
	typedef cons_tl<int&, int&, int&>::type tl4;
	int a = 0, b = 0, c = 0;
	cout << "Checking tuple ... " << endl;
	tuple<tl4> t3(a, b, c);
	t3.get<0>() = 1;
	t3.get<1>() = 2;
	t3.get<2>() = 3;
	cout << "0: " << a << endl;
	cout << "1: " << b << endl;
	cout << "2: " << c << endl;

	cout << "Checking Tuple Conversion" << endl;
	//typedef apply<tl3, boost::add_const>::type tl5;
	boost::tuple<int, int> bt(1, 2);
	boost::tuple<const int, const int> bt2(bt);
	//tuple<apply<tl3, boost::add_const>::type> ct(t);
	cout << "const t = " << t << endl;
	
	cout << "Check Tuple Operations ... " << endl;
	cout << "t = " << t << endl;
	t += 4;
	cout << "t += 4;\t" << t << endl;
	++t;
	cout << "++t;\t" << t << endl;
	--t;
	cout << "--t;\t" << t << endl;
	
	typedef cons_tl<char, short, int, long>:: type tl5;
	typedef keep_only<tl5, has_sizeof<4>::func>::type tl6;
	cout << "Checking keep_only ..." << endl;
	cout << " len: " << length<tl6>::value << endl;
	cout << " 0: " << type_name(typeid(type_at<tl6, 0>::type)) << endl;
	cout << " 1: " << type_name(typeid(type_at<tl6, 1>::type)) << endl;
	
	typedef find_first<tl5, has_sizeof<4>::func>::type first;
	cout << " first with size 4: " << type_name(typeid(first)) << endl;

	typedef find_first<tl5, has_sizeof<11>::func>::type first2;
	cout << " first with size 11 (no default): " << type_name(typeid(first2)) << endl;
	
	typedef find_first<tl5, has_sizeof<11>::func, int>::type first3;
	cout << " first with size 11 (default int): " << type_name(typeid(first3)) << endl;
	
	return 0;
}

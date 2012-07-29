#include <string>
#include <typeinfo>

std::string type_name(const std::type_info& i) {
	if(i == typeid(char)) {
		return "char";
	} else if(i == typeid(unsigned char)) {
		return "unsigned char";
	} else if(i == typeid(signed char)) {
		return "signed char";
	} else if(i == typeid(short int)) {
		return "short int";
	} else if(i == typeid(unsigned short int)) {
		return "unsigned short int";
	} else if(i == typeid(int)) {
		return "int";
	} else if(i == typeid(unsigned int)) {
		return "unsigned int";
	} else if(i == typeid(long int)) {
		return "long int";
	} else if(i == typeid(unsigned long int)) {
		return "unsigned long int";
	} else if(i == typeid(float)) {
		return "float";
	} else if(i == typeid(double)) {
		return "double";
	} else if(i == typeid(long double)) {
		return "long double";
	} /*else if(i == typeid(__vector signed char)) {
		return "__vector signed char";
	}*/
	return i.name();
}
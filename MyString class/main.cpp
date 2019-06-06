#include "myString.h"
#include <iostream>


int main()
{
	MyString string;
	string = MyString::Concatenate("pea", "ch");
	string.Print();
	//string.Read();
	std::cout << string.Size() << '\n';
	std::cout << string.At(1) << '\n';
	string.Reverse();
	string.Print();
	string.ToUpper();
	string.Print();
	std::cin >> string;
	string.Append("Apple");
	string.PushBack('K');
	string = string * 3;
	string += "Orange";
	std::cout << string.Capacity() << '\n';
	string.ShrinkToFit();
	std::cout << string.Capacity() << '\n';
	std::cout << string << "\n";
	std::cout << string[10] << "\n";
	system("pause");
	return 0;
}

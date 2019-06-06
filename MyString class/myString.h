#include <iostream>

#ifndef MYSTRING
#define MYSTRING

class MyString {
	char* str_;
	size_t size_;
	size_t capacity_;

public:
	MyString();
	MyString(size_t n, char symb);
	MyString(const char* str);
	MyString(const MyString& str);
	MyString(MyString&& other);
	MyString& operator=(MyString&& other);
	MyString& operator=(const MyString& other);
	~MyString();

	void Realloc(size_t size);
	void ShrinkToFit();
	size_t Capacity() const;
	size_t Size() const;
	bool Compare(const MyString& str) const;
	bool IsSubstring(const MyString& str) const;
	void ToUpper();
	void ToLower();
	void Capitalize();
	void Print();
	void Read();
	char& At(int ind) const;
	void Reverse();
	void Clear();
	void Append(const MyString& str);
	void PushBack(char letter);
	const char* ToCString() const;
	static MyString Concatenate(const MyString& strBeg, const MyString& strEnd);
	void operator+=(const MyString rhs);
	friend MyString operator*(const MyString& lhs, int n);
	char& operator[](int ind);
	const char& operator[](int ind) const;
};

MyString operator+(const MyString& lhs, const MyString& rhs);

bool operator<(const MyString& lhs, const MyString& rhs);
bool operator>(const MyString& lhs, const MyString& rhs);
bool operator==(const MyString& lhs, const MyString& rhs);
bool operator!=(const MyString& lhs, const MyString& rhs);
bool operator<=(const MyString& lhs, const MyString& rhs);
bool operator>=(const MyString& lhs, const MyString& rhs);
std::ostream& operator<<(std::ostream& os, const MyString& string);
std::istream& operator>>(std::istream& is, MyString& string);

#endif
#include "myString.h"
#include <cstring>
#include <iostream>
#include <cctype>


template <class T>
void Swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

template <class T>
T FindMin(const T& a, const T& b) {
	return a <= b ? a : b;
}

void CopyFromArrToArr(const char* strFrom, char* strTo) {
	size_t length = strlen(strFrom);
	for (size_t i = 0; i < length; ++i) {
		strTo[i] = strFrom[i];
	}
	strTo[length] = '\0';
}

void MyString::Realloc(size_t size) {
	const int incRate = 2;
	if (size < capacity_) {
		return;
	}
	while (size >= capacity_) {
		capacity_ *= incRate;
	}
	char* tmpPtr = str_;
	str_ = new char[capacity_];
	CopyFromArrToArr(tmpPtr, str_);
	delete[] tmpPtr;
}

void MyString::ShrinkToFit() {
	if (capacity_ == size_ + 1) {
		return;
	}
	char* tmpPtr = str_;
	capacity_ = size_ + 1;
	str_ = new char[capacity_];
	CopyFromArrToArr(tmpPtr, str_);
	delete[] tmpPtr;
}

MyString::MyString() : MyString(0, '\0') {
}

MyString::MyString(size_t n, char symb) {
	size_ = n;
	capacity_ = size_ + 1;
	str_ = new char[capacity_];
	str_[size_] = '\0';
	for (size_t i = 0; i < size_; ++i) {
		str_[i] = symb;
	}
}

MyString::MyString(const char* str) {
	size_ = strlen(str);
	capacity_ = size_ + 1;
	str_ = new char[capacity_];
	CopyFromArrToArr(str, str_);
}

MyString::MyString(const MyString& str) : MyString(str.str_) {
}

MyString::MyString(MyString&& other) {
	str_ = other.str_;
	size_ = other.size_;
	capacity_ = other.capacity_;
	other.str_ = nullptr;
}

MyString& MyString::operator=(MyString&& other) {
	if (str_ == other.str_) {
		return *this;
	}
	Clear();
	str_ = other.str_;
	size_ = other.size_;
	capacity_ = other.capacity_;
	other.str_ = nullptr;
	return *this;
}

MyString& MyString::operator=(const MyString& other) {
	if (str_ == other.str_) {
		return *this;
	}
	if (other.size_ >= capacity_) {
		Clear();
		capacity_ = other.size_ + 1;
		str_ = new char[capacity_];
	}
	size_ = other.size_;
	CopyFromArrToArr(other.str_, str_);
	return *this;
}

MyString::~MyString() {
	delete[] str_;
}

size_t MyString::Size() const {
	return size_;
}

size_t MyString::Capacity() const {
	return capacity_;
}

bool MyString::Compare(const MyString& str) const {
	size_t length = FindMin(size_, str.size_);
	for (size_t i = 0; i <= length; ++i) {
		if (str_[i] > str.str_[i]) {
			return false;
		}
		else if (str_[i] < str.str_[i]) {
			return true;
		}
	}
	return false;
}

bool MyString::IsSubstring(const MyString& str) const {
	for (int i = 0; str.str_[i] != '\0'; ++i) {
		int indInThis = 0;
		int indInStr = i;
		while (str.str_[indInStr] == str_[indInThis] && str_[indInThis] != '\0') {
			++indInStr;
			++indInThis;
		}
		if (str_[indInThis] == '\0') {
			return true;
		}
	}
	return false;

}

void MyString::ToUpper() {
	for (size_t i = 0; i < size_; ++i) {
		if (islower(str_[i])) {
			str_[i] = toupper(str_[i]);
		}
	}
}

void MyString::ToLower() {
	for (size_t i = 0; i < size_; ++i) {
		if (isupper(str_[i])) {
			str_[i] = tolower(str_[i]);
		}
	}
}

void MyString::Capitalize() {
	if (islower(str_[0])) {
		str_[0] = toupper(str_[0]);
	}
}

void MyString::Print(){
	std::cout << str_ << '\n';
}

void MyString::Read() {
	const int maxReadSize = 1000;
	Clear();
	char* str = new char[maxReadSize];
	std::cin >> str;
	size_ = strlen(str);
	str_ = str;
	capacity_ = maxReadSize;
}

char& MyString::At(int ind) const {
	if (ind < size_) {
		return str_[ind];
	}
}

void MyString::Reverse() {
	for (size_t i = 0; i < size_ / 2; ++i) {
		Swap(str_[i], str_[size_ - 1 - i]);
	}
}

void MyString::Clear() {
	delete[] str_;
	size_ = 0;
	str_ = nullptr;
}

void MyString::Append(const MyString& str) {
	Realloc(size_ + str.size_);
	CopyFromArrToArr(str.str_, str_ + size_);
	size_ += str.size_;
}

void MyString::PushBack(char letter) {
	++size_;
	Realloc(size_ );
	str_[size_ - 1] = letter;
	str_[size_] = '\0';
}

MyString MyString::Concatenate(const MyString& strBeg, const MyString& strEnd) {
	MyString newstring;
	newstring.size_ = strBeg.size_ + strEnd.size_;
	newstring.str_ = new char[newstring.size_ + 1];
	CopyFromArrToArr(strBeg.str_, newstring.str_);
	CopyFromArrToArr(strEnd.str_, newstring.str_ + strBeg.size_);
	return newstring;
}

const char* MyString::ToCString() const {
	return str_;
}

MyString operator+(const MyString& lhs, const MyString& rhs) {
	return MyString::Concatenate(lhs, rhs);
}

bool operator<(const MyString& lhs, const MyString& rhs) {
	return lhs.Compare(rhs);
}

bool operator>(const MyString& lhs, const MyString& rhs) {
	return rhs.Compare(lhs);
}

bool operator==(const MyString& lhs, const MyString& rhs) {
	if (lhs.Size() != rhs.Size()) {
		return false;
	}
	for (size_t i = 0; i < lhs.Size(); ++i) {
		if (lhs.At(i) != rhs.At(i)) {
			return false;
		}
	}
	return true;
}

bool operator<=(const MyString& lhs, const MyString& rhs) {
	return (lhs < rhs || lhs == rhs);
}

bool operator>=(const MyString& lhs, const MyString& rhs) {
	return (lhs > rhs || lhs == rhs);
}

bool operator!=(const MyString& lhs, const MyString& rhs) {
	return !(lhs == rhs);
}

void MyString::operator+=(const MyString rhs) {
	Append(rhs);
}

MyString operator*(const MyString& rhs, int n) {
	MyString newString = rhs;
	newString.Realloc(newString.size_ * n);
	for (int i = 0; i < n - 1; ++i) {
		newString += rhs;
	}
	return newString;
}

std::ostream& operator<<(std::ostream& os, const MyString& string) {
	os << string.ToCString();
	return os;
}

std::istream& operator>>(std::istream& is, MyString& string) {
	const int readSize = 100'000;
	char tmpStr[readSize];
	is >> tmpStr;
	string.Clear();
	string = tmpStr;
	return is;
} 

char& MyString::operator[](int ind) {
	if (ind < size_) {
		return str_[ind];
	}
}

const char& MyString::operator[](int ind) const {
	if (ind < size_) {
		return str_[ind];
	}
}
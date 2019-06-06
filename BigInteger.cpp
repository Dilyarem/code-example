#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <cctype>

class HugeInteger {
	typedef uint16_t base_t;
	typedef uint32_t ext_base_t;
	static const base_t BASE_ = 100;

	base_t* buffer_;
	size_t size_;
	size_t capacity_;
	bool is_negative_;

	base_t operator[](size_t idx) const;
	base_t& operator[](size_t idx);

	void Realloc(size_t new_size);

	void FillBufferFromString(const char* number_str);

	static HugeInteger SumPositives(const HugeInteger& lhs, const HugeInteger& rhs);
	static HugeInteger SubtractPositives(const HugeInteger& lhs, const HugeInteger& rhs);

public:
	HugeInteger(const HugeInteger& other);
	HugeInteger(HugeInteger&& other);

	template <class IntegerType = int>
	HugeInteger(IntegerType number = 0);

	HugeInteger(const char* number_str);

	~HugeInteger();

	bool IsNegative() const;
	size_t Size() const;

	HugeInteger& operator=(const HugeInteger& other);
	HugeInteger& operator=(HugeInteger&& other);

	HugeInteger& operator+=(const HugeInteger& value);
	HugeInteger& operator-=(const HugeInteger& value);
	HugeInteger& operator/=(const HugeInteger& value);
	HugeInteger& operator*=(const HugeInteger& value);
	HugeInteger& operator%=(const HugeInteger& value);

	HugeInteger& operator++();
	HugeInteger operator++(int);

	HugeInteger& operator--();
	HugeInteger operator--(int);

	HugeInteger SearchingSqrt();

	friend HugeInteger operator+(const HugeInteger& value);
	friend HugeInteger operator-(const HugeInteger& value);

	friend HugeInteger operator+(const HugeInteger& lhs, const HugeInteger& rhs);
	friend HugeInteger operator-(const HugeInteger& lhs, const HugeInteger& rhs);

	friend HugeInteger operator*(const HugeInteger& lhs, const HugeInteger& rhs);
	friend HugeInteger operator/(const HugeInteger& lhs, const HugeInteger& rhs);
	friend HugeInteger operator%(const HugeInteger& lhs, const HugeInteger& rhs);

	friend HugeInteger abs(const HugeInteger& value);

	friend bool operator<(const HugeInteger& lhs, const HugeInteger& rhs);
	friend bool operator>(const HugeInteger& lhs, const HugeInteger& rhs);
	friend bool operator>=(const HugeInteger& lhs, const HugeInteger& rhs);
	friend bool operator<=(const HugeInteger& lhs, const HugeInteger& rhs);
	friend bool operator==(const HugeInteger& lhs, const HugeInteger& rhs);
	friend bool operator!=(const HugeInteger& lhs, const HugeInteger& rhs);

	friend std::ostream& operator<<(std::ostream& os, const HugeInteger& value);
	friend std::istream& operator>>(std::istream& is, HugeInteger& value);
};

typedef uint16_t base_t;
typedef uint32_t ext_base_t;

static bool HaveSameSign(const HugeInteger& first, const HugeInteger& second) {
	return first.IsNegative() == second.IsNegative();
}

void CopyFromBufToBuf(base_t* bufFrom, base_t* bufTo, size_t from_size, size_t to_size) {
	for (size_t i = 0; i < from_size; ++i) {
		bufTo[i] = bufFrom[i];

	}
	for (size_t i = from_size; i < to_size; ++i) {
		bufTo[i] = 0;
	}
}

void HugeInteger::FillBufferFromString(const char* number_str) {
	const char* pos_numb;
	if (number_str[0] == '-') {
		pos_numb = number_str + 1;
		is_negative_ = true;
	}
	else {
		pos_numb = number_str;
		is_negative_ = false;
	}
	size_t leng = strlen(pos_numb);
	size_ = (leng + 1) / 2;
	capacity_ = size_ + 1;
	buffer_ = new base_t[capacity_];
	for (size_t i = 0; i < size_ - 1; ++i) {
		buffer_[i] = (pos_numb[leng - 1 - 2 * i] - '0') + (pos_numb[leng - 1 - 2 * i - 1] - '0') * 10;
	}
	if (leng % 2 == 0) {
		buffer_[size_ - 1] = (pos_numb[1] - '0') + (pos_numb[0] - '0') * 10;
	}
	else {
		buffer_[size_ - 1] = (pos_numb[0] - '0');
	}
}

void HugeInteger::Realloc(size_t new_size) {
	const int incRate = 2;
	if (new_size < capacity_) {
		return;
	}
	while (new_size >= capacity_) {
		capacity_ *= incRate;
	}
	base_t* tmpPtr = buffer_;
	buffer_ = new base_t[capacity_];
	CopyFromBufToBuf(tmpPtr, buffer_, size_, new_size);
	delete[] tmpPtr;
}


base_t HugeInteger::operator[](size_t idx) const {
	if (idx < size_) {
		return buffer_[idx];
	}
	else {
		return 0;
	}
}

base_t& HugeInteger::operator[](size_t idx) {
	if (idx < capacity_) {
		return buffer_[idx];
	}
	else {
		Realloc(idx + 1);
		return buffer_[idx];
	}
}

HugeInteger HugeInteger::SumPositives(const HugeInteger& lhs,
	const HugeInteger& rhs) {
	const size_t lhs_size = lhs.Size();
	const size_t rhs_size = rhs.Size();
	const size_t max_size = std::max(lhs_size, rhs_size);

	HugeInteger sum;
	sum.Realloc(max_size + 1);

	base_t transfer = 0;
	for (size_t i = 0; i < max_size; ++i) {
		base_t new_digit = lhs[i] + rhs[i] + transfer;
		sum[i] = new_digit % BASE_;
		transfer = new_digit / BASE_;
	}
	sum[max_size] = transfer;
	sum.size_ = (transfer > 0) ? max_size + 1 : max_size;

	return sum;
}

HugeInteger HugeInteger::SubtractPositives(const HugeInteger& lhs, const HugeInteger& rhs) {
	if (lhs < rhs) {
		return -(SubtractPositives(rhs, lhs));
	}

	const size_t lhs_size = lhs.Size();
	const size_t rhs_size = rhs.Size();
	const size_t max_size = lhs_size;

	HugeInteger res;
	res.Realloc(max_size + 1);
	res.is_negative_ = false;
	base_t transfer = 0;
	for (size_t i = 0; i < lhs_size; ++i) {
		if (lhs[i] >= rhs[i] + transfer) {
			res[i] = lhs[i] - rhs[i] - transfer;
			transfer = 0;
		} else {
			res[i] = BASE_ + lhs[i] - rhs[i] - transfer;
			transfer = 1;
		}
		if (res[i] != 0) {
			res.size_ = i + 1;
		}
	}
	return res;
}

HugeInteger::HugeInteger(const HugeInteger& other) {
	size_ = other.size_;
	capacity_ = size_;
	buffer_ = new base_t[capacity_];
	is_negative_ = other.is_negative_;
	CopyFromBufToBuf(other.buffer_, buffer_, other.Size(), size_);
}

HugeInteger::HugeInteger(HugeInteger&& other) {
	buffer_ = other.buffer_;
	size_ = other.size_;
	capacity_ = other.capacity_;
	is_negative_ = other.is_negative_;
	other.buffer_ = nullptr;
}

HugeInteger::HugeInteger(const char* number_str) {
	FillBufferFromString(number_str);
}

template <class IntegerType>
HugeInteger::HugeInteger(IntegerType number) {
	IntegerType num = number >= 0 ? number : -number;
	size_ = 0;
	if (number == 0) {
		size_ = 1;
	}
	else while (num > 0) {
		++size_;
		num /= BASE_;
	}
	num = number >= 0 ? number : -number;
	capacity_ = size_;
	buffer_ = new base_t[capacity_];
	for (size_t i = 0; i < size_; ++i) {
		buffer_[i] = num % BASE_;
		num /= BASE_;
	}
	if (number < 0) {
		is_negative_ = true;
	}
	else {
		is_negative_ = false;
	}
}

HugeInteger& HugeInteger::operator=(const HugeInteger& other) {
	if (buffer_ == other.buffer_) {
		return *this;
	}
	delete[] buffer_;
	capacity_ = other.size_;
	size_ = other.size_;
	buffer_ = new base_t[capacity_];
	CopyFromBufToBuf(other.buffer_, buffer_, other.Size(), size_);
	return *this;

}

HugeInteger& HugeInteger::operator=(HugeInteger&& other) {
	delete[] buffer_;
	buffer_ = other.buffer_;
	other.buffer_ = nullptr;
	size_ = other.size_;
	capacity_ = other.capacity_;
	is_negative_ = other.is_negative_;
	return *this;
}

HugeInteger& HugeInteger::operator++() {
	return (*this += 1);
}

HugeInteger HugeInteger::operator++(int) {
	*this += 1;
	return *this - 1;
}

HugeInteger& HugeInteger::operator--() {
	return (*this -= 1);
}

HugeInteger HugeInteger::operator--(int) {
	*this -= 1;
	return *this + 1;
}

HugeInteger operator-(const HugeInteger& value) {
	HugeInteger new_value = value;
	new_value.is_negative_ = !value.is_negative_;
	return new_value;
}

HugeInteger operator+(const HugeInteger& value) {
	return value;
}


HugeInteger operator+(const HugeInteger& lhs, const HugeInteger& rhs) {
	HugeInteger result;

	if (HaveSameSign(lhs, rhs)) {
		result = HugeInteger::SumPositives(lhs, rhs);
		result.is_negative_ = lhs.IsNegative();
	}
	else if (lhs.IsNegative()) {
		result = HugeInteger::SubtractPositives(rhs, lhs);
	}
	else {
		result = HugeInteger::SubtractPositives(lhs, rhs);
	}

	return result;
}

HugeInteger::~HugeInteger() {
	delete[] buffer_;
}

bool HugeInteger::IsNegative() const {
	return is_negative_;
}

size_t HugeInteger::Size() const {
	return size_;
}

HugeInteger operator-(const HugeInteger& lhs, const HugeInteger& rhs) {
	HugeInteger result;

	if (HaveSameSign(lhs, rhs)) {
		result = HugeInteger::SubtractPositives(lhs, rhs);
		result.is_negative_ = lhs.IsNegative() ? !result.is_negative_ : result.is_negative_;
	}
	else if (lhs.IsNegative()) {
		result = HugeInteger::SumPositives(lhs, rhs);
		result.is_negative_ = true;
	}
	else {
		result = HugeInteger::SumPositives(lhs, rhs);
		result.is_negative_ = false;
	}
	return result;
}

HugeInteger operator*(const HugeInteger& lhs, const HugeInteger& rhs) {
	HugeInteger result;
	result.Realloc(lhs.Size() + rhs.Size() + 3);

	size_t count_size = lhs.Size() + rhs.Size(); 
	ext_base_t transfer = 0;
	for (size_t i = 0; i < count_size; ++i) {
		size_t indLhs = i;
		size_t indRhs = 0;
		ext_base_t mltp = transfer;
		transfer = 0;
		while (indRhs <= i) {
			mltp += lhs[indLhs] * rhs[indRhs];
			--indLhs;
			++indRhs;
		}
		result[i] = mltp % HugeInteger::BASE_;
		transfer = mltp / HugeInteger::BASE_;
	}
	result.size_ = count_size + 2;
	while (result[result.size_ - 1] == 0 && result.size_ > 1) {
		--result.size_;
	}
	result.is_negative_ = HaveSameSign(lhs, rhs) ? false : true;
	return result;
}

HugeInteger operator/(const HugeInteger& lhs, const HugeInteger& rhs) {
	if (abs(lhs) < abs(rhs)) {
		return 0;
	}
	size_t rhs_size = rhs.Size();
	size_t lhs_size = lhs.Size();
	HugeInteger result;
	result.Realloc(lhs.Size() - rhs.Size() + 2);
	size_t count_size = lhs.Size() - rhs.Size();
	HugeInteger tmp;
	tmp.is_negative_ = false;
	tmp.Realloc(rhs.Size() + 1);
	for (size_t i = 0; i < rhs_size - 1; ++i) {
		tmp = tmp * HugeInteger::BASE_ + lhs[lhs_size - 1 - i];
	}
	for (int i = count_size; i >= 0; --i) {
		tmp = tmp * HugeInteger::BASE_ + lhs[i];
		if (tmp < rhs) {
			result[i] = 0;
			continue;
		}

		int ans = HugeInteger::BASE_ - 1;

		while ((rhs * ans) > tmp) {
			--ans;
		}
		result[i] = ans;
		tmp = tmp - (ans * rhs);
	}

	result.size_ = count_size + 1;
	while (result[result.size_ - 1] == 0) {
		--result.size_;
	}
	result.is_negative_ = HaveSameSign(lhs, rhs) ? false : true;
	return result;
}

HugeInteger operator%(const HugeInteger& lhs, const HugeInteger& rhs) {
	return (lhs - (lhs / rhs) * rhs);
}

HugeInteger& HugeInteger::operator+=(const HugeInteger& value) {
	*this = *this + value;
	return *this;
}

HugeInteger& HugeInteger::operator-=(const HugeInteger& value) {
	*this = *this - value;
	return *this;
}

HugeInteger& HugeInteger::operator/=(const HugeInteger& value) {
	*this = *this / value;
	return *this;
}

HugeInteger& HugeInteger::operator*=(const HugeInteger& value) {
	*this = *this * value;
	return *this;
}

HugeInteger& HugeInteger::operator%=(const HugeInteger& value) {
	*this = *this % value;
	return *this;
}


HugeInteger abs(const HugeInteger& value) {
	HugeInteger result = value;
	result.is_negative_ = false;
	return result;
}

bool operator<(const HugeInteger& lhs, const HugeInteger& rhs) {
	if (HaveSameSign(lhs, rhs)) {
		bool ans;
		if (lhs.Size() > rhs.Size()) {
			ans = false;
		}
		else if (lhs.Size() < rhs.Size()) {
			ans = true;
		}
		else {
			for (int i = lhs.Size() - 1; i >= 0; --i) {
				if (lhs[i] < rhs[i]) {
					ans = true;
					break;
				}
				else if (lhs[i] > rhs[i]) {
					ans = false;
					break;
				}
				if (i == 0) {
					ans = false;
				}
			}
			if (lhs.IsNegative()) {
				ans = !ans;
			}
		}
		return ans;
	}
	else if (lhs.IsNegative()) {
		return true;
	}
	else {
		return false;
	}
}

bool operator>(const HugeInteger& lhs, const HugeInteger& rhs) {
	return rhs < lhs;
}

bool operator>=(const HugeInteger& lhs, const HugeInteger& rhs) {
	return (lhs > rhs || lhs == rhs);
}

bool operator<=(const HugeInteger& lhs, const HugeInteger& rhs) {
	return (lhs < rhs || lhs == rhs);
}

bool operator==(const HugeInteger& lhs, const HugeInteger& rhs) {
	if (!HaveSameSign(lhs, rhs)) {
		return false;
	}
	if (lhs.Size() != rhs.Size()) {
		return false;
	}
	for (int i = lhs.Size() - 1; i >= 0; --i) {
		if (lhs[i] != rhs[i]) {
			return false;
		}
	}
	return true;
}

bool operator!=(const HugeInteger& lhs, const HugeInteger& rhs) {
	return !(lhs == rhs);
}


std::istream& operator>>(std::istream& is, HugeInteger& value) {
	const int maxReadSize = 100000;
	char* input = new char[maxReadSize];
	is >> input;

	delete[] value.buffer_;
	value.FillBufferFromString(input);

	delete[] input;

	return is;
}

std::ostream& operator<<(std::ostream& os, const HugeInteger& value) {
	if (value.IsNegative()) {
		os << '-';
	}
	for (int i = value.size_ - 1; i >= 0; --i) {
		if (value[i] > 9 || i == value.size_ - 1) {
			os << value[i];
		}
		else {
			os << 0 << value[i];
		}
	}
	return os;
}

HugeInteger HugeInteger::SearchingSqrt() {
	HugeInteger right = *this + 1;
	HugeInteger left = 0;
	HugeInteger tmp;
	while (right - left > 1) {
		tmp = (right + left) / 2;
		if (tmp * tmp > *this) {
			right = tmp;
		} else {
			left = tmp;
		}
	}
	return left;
}

template <class T>
T SearchingGCD(T a, T b) {
	while (a != 0 && b != 0) {
		if (a > b) {
			a %= b;
		} else {
			b %= a;
		}
	}
	return a + b;
}

int main()
{
	HugeInteger lhs;
	std::cin >> lhs;
	std::cout << lhs.SearchingSqrt() << '\n';
	//system("pause");
	return 0;
}
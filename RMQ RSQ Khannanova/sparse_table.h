#ifndef SPARSE_TABLE_H
#define SPARSE_TABLE_H
#include <utility> 
#include <vector>

size_t Log2(size_t n) {
	size_t answer = 0;
	while (n >>= 1) {
		++answer;
	}
	return answer;
}

template<class T, class BinaryOp>
class SparseTable {
	std::vector<std::vector<T>> table_;
	const BinaryOp op_ = BinaryOp();
public:
	explicit SparseTable(const std::vector<T>& data) {
		table_.push_back(data);
		for (size_t delta = 2; delta <= data.size(); delta *= 2) {
			std::vector<T> temp(data.size() - delta + 1);
			for (size_t i = 0; i < temp.size(); ++i) {
				temp[i] = op_(table_.back()[i], table_.back()[i + delta / 2]);

			}
			table_.push_back(std::move(temp));
		}
	}

	const T& Query(size_t left, size_t right) const;
};

template<class T, class BinaryOp>
const T& SparseTable<T, BinaryOp>::Query(size_t left, size_t right) const {
	size_t k = Log2(right - left);
	T result = op_(table_[k][left], table_[k][right - (1 << k) + 1]);
	return result;
}

#endif
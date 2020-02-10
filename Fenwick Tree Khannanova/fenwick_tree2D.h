#ifndef FENWICK_TREE_H
#define FENWICK_TREE_H

#include <vector>
#include <functional>

template <class T>
class FenwickTree {
	std::vector<std::vector<T>> tree_;
	const std::function<T(const T&, const T&)> op_;
	T neutral_;

public:
	FenwickTree(const std::vector<std::vector<T>>& data, std::function<T(const T&, const T&)> operation, const T& neutral_);
	T Query(int64_t idx, int64_t idy) const;
	T Query(int64_t x, int64_t y, int64_t z, int64_t w) const;
	void Update(size_t idx, size_t idy, const T& value);
	void Print() {
		for (T x : tree_) {
			std::cout << x << ' ';
		}
		std::cout << '\n';
	}
};


template <class T>
T FenwickTree<T>::Query(int64_t idx, int64_t idy) const {
	T answer = neutral_;
	for (int64_t i = idx; i >= 0; i = (i & (i + 1)) - 1) {
		for (int64_t j = idy; j >= 0; j = (j & (j + 1)) - 1) {
			answer = op_(answer, tree_[i][j]);
		}
	}
	return answer;
}

template <class T>
T FenwickTree<T>::Query(int64_t x, int64_t y, int64_t z, int64_t w) const {
	return (Query(z, w) - Query(x - 1, w) - Query(z, y - 1) + Query(x - 1, y - 1));
}

template <class T>
void FenwickTree<T>::Update(size_t idx, size_t idy, const T& value) {
	for (size_t i = idx; i < tree_.size(); i = i | (i + 1)) {
		for (size_t j = idy; j < tree_[0].size(); j = j | (j + 1)) {
			tree_[i][j] = op_(tree_[i][j], value_);
		}
	}
}

template <class T>
FenwickTree<T>::FenwickTree(const std::vector<std::vector<T>>& data, std::function<T(const T&, const T&)> operation, const T& neutral) :
	tree_(data.size(), std::vector<T>(data[0].size(), neutral)), op_(operation), neutral_(neutral) {
	for (size_t i = 0; i < data.size(); ++i) {
		for (size_t j = 0; j < data[0].size(); ++j) {
			Update(i, j, data[i][j]);
		}
	}
}

#endif
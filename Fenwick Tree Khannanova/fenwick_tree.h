#ifndef FENWICK_TREE_H
#define FENWICK_TREE_H

#include <vector>
#include <functional>

template <class T>
class FenwickTree {
	std::vector<T> tree_;
	const std::function<T(const T&, const T&)> op_;
	const T neutral_;

public:
	FenwickTree(const std::vector<T>& data, const std::function<T(const T&, const T&)>& operation, const T& neutral_);
	T Query(int64_t right) const; 
	T Query(int64_t left, int64_t right) const;
	void Update(size_t idx, const T& value);
};


template <class T>
T FenwickTree<T>::Query(int64_t right) const {
	T answer = neutral_;
	for (int64_t i = right; i >= 0; i = (i & (i + 1)) - 1) {
		answer = op_(answer, tree_[i]);
	}
	return answer;
}

template <class T>
T FenwickTree<T>::Query(int64_t left, int64_t right) const {
	return Query(right) - (left >= 1 ? Query(left - 1) : 0);
}

template <class T>
void FenwickTree<T>::Update(size_t idx, const T& value) {
	for (size_t i = idx; i < tree_.size(); i = i | (i + 1)) {
		tree_[i] = op_(tree_[i], value);
	}
}

template <class T>
FenwickTree<T>::FenwickTree(const std::vector<T>& data, std::function<T(const T&, const T&)> operation, const T& neutral) :
	tree_(data.size(), neutral), op_(operation), neutral_(neutral) {
	for (size_t i = 0; i < data.size(); ++i) {
		Update(i, data[i]);
	}
}
#endif
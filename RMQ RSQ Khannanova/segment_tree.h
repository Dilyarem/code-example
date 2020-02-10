#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <vector>
#include <functional>

template <class T>
class SegmentTree {
	std::vector<T> tree_;
	const std::function<T(const T&, const T&)> op_;
	const T neutral_;

	static size_t Parent(int32_t idx);
	static size_t Left(size_t idx);
	static size_t Right(size_t idx);
public:
	SegmentTree(const std::vector<T>& data, const std::function<T(const T&, const T&)>& operation, const T& neutral_);
	T Query(size_t left, size_t right) const;
	void Update(size_t idx, const T& value);
};

size_t ClosestPowerOfTwo(size_t size) {
	size_t powerOfTwo = 1;
	while (powerOfTwo < size) {
		powerOfTwo *= 2;
	}
	return powerOfTwo;
}

template <class T>
T SegmentTree<T>::Query(size_t left, size_t right) const {
	T answer = neutral_;
	left += tree_.size() / 2;
	right += tree_.size() / 2;
	while (left <= right) {
		if (left == Right(Parent(left))) {
			answer = op_(answer, tree_[left]);
		}

		if (right == Left(Parent(right))) {
			answer = op_(answer, tree_[right]);
		}

		left = Parent(left + 1);
		right = Parent(right - 1);
	}

	return answer;
}

template <class T>
void SegmentTree<T>::Update(size_t idx, const T& value) {
	size_t index = idx + tree_.size() / 2;
	tree_[index] = value;
	index = Parent(index);
	while (index > 0) {
		tree_[index] = op_(tree_[Left(index)], tree_[Right(index)]);
		index = Parent(index);
	}
}

template <class T>
SegmentTree<T>::SegmentTree(const std::vector<T>& data, std::function<T(const T&, const T&)> operation, const T& neutral) :
	tree_(2 * ClosestPowerOfTwo(data.size()) + 1, neutral), op_(operation), neutral_(neutral) {
	const size_t size = tree_.size();
	for (size_t i = 0; i < data.size(); ++i) {
		tree_[i + size / 2] = data[i];
	}
	for (int32_t i = size / 2 - 1; i > 0; --i) {
		tree_[i] = op_(tree_[Left(i)], tree_[Right(i)]);
	}
}

template <class T>
size_t SegmentTree<T>::Parent(int32_t idx) {
	return std::max(0, ((idx - 1) / 2));
}

template <class T>
size_t SegmentTree<T>::Left(size_t idx) {
	return idx * 2 + 1;
}

template <class T>
size_t SegmentTree<T>::Right(size_t idx) {
	return idx * 2 + 2;
}

#endif
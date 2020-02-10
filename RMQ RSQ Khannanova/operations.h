#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <algorithm>

template <class T>
struct Max {
	T operator()(const T& lhs, const T& rhs) const {
		return std::max(lhs, rhs);
	}
};

template <class T>
struct Min {
	T operator()(const T& lhs, const T& rhs) const {
		return std::min(lhs, rhs);
	}
};

template <class T>
struct Sum {
	T operator()(const T& lhs, const T& rhs) const {
		return (lhs + rhs);
	}
};

#endif

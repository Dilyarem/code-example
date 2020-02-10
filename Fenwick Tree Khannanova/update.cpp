#include <iostream>
#include "operations.h"
#include "fenwick_tree.h"


int main() {
	size_t size;
	std::cin >> size;
	std::vector<int64_t> data(size);
	for (size_t i = 0; i < data.size(); ++i) {
		std::cin >> data[i];
	}

	FenwickTree<int64_t> st(data, Sum<int64_t>(), 0);

	size_t querNum;
	std::cin >> querNum;

	for (size_t i = 0; i < querNum; ++i) {
		char op;
		std::cin >> op;

		if (op == 's') {
			size_t left, right;
			std::cin >> left >> right;
			std::cout << st.Query(left - 1, right - 1) << ' ';
		}

		if (op == 'u') {
			size_t idx;
			int64_t upd;
			std::cin >> idx >> upd;
			st.Update(idx - 1, upd - data[idx - 1]);
			data[idx - 1] = upd;
		}
	}

	//system("pause");
	return 0;
}
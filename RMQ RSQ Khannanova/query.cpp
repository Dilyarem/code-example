#include <iostream>
#include "sparse_table.h"
#include "operations.h"

int main() {
	int size;
	std::cin >> size;

	std::vector<std::pair<int, int>> array;

	for (int i = 0; i < size; ++i) {
		int next;
		std::cin >> next;
		array.push_back({ next, i + 1 });
	}

	SparseTable<std::pair<int, int>, Max<std::pair<int, int>>> st(array);

	int querNum;
	std::cin >> querNum;

	for (int i = 0; i < querNum; ++i) {
		int left, right;
		std::cin >> left >> right;
		auto ans = st.Query(left - 1, right - 1);
		std::cout << ans.first << ' ' << ans.second << '\n';
	}

	//system("pause");
	return 0;
}
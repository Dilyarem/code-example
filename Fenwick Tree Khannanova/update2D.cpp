#include <iostream>
#include "operations.h"
#include "fenwick_tree2D.h"


int main() {
	size_t comm, size;
	std::cin >> comm >> size;
	FenwickTree<int64_t> ft(std::vector<std::vector<int64_t>>(size, std::vector<int64_t>(size, 0)), Sum<int64_t>(), 0);

	while (true) {
		std::cin >> comm;

		if (comm == 1) {
			int64_t x, y, upd;
			std::cin >> x >> y >> upd;
			ft.Update(x, y, upd);
		}

		if (comm == 2) {
			int64_t x, y, z, w;
			std::cin >> x >> y >> z >> w;
			std::cout << ft.Query(x, y, z, w) << '\n';
		}

		if (comm == 3) {
			break;
		}
	}

	system("pause");
	return 0;
}
#include "MGML.hpp"

#include <iostream>
#include <vector>

int main() {

	std::vector<std::vector<int>> adjx{{1, 2}, {2, 3}, {1}};
	int ny{4};
	auto mx{MGML::MCBM(adjx, ny)};
	for (auto& y : mx) std::cout << y << ' ';
	std::cout << std::endl;



}

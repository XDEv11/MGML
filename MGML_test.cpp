#include "MGML.hpp"

#include <iostream>
#include <vector>

int main() {
	{
		std::cout << "Maximum Cardinality Bipartite Matching\n";
		std::vector<std::vector<int>> adjx{{1, 2}, {2, 3}, {1}};
		int ny{4};
		for (int x{0}; x < int(adjx.size()); ++x) {
			std::cout << x << ": ";
			for (auto& y : adjx[x]) std::cout << y << ' ';
			std::cout << '\n';
		}
		std::cout << std::endl;

		auto mx{MGML::MCBM(adjx, ny)};
		for (auto& y : mx) std::cout << y << ' ';
		std::cout << '\n';
		std::cout << std::endl;
	}

	{
		std::cout << "Maximum Weight Bipartite Matching\n";
		std::vector<std::vector<long long>> weight{
			{8, 4, 6},
			{5, 2, 3},
			{9, 4, 8}
		};
		for (int x{0}; x < int(weight.size()); ++x) {
			for (int y{0}; y < int(weight.size()); ++y) std::cout << weight[x][y] << ' ';
			std::cout << '\n';
		}
		std::cout << std::endl;

		auto mx{MGML::MWBM(weight)};
		for (auto& y : mx) std::cout << y << ' ';
		std::cout << '\n';
		std::cout << std::endl;
	}

	{
		std::cout << "Maximum Cardinality Matching\n";
		std::vector<std::vector<int>> adj{
			{1},
			{2, 4},
			{3, 1},
			{4, 2},
			{1, 3},
		};
		for (int u{0}; u < int(adj.size()); ++u) {
			std::cout << u << ": ";
			for (auto& v : adj[u]) std::cout << v << ' ';
			std::cout << '\n';
		}
		std::cout << std::endl;

		auto m{MGML::MCM(adj)};
		for (auto& v : m) std::cout << v << ' ';
		std::cout << '\n';
		std::cout << std::endl;
	}
}

#ifndef MGML_H
#define MGML_H

#include <vector>
#include <utility> // std::pair

namespace MGML { // Maximum Graph Matching Library
	/* Maximum Cardinality Bipartite Matching */
	std::vector<int> MCBM(const std::vector<std::vector<int>>& adjx, int ny);

	/* Maximum Weight Bipartite Matching */
	std::vector<int> MWBM(const std::vector<std::vector<long long>>& weight);

	/* Maximum Cardinality Matching */
	std::vector<int> MCM(const std::vector<std::vector<int>>& adj);
}

#endif

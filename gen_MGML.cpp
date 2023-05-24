#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <set>
#include <utility>
#include <limits>
#include <numeric>

long long randint(long long l, long long r) {
	static std::default_random_engine engine{std::chrono::system_clock::now().time_since_epoch().count()};
	std::uniform_int_distribution<long long> dist{l, r}; // [l:r]
	return dist(engine);
}

class bf_MCBM {
	int nx, ny;
	std::vector<int> mx{}, my{};
	int dfs(const std::vector<std::vector<int>>& adjx, int x, int c) {
		if (x == nx) return c;
		int mxc{0};
		for (auto& y : adjx[x]) {
			if (my[y] != -1) continue;
			mx[x] = y;
			my[y] = x;
			mxc = std::max(mxc, dfs(adjx, x + 1, c + 1));
			my[y] = -1;
		}
		mx[x] = -1;
		mxc = std::max(mxc, dfs(adjx, x + 1, c));
		return mxc;
	}
public:
	int operator()(const std::vector<std::vector<int>>& adjx, int _ny) { // brute force
		nx = adjx.size(), ny = _ny;
		mx.assign(nx, -1), my.assign(ny, -1);
		return dfs(adjx, 0, 0);
	}
};

void gen_MCBM(std::ostream& os) {
	static bf_MCBM bf{};

	int nx{int(randint(1, 20))}, ny{int(randint(1, 20))};
	std::vector<std::vector<int>> adjx(nx);

	int m{int(randint(0, std::min(nx * ny, 40)))};
	std::set<std::pair<int, int>> edge{};
	while (int(edge.size()) < m) {
		int x, y;
		do {
			x = randint(0, nx - 1), y = randint(0, ny - 1);
		} while (edge.count({x, y}));
		edge.emplace(x, y);
		adjx[x].push_back(y);
	}

	os << nx << '\n';
   	os << ny << '\n';
	for (int x{0}; x < nx; ++x) {
		for (auto& y : adjx[x]) os << y << ' ';
		os << '\n';
	}
	os << bf(adjx, ny)  << '\n';
}

void gen_MWBM(std::ostream& os) {
	int n{int(randint(1, 10))};
	std::vector<std::vector<long long>> weight(n, std::vector<long long>(n));
	for (int x{0}; x < n; ++x)
		for (int y{0}; y < n; ++y) {
			weight[x][y] = randint(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
		}

	long long ans{std::numeric_limits<long long>::min()};
	std::vector<int> mx(n); std::iota(mx.begin(), mx.end(), 0);
	do {
		long long cnt{0};
		for (int x{0}; x < n; ++x) cnt += weight[x][mx[x]];
		ans = std::max(ans, cnt);
	} while (next_permutation(mx.begin(), mx.end()));

	os << n << '\n';
	for (int x{0}; x < int(weight.size()); ++x) {
		for (int y{0}; y < int(weight.size()); ++y) os << weight[x][y] << ' ';
		os << '\n';
	}
	os << ans << '\n';
}

class bf_MCM {
	int n;
	std::vector<int> mh{};
	int dfs(const std::vector<std::vector<int>>& adj, int u, int c) {
		if (u == n) return c;
		int mxc{0};
		if (mh[u] == -1) {
			for (auto& v : adj[u]) {
				if (v < u || mh[v] != -1) continue;
				mh[u] = v;
				mh[v] = u;
				mxc = std::max(mxc, dfs(adj, u + 1, c + 1));
				mh[v] = -1;
			}
			mh[u] = -1;
		}
		mxc = std::max(mxc, dfs(adj, u + 1, c));
		return mxc;
	}
public:
	int operator()(const std::vector<std::vector<int>>& adj) { // brute force
		n = adj.size();
		mh.assign(n, -1);
		return dfs(adj, 0, 0);
	}
};

void gen_MCM(std::ostream& os) {
	static bf_MCM bf{};

	int n{int(randint(1, 20))};
	std::vector<std::vector<int>> adj(n);

	int m{int(randint(0, std::min(n * (n - 1) / 2, 40)))};
	std::set<std::pair<int, int>> edge{};
	while (int(edge.size()) < m) {
		int u, v;
		do {
			u = randint(0, n - 1), v = randint(0, n - 1);
			if (u > v) std::swap(u, v);
		} while (u == v || edge.count({u, v}));
		edge.emplace(u, v);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	os << n << '\n';
	for (int u{0}; u < n; ++u) {
		for (auto& v : adj[u]) os << v << ' ';
		os << '\n';
	}
	os << bf(adj)  << '\n';
}

int main() {
	{
		int n_MCBM{100};
		std::ofstream fs_MCBM{"testcase_MCBM.txt", std::ios_base::out | std::ios_base::trunc};
		fs_MCBM << n_MCBM << '\n';
		while (n_MCBM--) gen_MCBM(fs_MCBM);
	}

	{
		int n_MWBM{100};
		std::ofstream fs_MWBM{"testcase_MWBM.txt", std::ios_base::out | std::ios_base::trunc};
		fs_MWBM << n_MWBM << '\n';
		while (n_MWBM--) gen_MWBM(fs_MWBM);
	}

	{
		int n_MCM{100};
		std::ofstream fs_MCM{"testcase_MCM.txt", std::ios_base::out | std::ios_base::trunc};
		fs_MCM << n_MCM << '\n';
		while (n_MCM--) gen_MCM(fs_MCM);
	}
}

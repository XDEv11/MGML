#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include <numeric>

static std::default_random_engine rand_engine{std::chrono::system_clock::now().time_since_epoch().count()};
long long randint(long long l, long long r) {
	std::uniform_int_distribution<long long> rand_dist{l, r}; // [l:r]
	return rand_dist(rand_engine);
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

std::pair<std::vector<std::vector<int>>, int> gen_MCBM(std::ostream& os, int max_nx, int max_ny, int max_m) {
	int nx{int(randint(1, max_nx))}, ny{int(randint(1, max_ny))};
	std::vector<std::vector<int>> adjx(nx);

	std::vector<std::pair<int, int>> edge{};
	for (int x{0}; x < nx; ++x)
		for (int y{0}; y < ny; ++y) edge.emplace_back(x, y);
	shuffle(edge.begin(), edge.end(), rand_engine);

	int m{int(randint(0, std::min(int(edge.size()), max_m)))};
	edge.resize(m);
	for (auto& [x, y] : edge) adjx[x].push_back(y);

	std::cerr << "MCBM: " << nx << ", " << ny << ", " << m << std::endl;

	os << nx << '\n';
   	os << ny << '\n';
	for (int x{0}; x < nx; ++x) {
		for (auto& y : adjx[x]) os << y << ' ';
		os << '\n';
	}
	return {adjx, ny};
}

std::vector<std::vector<long long>> gen_MWBM(std::ostream& os, int max_n) {
	int n{int(randint(1, max_n))};
	std::vector<std::vector<long long>> weight(n, std::vector<long long>(n));
	for (int x{0}; x < n; ++x)
		for (int y{0}; y < n; ++y) {
			weight[x][y] = randint(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
		}

	std::cerr << "MWBM: " << n << std::endl;

	os << n << '\n';
	for (int x{0}; x < int(weight.size()); ++x) {
		for (int y{0}; y < int(weight.size()); ++y) os << weight[x][y] << ' ';
		os << '\n';
	}
	return weight;
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

std::vector<std::vector<int>> gen_MCM(std::ostream& os, int max_n, int max_m) {
	int n{int(randint(1, max_n))};
	std::vector<std::vector<int>> adj(n);

	std::vector<std::pair<int, int>> edge{};
	for (int u{0}; u < n; ++u)
		for (int v{u + 1}; v < n; ++v) edge.emplace_back(u, v);
	shuffle(edge.begin(), edge.end(), rand_engine);

	int m{int(randint(0, std::min(int(edge.size()), max_m)))};
	edge.resize(m);
	for (auto& [u, v] : edge) adj[u].push_back(v), adj[v].push_back(u);
  
	std::cerr << "MCM: " << n << ", " << m << std::endl;

	os << n << '\n';
	for (int u{0}; u < n; ++u) {
		for (auto& v : adj[u]) os << v << ' ';
		os << '\n';
	}
	return adj;
}

void gen_MCBM_wans(std::ostream& os) {
	auto [adjx, ny]{gen_MCBM(os, 20, 20, 60)};

	static bf_MCBM bf{};
	os << bf(adjx, ny) << '\n';
}

void gen_MCBM_woans(std::ostream& os) {
	gen_MCBM(os, 1500, 1500, 2000000);
}

void gen_MWBM_wans(std::ostream& os) {
	auto weight{gen_MWBM(os, 11)};
	int n{int(weight.size())};

	long long ans{std::numeric_limits<long long>::min()};
	std::vector<int> mx(n); std::iota(mx.begin(), mx.end(), 0);
	do {
		long long cnt{0};
		for (int x{0}; x < n; ++x) cnt += weight[x][mx[x]];
		ans = std::max(ans, cnt);
	} while (next_permutation(mx.begin(), mx.end()));
	os << ans << '\n';
}

void gen_MWBM_woans(std::ostream& os) {
	gen_MWBM(os, 1000);
}

void gen_MCM_wans(std::ostream& os) {
	auto adj{gen_MCM(os, 25, 80)};

	static bf_MCM bf{};
	os << bf(adj)  << '\n';
}

void gen_MCM_woans(std::ostream& os) {
	gen_MCM(os, 1000, 1000000);
}

int main() {
	{
		std::string fn{"testcase_MCBM_wans.txt"};
		std::cout << "Generating " << fn << std::endl;
		int n_MCBM{100};
		std::ofstream fs_MCBM{fn, std::ios_base::out | std::ios_base::trunc};
		fs_MCBM << n_MCBM << '\n';
		while (n_MCBM--) {
			auto b{std::chrono::system_clock::now()};

			gen_MCBM_wans(fs_MCBM);

			auto e{std::chrono::system_clock::now()};
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(e - b).count() << " ms / " << std::flush;
		}
		std::cout << std::endl;
	}

	{
		std::string fn{"testcase_MWBM_wans.txt"};
		std::cout << "Generating " << fn << std::endl;
		int n_MWBM{100};
		std::ofstream fs_MWBM{fn, std::ios_base::out | std::ios_base::trunc};
		fs_MWBM << n_MWBM << '\n';
		while (n_MWBM--) {
			auto b{std::chrono::system_clock::now()};

			gen_MWBM_wans(fs_MWBM);

			auto e{std::chrono::system_clock::now()};
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(e - b).count() << " ms / " << std::flush;
		}
		std::cout << std::endl;
	}

	{
		std::string fn{"testcase_MCM_wans.txt"};
		std::cout << "Generating " << fn << std::endl;
		int n_MCM{100};
		std::ofstream fs_MCM{fn, std::ios_base::out | std::ios_base::trunc};
		fs_MCM << n_MCM << '\n';
		while (n_MCM--) {
			auto b{std::chrono::system_clock::now()};

			gen_MCM_wans(fs_MCM);

			auto e{std::chrono::system_clock::now()};
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(e - b).count() << " ms / " << std::flush;
		}
		std::cout << std::endl;
	}

	{
		std::string fn{"testcase_MCBM_woans.txt"};
		std::cout << "Generating " << fn << std::endl;
		int n_MCBM{100};
		std::ofstream fs_MCBM{fn, std::ios_base::out | std::ios_base::trunc};
		fs_MCBM << n_MCBM << '\n';
		while (n_MCBM--) {
			auto b{std::chrono::system_clock::now()};

			gen_MCBM_woans(fs_MCBM);

			auto e{std::chrono::system_clock::now()};
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(e - b).count() << " ms / " << std::flush;
		}
		std::cout << std::endl;
	}

	{
		std::string fn{"testcase_MWBM_woans.txt"};
		std::cout << "Generating " << fn << std::endl;
		int n_MWBM{100};
		std::ofstream fs_MWBM{fn, std::ios_base::out | std::ios_base::trunc};
		fs_MWBM << n_MWBM << '\n';
		while (n_MWBM--) {
			auto b{std::chrono::system_clock::now()};

			gen_MWBM_woans(fs_MWBM);

			auto e{std::chrono::system_clock::now()};
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(e - b).count() << " ms / " << std::flush;
		}
		std::cout << std::endl;
	}

	{
		std::string fn{"testcase_MCM_woans.txt"};
		std::cout << "Generating " << fn << std::endl;
		int n_MCM{100};
		std::ofstream fs_MCM{fn, std::ios_base::out | std::ios_base::trunc};
		fs_MCM << n_MCM << '\n';
		while (n_MCM--) {
			auto b{std::chrono::system_clock::now()};

			gen_MCM_woans(fs_MCM);

			auto e{std::chrono::system_clock::now()};
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(e - b).count() << " ms / " << std::flush;
		}
		std::cout << std::endl;
	}
}

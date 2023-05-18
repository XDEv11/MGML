#include "MGML.hpp"

#include <vector>
#include <utility> // std::pair, std::move()
#include <queue>
#include <limits>
#include <algorithm>
#include <numeric> // std::iota()

namespace MGML {
	namespace {
		// Hopcroft–Karp algorithm - O(E√V)
		class Hopcroft_Karp {
			mutable std::vector<std::vector<int>> adjy;
			mutable std::vector<int> dx, dy, mx, my;
			bool dfs(const std::vector<std::vector<int>>& adjx, int y) const {
				for (auto& x : adjy[y]) {
					if (dx[x] + 1 != dy[y]) continue;
					if (mx[x] == -1 || dfs(adjx, mx[x])) {
						my[y] = x, mx[x] = y;
						dy[y] = -1; // remove from bfs forest
						return true;
					}
				}
				dy[y] = -1; // remove from bfs forest
				return false;
			}
			bool bfs(const std::vector<std::vector<int>>& adjx) const {
				std::fill(dx.begin(), dx.end(), -1);
				std::fill(dy.begin(), dy.end(), -1);
				std::queue<int> qu1{}, qu2{};
				for (int x{0}; x < int(adjx.size()); ++x) // use all unmatched x's as roots
					if (mx[x] == -1) qu1.push(x), dx[x] = 0;

				bool found{false};
				while (!qu1.empty() && !found) { // stop at the level found
					while (!qu1.empty()) {
						auto x{qu1.front()}; qu1.pop();
						for (auto& y : adjx[x])
							if (dy[y] == -1 && my[y] != x) {
								dy[y] = dx[x] + 1;
								if (my[y] == -1) found = true;
								else qu2.push(my[y]), dx[my[y]] = dy[y] + 1;
							}
					}
					qu1.swap(qu2);
				}
				return found;
			}
		public:
			std::pair<std::vector<int>, std::vector<int>> operator()(const std::vector<std::vector<int>>& adjx, int ny) const {
				adjy.assign(ny, {});
				for (int x{0}; x < int(adjx.size()); ++x)
					for (auto& y : adjx[x]) adjy[y].push_back(x);
				dx.resize(adjx.size()), dy.resize(ny), mx.assign(adjx.size(), -1), my.assign(ny, -1);

				int c{0};
				while (bfs(adjx)) {
					for (int y{0}; y < ny; ++y)
						if (my[y] == -1 && dy[y] != -1) c += dfs(adjx, y);
				}
				return {std::move(mx), std::move(my)};
			}
		};

		// Hungarian Algorithm - O(V^3)
		class Hungarian {
			mutable int n;
			mutable std::vector<long long> lx{}, ly{};
			mutable std::vector<bool> vx{}, vy{};
			mutable std::queue<int> qu{}; // only X's vertices
			mutable std::vector<int> py{};
			mutable std::vector<long long> dy{}; // smallest (lx[x] + ly[y] - w[x][y])
			mutable std::vector<int> pdy{}; // & which x
			mutable std::vector<int> mx{}, my{};
			void relax(const std::vector<std::vector<long long>>& w, int x) const {
				for (int y{0}; y < n; ++y)
					if (lx[x] + ly[y] - w[x][y] < dy[y])
						dy[y] = lx[x] + ly[y] - w[x][y], pdy[y] = x;
			}
			void augment(int y) const {
				while (y != -1) {
					int x{py[y]}, yy{mx[x]};
					mx[x] = y, my[y] = x;
					y = yy;
				}
			}
			bool bfs(const std::vector<std::vector<long long>>& w) const {
				while (!qu.empty()) {
					int x{qu.front()}; qu.pop();
					for (int y{0}; y < n; ++y) {
						if (!vy[y] && lx[x] + ly[y] == w[x][y]) {
							vy[y] = true, py[y] = x;
							if (my[y] == -1) return augment(y), true;
							int xx{my[y]};
							qu.push(x), vx[xx] = true, relax(w, xx);
						}
					}
				}
				return false;
			}
			void relabel() const {
				long long d{std::numeric_limits<long long>::max()};
				for (int y{0}; y < n; ++y) if (!vy[y]) d = std::min(d, dy[y]);
				for (int x{0}; x < n; ++x) if (vx[x]) lx[x] -= d;
				for (int y{0}; y < n; ++y) if (vy[y]) ly[y] += d;
				for (int y{0}; y < n; ++y) if (!vy[y]) dy[y] -= d;
			}
			bool expand(const std::vector<std::vector<long long>>& w) const { // expand one layer with new equality edges
				for (int y{0}; y < n; ++y) {
					if (!vy[y] && dy[y] == 0) {
						vy[y] = true, py[y] = pdy[y];
						if (my[y] == -1) return augment(y), true;
						int xx{my[y]};
						qu.push(xx), vx[xx] = true, relax(w, xx);
					}
				}
				return false;
			}
		public:
			std::pair<std::vector<int>, std::vector<int>> operator()(const std::vector<std::vector<long long>>& w) const {
				n = w.size(), lx.assign(n, 0), ly.assign(n, 0), vx.resize(n), vy.resize(n), py.resize(n), dy.resize(n), pdy.resize(n), mx.assign(n, -1), my.assign(n, -1);
				for (int i{0}; i < n; ++i)
					for (int j{0}; j < n; ++j)
						lx[i] = std::max(lx[i], w[i][j]);

				for (int x{0}; x < n; ++x) {
					std::fill(vx.begin(), vx.end(), false);
					std::fill(vy.begin(), vy.end(), false);
					std::fill(dy.begin(), dy.end(), std::numeric_limits<long long>::max());

					qu = {}, qu.push(x), vx[x] = true, relax(w, x);
					while (!bfs(w)) {
						relabel();
						if (expand(w)) break;
					}
				}

				return {std::move(mx), std::move(my)};
			}
		};

		// Edmonds' Blossom Algorithm - O(VEa(V))
		class Edmonds_Blossom {
			mutable int label{0};
			mutable std::vector<int> p{}, d{}, a{}, c1{}, c2{}, m{};
			// (alternating tree), (unvisited: -1, even: 0, odd: 1), (auxiliary for lca), (cross edge)
			/* DSU */
			mutable std::vector<int> dsu_p{}, dsu_sz{}, dsu_b{};
			void dsu_reset() const {
				std::fill(dsu_p.begin(), dsu_p.end(), -1);
				std::fill(dsu_sz.begin(), dsu_sz.end(), 1);
				std::iota(dsu_b.begin(), dsu_b.end(), 0);
			}
			int find(int x) const { // collapsing find
				return dsu_p[x] == -1 ? x : dsu_p[x] = find(dsu_p[x]);
			}
			int base(int x) const { return dsu_b[find(x)]; }
			void contract(int x, int y) const { // weighted union
				auto rx{find(x)}, ry{find(y)};
				if (rx == ry) return ;
				auto b{dsu_b[rx]}; // treat x's base as new base
				if (dsu_sz[rx] < dsu_sz[ry]) std::swap(rx, ry);
				dsu_p[ry] = rx, dsu_sz[rx] += dsu_sz[ry], dsu_b[rx] = b;
			}
			/*******/
			mutable std::queue<int> qu{}; // only even vertices
			void handle_one_side(int x, int y, int b) const {
				for (int v{base(x)}; v != b; v = base(p[v])) {
					c1[v] = x, c2[v] = y, contract(b, v);
					if (d[v] == 1) qu.push(v); // odd vertex -> even vertex
				}
			}
			int lca(int u, int v) const {
				++label; // use next number in order not to clear a
				while (true) {
					if (a[u] == label) return u;
					a[u] = label;
					if (p[u] != -1) u = base(p[u]);
					std::swap(u, v);
				}
			}
			void augment(int r, int y) const {
				if (r == y) return ;
				if (d[y] == 0) { // even vertex -> odd vertex
					// check d[y] == 0 instead of d[p[y]] == 1, so (m[y], y) is in the blossom
					augment(m[y], m[c1[y]]);
					augment(r, m[c2[y]]);
					m[c1[y]] = c2[y], m[c2[y]] = c1[y];
				} else {
					int x{p[y]};
					augment(r, m[x]);
					m[x] = y, m[y] = x;
				}
			}
			bool bfs(const std::vector<std::vector<int>>& adj, int r) const {
				dsu_reset();
				std::fill(d.begin(), d.end(), -1);

				qu = {}, qu.push(r), d[r] = 0;
				while (!qu.empty()) {
					int x{qu.front()}; qu.pop();
					for (auto& y : adj[x]) {
						if (base(x) == base(y)) continue;
						if (d[y] == -1) {
							p[y] = x, d[y] = 1;
							if (m[y] == -1) { // augmenting path
								augment(-1, y);
								return true;
							} else {
								p[m[y]] = y, d[m[y]] = 0;
								qu.push(m[y]);
							}
						} else if (d[base(y)] == 0) { // blossom
							int b{lca(base(x), base(y))};
							handle_one_side(x, y, b);
							handle_one_side(y, x, b);
						}
					}
				}
				return false;
			}
		public:
			std::vector<int> operator()(const std::vector<std::vector<int>>& adj) const {
				label = 0, p.assign(adj.size(), -1), d.resize(adj.size()), a.resize(adj.size()), c1.resize(adj.size()), c2.resize(adj.size()), m.assign(adj.size(), -1);
				dsu_p.resize(adj.size()), dsu_sz.resize(adj.size()), dsu_b.resize(adj.size());

				int c{0};
				// find random greey matching first
				std::vector<int> X(adj.size()); std::iota(X.begin(), X.end(), 0);
				std::random_shuffle(X.begin(), X.end());
				for (auto& x : X) {
					if (m[x] != -1) continue;
					for (auto& y : adj[x])
						if (m[y] == -1) {
							m[x] = y, m[y] = x, ++c;
							break;
						}
				}
				for (int v{0}; v < int(adj.size()); ++v)
					if (m[v] == -1 && bfs(adj, v)) ++c;
				return std::move(m);
			}
		};

	} // anonymous

	std::vector<int> MCBM(const std::vector<std::vector<int>>& adjx, int ny) {
		Hopcroft_Karp fctr{};
		auto [mx, my]{fctr(adjx, ny)};
		return mx;
	}

	std::vector<int> MWBM(const std::vector<std::vector<long long>>& weight) {
		Hungarian fctr{};
		auto [mx, my]{fctr(weight)};
		return mx;
	}

	std::vector<int> MCM(const std::vector<std::vector<int>>& adj) {
		Edmonds_Blossom fctr{};
		auto m{fctr(adj)};
		return m;
	}
}

# MGML

Maximum Graph Matching Library (NSD 2023 Spring NYCU Term Project)

## C++ API (`MGML.hpp`)

All vertices are numbered from 0.

### `std::vector<int> MGML::MCBM(const std::vector<std::vector<int>>& adjx, int ny);`

> Maximum Cardinality Bipartite Matching

* Parameter
    * Give `adjx` that for all `x` in $X$, `adjx[x]` stores all vertices in the other part, $Y$, connected to `x` and the number of vertices in $Y$, `ny`.
* Return Value
    * Return `mx` such that `mx[x]` is the matched vertex to `x` or -1 if unmatched.

### `std::vector<int> MGML::MWBM(const std::vector<std::vector<long long>>& weight);`

> Maximum Weight Bipartite Matching

* Parameter
    * Give a $n$ by $n$ matrix, $weight$.
* Return Value
    * Return `mx` such that `mx[x]` is the matched vertex to `x`

### `std::vector<int> MGML::MCM(const std::vector<std::vector<int>>& adj);`

> Maximum Cardinality Matching

* Parameter
    * Give `adj` that for all `u`, `adj[u]` stores all vertices connected to `u`.
* Return Value
    * Return `mh` such that `mh[u]` is the matched vertex to `u` or -1 if unmatched.

## Python API

The module name is `MGML` and all vertices are also numbered from 0.

> The Python API are basically almost the same as C++ API.

### `MGML.MCBM(adjx, ny)`

### `MGML.MWBM(weight);`

### `MGML.MCM(adj)`


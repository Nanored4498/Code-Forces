#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>
#include <deque>
#include <cassert>
#include <queue>

using namespace std;
using ll = long long;
using vi = vector<int>;

constexpr int MAXN = 10;
constexpr int MAXQ = 1'000'000;
constexpr double tau = 3;
constexpr double gamma = .25;
constexpr int MAXP = 100'000;

int N, Q, K;
array<int, MAXN> L, Qmin, Qexp, lastU;
array<array<int, MAXP>, MAXN> loc, last, counter, lastexp;
array<deque<pair<int, int>>, MAXN> preAll;
array<deque<pair<int, int>>, MAXN> lruexp;
array<double, MAXN> E;
array<int, MAXN> Eexp;
array<int, MAXN> Qt;
int T;
int largest_wait = 3;

constexpr int LRU = 0;
constexpr int LFU = 1;
constexpr int MFU = 2;
constexpr int MK = 3;

struct PageEvict {
	int K, u, q = 0;
	vi lrm;
	deque<pair<int, int>> lru;
	using QEl3 = array<int, 3>;
	priority_queue<QEl3, vector<QEl3>, greater<QEl3>> Q3;

	PageEvict() = default;
	PageEvict(int K, int u): K(K), u(u) {}

	void clear() {
		lrm.clear();
		lru.clear();
		Q3 = {};
	}

	void clean(const array<bool, MAXP> &in) {
		if(K == LRU) {
			while(!lru.empty()) {
				const auto [m, p] = lru.front();
				if(in[p] && last[u][p] == m) return;
				lru.pop_front();
			}
		} else {
			while(!Q3.empty()) {
				const auto [k, m, p] = Q3.top();
				if(in[p] && m == last[u][p]) return;
				Q3.pop();
			}
		}
	}

	int worst(const array<bool, MAXP> &in) {
		clean(in);
		if(K == LRU) {
			if(!lru.empty()) return lru.front().second;
		} else {
			if(!Q3.empty()) return Q3.top()[2];
		}
		return -1;
	}

	void remove(int p, const array<bool, MAXP> &in) {
		lrm.push_back(p);
		-- q;
		clean(in);
	}

	void add(int p, const array<bool, MAXP> &in) {
		if(K == LRU) {
			lru.emplace_back(last[u][p], p);
		} else if(K == LFU) {
			Q3.push({counter[u][p], last[u][p], p});
		} else if(K == MFU) {
			Q3.push({-counter[u][p], last[u][p], p});
		}
		if(!in[p]) ++ q;
	}

	void addlast(array<bool, MAXP> &in) {
		while(!lrm.empty()) {
			const int p = lrm.back(); lrm.pop_back();
			if(in[p]) continue;
			if(K == LRU) {
				lru.emplace_front(last[u][p], p);
			} else if(K == LFU) {
				Q3.push({counter[u][p], last[u][p], p});
			} else if(K == MFU) {
				Q3.push({-counter[u][p], last[u][p], p});
			}
			++ q;
			in[p] = true;
			clean(in);
			return;
		}
	}
};

double eval(int u) {
	if(T - lastU[u] > 2*largest_wait) return -1e3;
	if(E[u] <= Eexp[u] && Qt[u] > Qexp[u]) return -3. - (Eexp[u] - E[u]) - (Qt[u] - Qexp[u]);
	const double rho = max(0., E[u] / Eexp[u] - 1.) + 1e-4 * E[u] / Eexp[u];
	return L[u] * rho * Qexp[u] / (double(Qt[u]) * Qt[u]);
}

struct UserEvict {
	array<array<bool, MAXP>, MAXN> in;
	array<array<array<bool, MAXP>, MK>, MAXN> in0;
	array<array<int, MK>, MAXN> fault0;
	array<array<PageEvict, MK>, MAXN> pes0, pes1;

	UserEvict() = default;

	void init() {
		for(int u = 0; u < N; ++u) {
			E[u] = 0.;
			for(int p = 0; p < MAXP; ++p) in[u][p] = false;
			for(int k = 0; k < MK; ++k) {
				for(int p = 0; p < MAXP; ++p) in0[u][k][p] = false;
				fault0[u][k] = 0;
				pes0[u][k] = PageEvict(k, u);
				pes1[u][k] = PageEvict(k, u);
			}
		}
	}

	int bestK(int u) {
		int bk = 0;
		for(int k = 0; k < MK; ++k) if(fault0[u][k] < fault0[u][bk]) bk = k;
		return bk;
	}

	void evictPage0(int u, int k) {
		const int p = pes0[u][k].worst(in0[u][k]);
		in0[u][k][p] = false;
		pes0[u][k].remove(p, in0[u][k]);
	}

	void autoEvict(int u, int p, int op) {
		in[u][op] = false;
		for(int k = 0; k < MK; ++k) {
			if(pes0[u][k].q >= pes1[u][k].q && !in0[u][k][p]) evictPage0(u, k);
			pes1[u][k].remove(op, in[u]);
		}
	}

	void otherEvict(int u, int p, int lu, int op) {
		if(u == lu) return autoEvict(u, p, op);
		in[lu][op] = false;
		for(int k = 0; k < MK; ++k) {
			if(pes0[lu][k].q > pes1[lu][k].q) evictPage0(lu, k);
			pes1[lu][k].remove(op, in[lu]);
		}
	}

	void autoEvict2(int u, int p) {
		for(int k = 0; k < MK; ++k)
			if(pes0[u][k].q >= pes1[u][k].q && !in0[u][k][p])
				evictPage0(u, k);
	}

	void updateE0(int u, int p) {
		for(int k = 0; k < MK; ++k) if(!in0[u][k][p]) ++ fault0[u][k];
	}

	pair<int, int> f(int u, int p) {
		updateE0(u, p);

		pair<int, int> ans = {-1, -1};
		if(!in[u][p]) {
			if(preAll[u].empty() || preAll[u].front().second >= T) {
				E[u] += 1 + gamma * tau;
				++ Qt[u];
				int lu = -1;
				double bscore = 1e9;
				for(int u2 = 0; u2 < N; ++u2) if(Qt[u2] > Qmin[u2] && Qt[u2] > preAll[u2].size()) {
					const double sc = eval(u2);
					if(lu == -1 || sc < bscore) {
						bscore = sc;
						lu = u2;
					}
				}
				assert(lu != -1);
				-- Qt[lu];
				const int op = pes1[lu][bestK(lu)].worst(in[lu]);
				otherEvict(u, p, lu, op);
				ans = {lu, op};
			} else {
				ans.first = preAll[u].front().first;
				E[u] += 1 + gamma * max(0., tau - (T - preAll[u].front().second));
				preAll[u].pop_front();
			}
		} else {
			autoEvict2(u, p);
			ans = {u, p};
		}

		return ans;
	}

	void g(int u, int p) {
		for(int k = 0; k < MK; ++k) {
			pes0[u][k].add(p, in0[u][k]);
			in0[u][k][p] = true;
			pes0[u][k].clean(in0[u][k]);
			pes1[u][k].add(p, in[u]);
			if(pes0[u][k].q < pes1[u][k].q) pes0[u][k].addlast(in0[u][k]);
		}
		in[u][p] = true;
		for(int k = 0; k < MK; ++k) pes1[u][k].clean(in[u]);
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> N >> Q >> K;
	for(int i = 0; i < N; ++i) cin >> L[i];
	for(int i = 0; i < N; ++i) cin >> Qmin[i] >> Qexp[i];

	for(int u = 0; u < N; ++u) {
		for(int p = 0; p < MAXP; ++p)	
			loc[u][p] = counter[u][p] = lastexp[u][p] = 0;
		lastU[u] = 0;
		E[u] = 0.;
	}

	{ // PREALOC
	vector<bool> rm(MAXN, true);
	int q = Q;
	int SL = 0;
	for(int i = 0; i < N; ++i) SL += L[i];
	while(true) {
		for(int i = 0; i < N; ++i) if(rm[i])
			Qt[i] = double(L[i])/SL * q;
		bool ok = true;
		for(int i = 0; i < N; ++i) if(rm[i] && Qt[i] < Qmin[i]) {
			rm[i] = false;
			q -= Qmin[i];
			Qt[i] = Qmin[i];
			ok = false;
			SL -= L[i];
		}
		if(ok) break;
	}
	while(true) {
		for(int i = 0; i < N; ++i) if(rm[i])
			Qt[i] = double(L[i])/SL * q;
		bool ok = true;
		for(int i = 0; i < N; ++i) if(rm[i] && Qt[i] > MAXP) {
			rm[i] = false;
			q -= MAXP;
			Qt[i] = MAXP;
			ok = false;
			SL -= L[i];
		}
		if(ok) break;
	}
	while(true) {
		for(int i = 0; i < N; ++i) if(rm[i])
			Qt[i] = double(L[i])/SL * q;
		bool ok = true;
		for(int i = 0; i < N; ++i) if(rm[i] && Qt[i] > MAXP) {
			rm[i] = false;
			q -= MAXP;
			Qt[i] = MAXP;
			ok = false;
			SL -= L[i];
		}
		if(ok) break;
	}
	for(int i = 0; i < N; ++i) if(rm[i]) q -= Qt[i];
	for(int i = 0; i < N; ++i) while(q && Qt[i] < MAXP) {
		--q;
		++Qt[i];
	}
	cout << Q-q;
	q = 0;
	for(int i = 0; i < N; ++i) for(int j = 0; j < Qt[i]; ++j) {
		++q;
		cout << ' ' << q << ' ' << i+1;
		preAll[i].emplace_back(q, -tau);
	}
	cout << endl;
	}


	UserEvict ue;
	ue.init();
	while(true) {
		int flag;
		cin >> flag;
		if(flag > 0) {
			int u, p;
			cin >> u >> p;
			-- u; -- p;
			T = flag;

			largest_wait = max(largest_wait, T - lastU[u]);
			lastU[u] = T;

			// base simulation
			if(!lastexp[u][p]) {
				if(++ Eexp[u] > Qexp[u]) {
					while(true) {
						const auto [op, ot] = lruexp[u].front();
						lruexp[u].pop_front();
						int &t = lastexp[u][op];
						if(t != ot) continue;
						t = 0;
						break;
					}
				}
			}
			lastexp[u][p] = T;
			lruexp[u].emplace_back(p, T);

			auto [u2, p2] = ue.f(u, p);
			if(p2 == -1) {
				loc[u][p] = u2;
			} else {
				swap(loc[u][p], loc[u2][p2]);
			}
			last[u][p] = T;
			++ counter[u][p];
			ue.g(u, p);
			cout << loc[u][p] << endl;
		} else if(flag) break;
		else {
			++ T;
			vector<pair<int, int>> pre;
			for(int u = 0; u < N; ++u) if(Eexp[u] > Qexp[u] && E[u] <= Eexp[u]) {
				while(Qt[u] > Qexp[u] && Qt[u] > preAll[u].size() && pre.size() < K) {
					const int p = ue.pes1[u][ue.bestK(u)].worst(ue.in[u]);
					ue.otherEvict(-1, -1, u, p);
					pre.emplace_back(loc[u][p], 0);
					loc[u][p] = 0;
					-- Qt[u];
				}
			}
			for(int u = 0; u < N; ++u) if(T - lastU[u] > 2*largest_wait) {
				while(Qt[u] > Qmin[u] && Qt[u] > preAll[u].size() && pre.size() < K) {
					const int p = ue.pes1[u][ue.bestK(u)].worst(ue.in[u]);
					ue.otherEvict(-1, -1, u, p);
					pre.emplace_back(loc[u][p], 0);
					loc[u][p] = 0;
					-- Qt[u];
				}
			}
			for(auto &[p, u] : pre) {
				u = -1;
				double bscore = -1e9;
				for(int u2 = 0; u2 < N; ++u2) if(Qt[u2] < MAXP) {
					const double sc = eval(u2);
					if(u == -1 || sc > bscore) {
						bscore = sc;
						u = u2;
					}
				}
				++ Qt[u];
			}
			while(pre.size() < K) {
				int u0 = -1, u1 = -1;
				double bscore0 = 1e9, bscore1 = 0;
				for(int u2 = 0; u2 < N; ++u2) if(Eexp[u2] > Qexp[u2] && Qt[u2] > Qmin[u2] && Qt[u2] > preAll[u2].size()) {
					const double sc = eval(u2);
					if(sc < bscore0) {
						bscore0 = sc;
						u0 = u2;
					}
				}
				for(int u2 = 0; u2 < N; ++u2) if(Qt[u2] < MAXP) {
					const double sc = eval(u2);
					if(u1 == -1 || sc > bscore1) {
						bscore1 = sc;
						u1 = u2;
					}
				}
				if(4 * bscore0 < gamma * bscore1) {
					const int p = ue.pes1[u0][ue.bestK(u0)].worst(ue.in[u0]);
					ue.otherEvict(-1, -1, u0, p);
					pre.emplace_back(loc[u0][p], u1);
					loc[u0][p] = 0;
					-- Qt[u0];
					++ Qt[u1];
				} else break;
			}
			cout << pre.size();
			for(const auto [p, u] : pre) {
				cout << ' ' << p << ' ' << u+1;
				preAll[u].emplace_back(p, T);
			}
			cout << endl;
		}
	}

	return 0;
}
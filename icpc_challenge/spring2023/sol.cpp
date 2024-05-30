#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <array>
#include <cmath>
#include <queue>
#include <cassert>
#include <tuple>

using namespace std;
using ll = long long;
using vi = vector<int>;

const int MAXN = 10;

const int LRU = 0;
const int LFU = 1;
const int MFU = 2;
const int MK = 3;
const int ME = 6;
// E = 3 not so usefull

int N, Q, M;
array<int, MAXN> L, D;
array<array<int, 3>, MAXN> Qm;
array<int, MAXN> MU, SLAb, lastU, bestHit;
array<vi, MAXN> counter, first, last;
int largest_wait = 0;
int smallest_wait = 1e9;

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

	void clean(const vector<bool> &in) {
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

	int worst(const vector<bool> &in) {
		clean(in);
		if(K == LRU) {
			if(!lru.empty()) return lru.front().second;
		} else {
			if(!Q3.empty()) return Q3.top()[2];
		}
		return -1;
	}

	void remove(int p, const vector<bool> &in) {
		lrm.push_back(p);
		-- q;
		clean(in);
	}

	void add(int p, const vector<bool> &in) {
		if(K == LRU) {
			lru.emplace_back(last[u][p], p);
		} else if(K == LFU) {
			Q3.push({counter[u][p], last[u][p], p});
		} else if(K == MFU) {
			Q3.push({-counter[u][p], last[u][p], p});
		}
		if(!in[p]) ++ q;
	}

	void addlast(vector<bool> &in) {
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

struct UserEvict {
	int nc = 0;

	array<int, MAXN> Qt, SLAa;
	array<double, MAXN> RDa, RDb, RDc;
	array<pair<double, double>, MAXN> subRa, subRb;
	array<deque<pair<double, double>>, MAXN> subsRa, subsRb;

	array<vector<bool>, MAXN> in;
	array<array<bool, MK>, MAXN> active;
	array<array<vector<bool>, MK>, MAXN> in0;
	array<array<int, MK>, MAXN> SLAa0;
	array<array<PageEvict, MK>, MAXN> pes0, pes1;

	UserEvict() = default;

	void init() {
		for(int u = 0; u < N; ++u) {
			Qt[u] = SLAa[u] = 0;
			RDa[u] = RDb[u] = RDc[u] = 0.;
			subRa[u] = subRb[u] = {0., 0.};
			in[u].assign(D[u], false);
			for(int k = 0; k < MK; ++k) {
				active[u][k] = true;
				in0[u][k].assign(D[u], false);
				SLAa0[u][k] = 0;
				pes0[u][k] = PageEvict(k, u);
				pes1[u][k] = PageEvict(k, u);
			}
		}
	}

	void newReq(int u) {
		if(Qt[u]) {
			const double d = max(.02, double(Qt[u]) / double(Qm[u][1])); 
			RDa[u] += d;
			if(SLAa[u] < Qm[u][1]) subsRa[u].emplace_back(0., d);
			else if(SLAa[u] > 1.15*Qm[u][1] && !subsRa[u].empty()) {
				auto [n, d] = subsRa[u].front(); subsRa[u].pop_front();
				subRa[u].first -= n;
				subRa[u].second -= d;
			}
		}
		if(SLAb[u]) {
			const double d = min(1., .06 + double(SLAb[u]) / double(Qm[u][1]));
			RDb[u] += d;
			if(SLAb[u] < Qm[u][1]) subsRb[u].emplace_back(0., d);
			else if(SLAb[u] > 1.15*Qm[u][1] && !subsRb[u].empty()) {
				auto [n, d] = subsRb[u].front(); subsRb[u].pop_front();
				subRb[u].first -= n;
				subRb[u].second -= d;
			}
		}
		if(SLAb[u]) {
			const double d = min(1., .35 + double(SLAb[u]) / double(Qm[u][1]));
			RDc[u] += d;
		}
	}

	void newHitB(int u) {
		if(SLAb[u] < Qm[u][1]) subsRb[u].back().first = 1.;
	}

	int bestK(int m, int u, int E) {
		int bk = 0;
		for(int k = 0; k < MK; ++k) if(active[u][k])
			if(SLAa0[u][k] < SLAa0[u][bk])
				bk = k;
		if(m > M/3) {
			if(active[u][MFU])
				if(max(SLAa0[u][MFU] - SLAb[u], 0) > 1.42*max(SLAa0[u][bk] - SLAb[u], 0)+.01*SLAb[u]) {
					active[u][MFU] = false;
					in0[u][MFU].clear();
					pes0[u][MFU].clear();
					pes1[u][MFU].clear();
				}
		}
		return E == 5 ? LRU : bk;
	}

	void evictPage0(int u, int k) {
		const int p = pes0[u][k].worst(in0[u][k]);
		in0[u][k][p] = false;
		pes0[u][k].remove(p, in0[u][k]);
	}

	void autoEvict(int u, int p, int op) {
		in[u][op] = false;
		for(int k = 0; k < MK; ++k) if(active[u][k]) {
			if(pes0[u][k].q >= Qt[u] && !in0[u][k][p]) evictPage0(u, k);
			pes1[u][k].remove(op, in[u]);
		}
	}

	void otherEvict(int u, int p, int lu, int op) {
		if(u == lu) return autoEvict(u, p, op);
		in[lu][op] = false;
		for(int k = 0; k < MK; ++k) if(active[lu][k]) {
			if(pes0[lu][k].q > Qt[lu]) evictPage0(lu, k);
			pes1[lu][k].remove(op, in[lu]);
		}
	}

	void autoEvict2(int u, int p) {
		for(int k = 0; k < MK; ++k) if(active[u][k])
			if(pes0[u][k].q >= Qt[u] && !in0[u][k][p])
				evictPage0(u, k);
	}

	void updateSLAa0(int u, int p) {
		for(int k = 0; k < MK; ++k) if(active[u][k] && !in0[u][k][p]) ++SLAa0[u][k];
	}

	pair<int, int> f(int m, int u, int p, int E) {
		updateSLAa0(u, p);

		pair<int, int> ans = {-1, -1};
		if(!in[u][p]) {
			++ SLAa[u];
			if(Qt[u] >= Qm[u][2]) {
				const int op = pes1[u][bestK(m, u, E)].worst(in[u]);
				autoEvict(u, p, op);
				ans = {u, op};
			} else if(nc < Q) {
				++ Qt[u];
				ans.first = ++nc;
			} else {
				++ Qt[u];
				int T0 = 0;
				for(int u2 = 0; u2 < N; ++u2) if(Qt[u2] > Qm[u2][0])
					T0 = max(T0, pes1[u2][LRU].lru[int(pes1[u2][LRU].lru.size()*.35)].first);
				const int UU0 = u;
				const auto eval = [&](int u) {
					if(E == ME) {
						if(u == UU0) {
							if(Qm[u][1] > Qm[u][2]) return 1.;
							if(Qt[u] < Qm[u][1] && SLAa[u] >= SLAb[u]) return 1.;
							if(m + smallest_wait >= M) return 1.;
							return -1.;
						}
						return 0.;
					}
					if(m - lastU[u] > 2*largest_wait) return -1e3;
					if(SLAa[u] <= SLAb[u] && Qt[u] > Qm[u][1]) return -3. - (SLAb[u] - SLAa[u]) - (Qt[u] - Qm[u][1]);
					if(pes1[u][LRU].lru.back().first < T0) return - double(m - pes1[u][LRU].lru.back().first) / m - 1.;
					if(E == 5) {
						// return (double) pes1[u][LRU].lru.front().first;
						if(SLAb[u] < .1*MU[u] && m - pes1[u][LRU].lru.front().first > .96*bestHit[u]) return - 1.;
					} else if(E >= 2 && SLAb[u] < .1*MU[u] && m - pes1[u][LRU].lru.front().first > bestHit[u]) return - 1.;

					const double EMU = MU[u] * double(M) / double(m+1);
					const double Rb =
						(E == 0 || E == 2 || E == 4) ?
						  RDb[u] <= 0. ? 0. : double(MU[u] - SLAb[u] - subRb[u].first) / (RDb[u] - subRb[u].second)
						: RDc[u] <= 0. ? 0. : double(MU[u] - SLAb[u]) / (E == 5 ? MU[u]-1 : RDc[u]);
					double ESLAb;
					if(SLAb[u] >= Qm[u][1]) ESLAb = SLAb[u] + max(0., 1. - Rb) * (EMU - MU[u]);
					else if(SLAb[u]*Rb >= Qm[u][1]) ESLAb = SLAb[u];
					else {
						const double S = Rb > 1. ? double(Qm[u][1]) / Rb : Qm[u][1];
						const double MQ = MU[u] + S - SLAb[u] + Rb / double(Qm[u][1]) / 2. *
											( S*(S-1) - double(SLAb[u])*double(SLAb[u]-1) );
						if(MQ <= EMU) ESLAb = S + max(0., 1. - Rb) * (EMU - MQ);
						else {
							const double a = Rb / double(Qm[u][1]) / 2.;
							if(a <= 0.) {
								ESLAb = SLAb[u] + EMU - MU[u];
							} else {
								const double b = 1. - a;
								const double c = MU[u] - SLAb[u] - EMU - double(SLAb[u])*double(SLAb[u]-1) * a;
								const double d = b*b - 4.*a*c;
								ESLAb = (sqrt(d) - b) / (2*a);
							}
						}
					}
					const double Ra = 
						(E == 0 || E == 2 || E == 4) ?
						  RDa[u] <= 0. ? 0. : double(MU[u] - SLAa[u] - subRa[u].first) / (RDa[u] - subRa[u].second)
						: Rb;
					const double ESLAa0 = SLAa[u] + max(0., 1. - Ra*double(Qt[u])/double(Qm[u][1])) * (EMU - MU[u]);
					if(E == 4) {
						const double prob = pow(double(pes1[u][LFU].Q3.top()[0]), 1.3);
						const double SLAdiff = ESLAa0 - ESLAb;
						const double LIM = .001;
						if(SLAdiff < LIM) {
							return L[u] * LIM / (ESLAb*ESLAb) * prob / (1. + LIM - SLAdiff);
						}
						return L[u] * SLAdiff / (ESLAb*ESLAb) * prob;
					}
					const double ESLAa1 = SLAa[u] + max(0., 1. - Ra*double(Qt[u]-1)/double(Qm[u][1])) * (EMU - MU[u]);
					const double diff = Ra*(EMU - MU[u])/double(Qm[u][1]);
					if((ESLAa1 <= ESLAb || ESLAa0 == ESLAa1) && diff > 0.)
						return .4 * L[u] * (pow((diff + max(ESLAa0-ESLAb, 0.)) / ESLAb, 2.) - pow(max(ESLAa0-ESLAb, 0.) / ESLAb, 2.)) /
								(1. + max(0., Qt[u] - double(Qm[u][1])/Ra) + max(0., ESLAb - ESLAa0) / diff);
					const double SLAr0 = (max(ESLAa0 - ESLAb, 0.)) / ESLAb;
					const double SLAr1 = (max(ESLAa1 - ESLAb, 0.)) / ESLAb;
					return L[u] * (SLAr1*SLAr1 - SLAr0*SLAr0);
				};
				int lu = -1;
				double bscore = 1e9;
				for(int u2 = 0; u2 < N; ++u2) if(Qt[u2] > Qm[u2][0]) {
					const double sc = eval(u2);
					if(lu == -1 || sc < bscore) {
						bscore = sc;
						lu = u2;
					}
				}
				-- Qt[lu];
				const int op = pes1[lu][bestK(m, lu, E)].worst(in[lu]);
				otherEvict(u, p, lu, op);
				ans = {lu, op};
			}
		} else {
			if(SLAa[u] < Qm[u][1]) subsRa[u].back().first = 1.;
			if(E >= 2) bestHit[u] = max(bestHit[u], m - last[u][p]);
			autoEvict2(u, p);
			ans = {u, p};
		}

		return ans;
	}

	void g(int m, int u, int p) {
		for(int k = 0; k < MK; ++k) if(active[u][k]) {
			pes0[u][k].add(p, in0[u][k]);
			in0[u][k][p] = true;
			pes0[u][k].clean(in0[u][k]);
			if(pes0[u][k].q < Qt[u]) pes0[u][k].addlast(in0[u][k]);
			pes1[u][k].add(p, in[u]);
		}
		in[u][p] = true;
		for(int k = 0; k < MK; ++k) if(active[u][k]) pes1[u][k].clean(in[u]);
	}

	double cost() const {
		double C = 0.;
		for(int u = 0; u < N; ++u) if(SLAb[u]) {
			const double rate = double(max(SLAa[u]-SLAb[u], 0)) / double(SLAb[u]);
			C += L[u] * rate*rate;
		}
		return C;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cin >> N >> Q >> M;
	for(int u = 0; u < N; ++u) cin >> L[u];
	for(int u = 0; u < N; ++u) cin >> D[u];
	for(int u = 0; u < N; ++u) for(int &q : Qm[u]) cin >> q;

	vector<deque<pair<int, int>>> lrub(N);
	array<vi, MAXN> loc, lastb;
	for(int u = 0; u < N; ++u) {
		MU[u] = SLAb[u] = bestHit[u] = 0;
		lastU[u] = -1;
		loc[u].assign(D[u], 0);
		lastb[u].assign(D[u], -1);
		counter[u].assign(D[u], 0);
		first[u].assign(D[u], -1);
		last[u].assign(D[u], -1);
	}

	array<UserEvict, ME> ues0;
	UserEvict ue1;
	array<double, ME> costs;
	array<bool, ME> active;
	for(int e = 0; e < ME; ++e) {
		active[e] = true;
		ues0[e].init();
	}
	ue1.init();

	bool lastUserStrategy = false;
	bool repeatStrategy = true;
	array<vector<int>, MAXN> hist;
	array<int, MAXN> part_ind, firstU;
	array<priority_queue<pair<int, int>>, MAXN> repeatQ;
	vector<int> nxt(M, M);
	for(int u = 0; u < N; ++u) {
		part_ind[u] = -1;
		firstU[u] = -1;
	}

	for(int m = 0; m < M; ++m) {
		int u, p;
		cin >> u >> p;
		-- u; -- p;

		++ MU[u];
		if(lastU[u] != m-1) {
			smallest_wait = min(smallest_wait, m - lastU[u]);
			part_ind[u] = 0;
		} else {
			++ part_ind[u];
		}
		largest_wait = max(largest_wait, m - lastU[u]);
		lastU[u] = m;
		if(firstU[u] == -1) firstU[u] = m;
		if(first[u][p] == -1) first[u][p] = m;

		for(int e = 0; e < ME; ++e) if(active[e]) ues0[e].newReq(u);
		ue1.newReq(u);

		// base simulation
		if(lastb[u][p] == -1) {
			if(++ SLAb[u] > Qm[u][1]) {
				while(true) {
					const auto [op, omp] = lrub[u].front(); lrub[u].pop_front();
					int &tmp = lastb[u][op];
					if(tmp != omp) continue;
					tmp = -1;
					break;
				}
			}
		} else {
			for(int e = 0; e < ME; ++e) if(active[e]) ues0[e].newHitB(u);
			ue1.newHitB(u);
			bestHit[u] = max(bestHit[u], m - lastb[u][p]);
		}
		lastb[u][p] = m;
		lrub[u].emplace_back(p, m);

		for(int e = 0; e < ME; ++e) if(active[e]) ues0[e].f(m, u, p, e);
		int be = 0;
		if(M > 100 && (lastUserStrategy || (smallest_wait >= M/8))) {
			if(last[u][p] != -1) nxt[last[u][p]] = m;
			if(firstU[u]+part_ind[u] == m || !repeatStrategy) {
				hist[u].push_back(p);
				be = ME;
			} else if(part_ind[u] >= (int) hist[u].size() || p != hist[u][part_ind[u]]) {
				repeatStrategy = false;
				be = ME;
			} else {
				if(part_ind[u] == 0) {
					repeatQ[u] = {};
					for(int u = 0; u < N; ++u) if(repeatQ[u].empty())
						for(int p = 0; p < D[u]; ++p)
							if(loc[u][p])
								repeatQ[u].emplace(first[u][p], p);
				}
				be = ME+1;
			}
			if(m == M/4) {
				lastUserStrategy = true;
				for(int e = 0; e < ME; ++e) active[e] = false;
			}
		} else {
			while(!active[be]) ++be;
			costs[be] = ues0[be].cost();
			for(int e = be+1; e < ME; ++e) if(active[e]) {
				costs[e] = ues0[e].cost();
				if(costs[e] < costs[be]) be = e;
			}
			if(m > M/3) {
				for(int e = 0; e < ME; ++e) if(active[e])
					if(costs[e] > 1.33*costs[be]+.01)
						active[e] = false;
			}
		}
		int u2, p2;
		if(be <= ME) {
			tie(u2, p2) = ue1.f(m, u, p, be);
		} else { // repeat mode
			ue1.updateSLAa0(u, p);
			if(!ue1.in[u][p]) {
				++ ue1.SLAa[u];
				if(ue1.Qt[u] >= Qm[u][2]) {
					while(!ue1.in[u][repeatQ[u].top().second]) repeatQ[u].pop();
					u2 = u;
					p2 = repeatQ[u].top().second;
					ue1.autoEvict(u, p, p2);
				} else if(ue1.nc < Q) {
					++ ue1.Qt[u];
					u2 = ++ ue1.nc;
					p2 = -1;
				} else {
					++ ue1.Qt[u];
					u2 = u;
					for(int uu = 0; uu < N; ++uu) if(uu != u && ue1.Qt[uu] > Qm[uu][0]) {
						u2 = uu;
						break;
					}
					-- ue1.Qt[u2];
					while(!ue1.in[u2][repeatQ[u2].top().second]) repeatQ[u2].pop();
					p2 = repeatQ[u2].top().second;
					ue1.otherEvict(u, p, u2, p2);
				}
			} else {
				u2 = u;
				p2 = p;
				ue1.autoEvict2(u, p);
			}
			repeatQ[u].emplace(nxt[firstU[u] + part_ind[u]], p);
		}
		if(p2 == -1) {
			loc[u][p] = u2;
		} else {
			swap(loc[u][p], loc[u2][p2]);
		}
		last[u][p] = m;
		++ counter[u][p];
		for(int e = 0; e < ME; ++e) if(active[e]) ues0[e].g(m, u, p);
		ue1.g(m, u, p);
		
		cout << loc[u][p] << endl;
	}

	return 0;
}
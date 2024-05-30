#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#pragma GCC optimize("O3")

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>

#ifdef ONLINE_JUDGE
#include <windows.h>
#endif

using namespace std;
using vi = vector<int>;
using pii = pair<int, int>;

double get_time() {
#ifndef ONLINE_JUDGE
    uint32_t lo, hi;
    asm volatile ( "rdtsc\n" : "=a" (lo), "=d" (hi) );
    return (double)((uint64_t)hi << 32 | lo) / 3.6e9;
#else
	HANDLE hProcess;
	hProcess = GetCurrentProcess();
	FILETIME CreationTime, ExitTime, KernelTime, UserTime;
	GetProcessTimes(hProcess, &CreationTime, &ExitTime, &KernelTime, &UserTime);
	uint64_t kernel = ((uint64_t)KernelTime.dwHighDateTime << 32) | KernelTime.dwLowDateTime;
	uint64_t user = ((uint64_t) UserTime.dwHighDateTime << 32) | UserTime.dwLowDateTime;
	return (kernel + user) / 1e7;
#endif
}

struct RNG {
	uint32_t x=123456789, y=362436069, z=521288629;
    RNG() { }
	uint32_t rand() { uint32_t t; x ^= x << 16; x ^= x >> 5; x ^= x << 1; t = x; x = y; y = z; z = t ^ x ^ y; return z;}
};
static RNG rng;

const int BLOCK = 16;
const double MAX_FP16 = 1.3107199999999999e+05;
const double MIN_FP16 = 3.0517578125000000e-05;

int N;
vector<double> a;
vector<vector<double>> b;
vector<vector<tuple<int, int, int, char>>> c;
vector<pii> tree;
string op;
 
int getExponant(double x) {
	return int((*reinterpret_cast<uint64_t*>(&x) >> 52) & 2047ull);
}
 
pair<int, int> getPriority(double x) {
	return make_pair(getExponant(x), countr_zero(*reinterpret_cast<uint64_t*>(&x)));
}
 
struct Cmp {
	Cmp() = default;
	bool operator()(const int i, const int j) const {
		return getPriority(a[i]) > getPriority(a[j]);
	}
};

struct Cmp0 {
	Cmp0() = default;
	bool operator()(const int i, const int j) const {
		return abs(a[i]) > abs(a[j]);
	}
};

double toFP16(double x) {
	if(abs(x) > MAX_FP16)
		return signbit(x) ? -numeric_limits<double>::infinity() : numeric_limits<double>::infinity();
	if(abs(x) < MIN_FP16)
		return 0.;
	*reinterpret_cast<uint64_t*>(&x) &= ~((1ull<<42)-1ull);
	return x;
}

double low_eq_sum(double a, double b) {
	const uint64_t u = *reinterpret_cast<const uint64_t*>(&a);
	const uint64_t v = *reinterpret_cast<const uint64_t*>(&b);
	if(u == v) {
		if(u&1) {
			uint64_t w = u | (1ull<<63);
			if(w >= ((1ull<<63) | (54ull<<52))) w = (w - (54ull<<52)) | ((1ull<<52)-1ull);
			else if(w >= ((1ull<<63) | (2ull<<52))) w = (1ull<<63) | ((1ull<<(((w>>52)&0x7ff)-2))-1);
			else w = 1ull<<63;
			return *reinterpret_cast<const double*>(&w);
		} else {
			uint64_t w = (1ull<<63) | (u & (0xfffull<<52));
			if(w > ((1ull<<63) | (53ull<<52))) w -= 53ull<<52;
			else if(w >= ((1ull<<63) | (2ull<<52))) w = (1ull<<63) | (1ull<<(((w>>52)&0x7ff)-2));
			return *reinterpret_cast<double*>(&w);
		}
	}
	const double c = a-b;
	uint64_t w = *reinterpret_cast<const uint64_t*>(&c);
	const int ew = int(w>>52)&0x7ff;
	int de = (int(u>>52)&0x7ff) - ew - 1;
	if(de >= 0) {
		if(!ew && !(de--)) return *reinterpret_cast<double*>(&w);
		if(w&(1ull<<63)) w += (1ull<<de);
		else w -= (1ull<<de);
	}
	if(w&(1ull<<63)) {
		if(!(a <= b+(*reinterpret_cast<double*>(&w)))) --w;
		else {
			++w;
			if(!(a <= b+(*reinterpret_cast<double*>(&w)))) --w;
		}
	} else {
		if(!(a <= b+(*reinterpret_cast<double*>(&w)))) ++w;
		else {
			--w;
			if(!(a <= b+(*reinterpret_cast<double*>(&w)))) ++w;
		}
	}
	return *reinterpret_cast<double*>(&w);
}

double low_eq_sum_float(double a, double b) {
	double x = low_eq_sum(a, b);
	uint64_t &w = *reinterpret_cast<uint64_t*>(&x);
	const int ew = int(w>>52)&0x7ff;
	const uint64_t lb = ew <= 0b01110000000 ? (1ull<<(30+0b01110000000-ew)) : (1ull<<29);
	if(w&(1ull<<63)) {
		if(w&lb) {
			w |= lb-1ull;
			w &= ~(lb>>1);
		} else {
			w &= ~(lb-1ull);
			w |= lb>>1;
		}
		w = min(w, (uint64_t)0xc7efffffefffffffull);
	} else {
		if(w&(lb-1ull)) w &= ~(lb-1ull);
		else w -= lb;
		w |= lb>>1;
		if(!(w&lb)) w |= 1;
		w = min(w, (uint64_t)0x47effffff0000000ull);
	}
	return x;
}

// smallest value y such that x <= toFP16(y)
double rdh(double x) {
	uint64_t &u = *reinterpret_cast<uint64_t*>(&x);
	if(u&(1ull<<63)) {
		if(u&((1ull<<42)-1ull)) u -= 1ull<42;
		u |= (1ull<<42)-1ull;
		u = clamp(u, (uint64_t)0xbeffffffffffffffull, (uint64_t)0xc0ffffffffffffffull);
	} else {
		if(u&((1ull<<42)-1ull)) {
			u += (1ull<<42);
			u &= ~((1ull<<42)-1ull);
		} else if(!u) return -3.0517578124999997e-05;
		u = clamp(u, (uint64_t)0x3f00000000000000ull, (uint64_t)0x4100000000000000ull);
	}
	return x;
}

double low_eq_sum_fp16(double a, double b) {
	return rdh(low_eq_sum(rdh(a), b));
}

int W;

double computeOp(int i, const double mi, const double ma) {
	auto [j, k] = tree[i];
	if(k == -1) return a[i];
	if(rng.rand()&1) swap(j, k);
	if(b[i].empty()) b[i].push_back(a[i]);
	double dist = numeric_limits<double>::infinity();
	double nearest = a[i];
	for(double x : b[i]) {
		double d = max(x-ma, mi-x);
		if(d < dist) {
			dist = d;
			nearest = x;
		}
	}
	double mi2 = min(mi, nearest);
	double ma2 = max(ma, nearest);
	if(mi2 < mi) ma2 += (mi-mi2)*0.99999;
	else if(ma2 > ma) mi2 -= (ma2-ma)*0.99999;
	const double mi2_16 = rdh(mi2);
	const double ma2_16 = -rdh(-ma2);
	const auto isok = [&](const double x)->bool { return mi2 <= x && x <= ma2; };
	const auto isok16 = [&](const double x)->bool { return mi2_16 <= x && x <= ma2_16; };
	const auto updtB = [&](const double s)->double {
		for(double x : b[i]) if(x == s) return s;
		b[i].push_back(s);
		return s;
	};
	const auto useSorD = [&](const double u, const double u32, const double v)->double {
		const double s = u+v;
		mi2 = min(s, mi);
		ma2 = max(s, ma);
		if(mi2 < mi) ma2 += (mi-mi2)*0.9999;
		else if(ma2 > ma) mi2 -= (ma2-ma)*0.9999;
		const double s32 = u32 + double(float(v));
		if(isok(s32)) {
			op[i] = 's';
			W += 2;
			return updtB(s32);
		}
		op[i] = 'd';
		W += 4;
		return updtB(s);
	};
	const auto useSorD2 = [&](const double u)->double {
		const double u32 = float(u);
		for(double y : b[k]) if(isok(u32 + double(float(y)))) {
			op[i] = 's';
			W += 2;
			const double v32 = float(computeOp(k, low_eq_sum_float(mi, u32), -low_eq_sum_float(-ma, -u32)));
			return updtB(u32+v32);
		}
		const double v = computeOp(k, low_eq_sum(mi, u), -low_eq_sum(-ma, -u));
		return useSorD(u, u32, v);
	};
	const auto tryH = [&](const double u, const double u16)->double {
		const double v = computeOp(k, low_eq_sum_fp16(mi, u16), -low_eq_sum_fp16(-ma, -u16));
		const double s16 = toFP16(u16+toFP16(v));
		if(isinf(s16)) return useSorD(u, double(float(u)), v);
		op[i] = 'h';
		W += 1;
		return updtB(s16);
	};
	if(b[j].empty()) b[j].push_back(a[j]);
	if(b[k].empty()) b[k].push_back(a[k]);
	for(int l = (int) b[k].size() - 1; l >= 0; --l) {
		if(l) {
			const int l2 = rng.rand()%(l+1);
			if(l != l2) swap(b[k][l], b[k][l2]);
		}
		const double y16 = toFP16(b[k][l]);
		for(double x : b[j]) if(isok16(toFP16(x) + y16)) {
			const double u = computeOp(j, low_eq_sum_fp16(mi, y16), -low_eq_sum_fp16(-ma, -y16));
			if(abs(u) < MAX_FP16) return tryH(u, toFP16(u));
			return useSorD2(u);
		}
	}
	for(double y : b[k]) {
		const double y32 = float(y);
		for(double x : b[j]) if(isok(double(float(x)) + y32)) {
			op[i] = 's';
			W += 2;
			const double u32 = float(computeOp(j, low_eq_sum_float(mi, y32), -low_eq_sum_float(-ma, -y32)));
			const double v32 = float(computeOp(k, low_eq_sum_float(mi, u32), -low_eq_sum_float(-ma, -u32)));
			return updtB(u32+v32);
		}
	}
	const double u = computeOp(j, low_eq_sum(mi, a[k]), -low_eq_sum(-ma, -a[k]));
	const double u16 = toFP16(u);
	if(!isinf(u16))
		for(double y : b[k]) if(isok16(u16 + toFP16(y)))
			return tryH(u, u16);
	return useSorD2(u);
}

void show(int i) {
	if(i < N) {
		printf("%d", i+1);
	} else {
		if(op[i] == 'j') {
			printf("{d:{s:");
			show(tree[i].first);
			printf("},");
			show(tree[i].second);
			printf("}");
		} else if(op[i] == 'u') {
			printf("{d:{h:");
			show(tree[i].first);
			printf("},");
			show(tree[i].second);
			printf("}");
		} else if(op[i] == 'v') {
			printf("{d:{h:");
			show(tree[i].first);
			printf("},{h:");
			show(tree[i].second);
			printf("}}");
		} else {
			printf("{%c:", op[i]);
			show(tree[i].first);
			printf(",");
			show(tree[i].second);
			printf("}");
		}
	}
}

array<uint64_t, 33> SUM_P, SUM_N;

void add_sum(array<uint64_t, 33> &SUM, __int128_t u, int b) {
	while(u) {
		u += SUM[b];
		SUM[b] = uint64_t(u);
		u >>= 64;
		b += 1;
	}
}

template<typename It>
double sum(It a, It b) {
	SUM_P.fill(0u);
	SUM_N.fill(0u);
	while(a != b) {
		const uint64_t y = *reinterpret_cast<uint64_t*>(&(*a));
		++a;
		int e = (y>>52)&((1<<11)-1);
		__int128_t u = y & ((1LL<<52)-1);
		if(e) {
			--e;
			u += 1LL<<52;
		}
		const int b = e/64;
		const int o = e%64;
		u <<= o;
		if(y&(1ull<<63)) add_sum(SUM_N, u, b);
		else add_sum(SUM_P, u, b);
	}
	int i = 32;
	while(i && SUM_N[i] == SUM_P[i]) --i;
	if(SUM_N[i] == SUM_P[i]) return 0.;
	bool neg = false;
	if(SUM_N[i] > SUM_P[i]) {
		neg = true;
		swap(SUM_N, SUM_P);
	}
	bool prop = false;
	for(int j = 0; j <= i; ++j) {
		if(prop) {
			prop = false;
			if(!SUM_P[j]) prop = true;
			--SUM_P[j];
		}
		if(SUM_N[j] > SUM_P[j]) prop = true;
		SUM_P[j] -= SUM_N[j];
	}
	while(i && !SUM_P[i]) --i;
	uint64_t u = SUM_P[i];
	const int lz = countl_zero(u)-10;
	if(lz <= 0) {
		u >>= -lz;
	} else {
		u <<= lz;
		if(i) u += SUM_P[i-1] >> (64-lz);
	}
	i = 64*i + 1-lz;
	if(u&1) {
		++u;
		if(u >= (1LL<<54)) {
			++i;
			u >>= 1;
		}
	}
	u >>= 1;
	uint64_t y = u & ((1LL<<52)-1);
	if(i < 0) {
		y >>= -i;
	} else {
		y += ((uint64_t)i+1) << 52;
	}
	if(neg) y += ((uint64_t)1) << 63;
	return *reinterpret_cast<double*>(&y);
}

double kahan_sum(const vector<double>& vec) {
    long double trueSum=0, corr=0;
    vector<double> dvtmp=vec;
    sort(dvtmp.begin(),dvtmp.end(), [](const double x, const double y) {
        return fabs(x) < fabs(y);
    });
    for(auto i : dvtmp) {
        long double y = static_cast<long double>(i) - corr;
        long double t = trueSum + y;
        corr = (t - trueSum) - y;
        trueSum = t;
    }
    return (double)trueSum;
}

int bruteForce(const double GOAL) {
	vector<vector<tuple<double, int, int>>> dp(1<<N);
	for(int i = 0; i < N; ++i) dp[1<<i].emplace_back(a[i], i, -1);
	for(int m = 1; m < (1<<N); ++m) {
		vector<double> b;
		for(int i = 0; i < N; ++i) if((m>>i)&1) b.push_back(a[i]);
		const double G = sum(b.begin(), b.end());
		for(int q = 1; q < m; ++q) {
			if((q&m) != q) continue;
			const int p = m^q;
			if(p > q) continue;
			for(int k = 0; k < (int) dp[q].size(); ++k)
				for(int l = 0; l < (int) dp[p].size(); ++l)
					dp[m].emplace_back(get<0>(dp[q][k])+get<0>(dp[p][l]), q+(k<<N), p+(l<<N));
		}
		sort(dp[m].begin(), dp[m].end());
		dp[m].resize(unique(dp[m].begin(), dp[m].end(), [&](const tuple<double, int, int> &a, const tuple<double, int, int> &b) {
			return get<0>(a) == get<0>(b);
		}) - dp[m].begin());
		const int k = int(lower_bound(dp[m].begin(), dp[m].end(), G, [&](const tuple<double, int, int> &a, double x) {
			return get<0>(a) < x;
		}) - dp[m].begin());
		dp[m].erase(dp[m].begin() + min((int) dp[m].size(), k+50), dp[m].end());
		dp[m].erase(dp[m].begin(), dp[m].begin() + max(0, k-50));
	}
	const int m = (1<<N)-1;
	if(!dp[m].empty()) {
		int k = int(lower_bound(dp[m].begin(), dp[m].end(), GOAL, [&](const tuple<double, int, int> &a, double x) {
			return get<0>(a) < x;
		}) - dp[m].begin());
		if(k >= (int) dp[m].size()) k = int(dp[m].size())-1;
		vector<pair<int, int>> Q;
		Q.emplace_back(-1, m+(k<<N));
		int ans = (int) a.size();
		while(!Q.empty()) {
			auto [p, mk] = Q.back();
			Q.pop_back();
			const auto &[x, u, v] = dp[mk&((1<<N)-1)][mk>>N];
			if(v == -1) {
				if(p != -1) {
					if(p&1) tree[p>>1].second = u;
					else tree[p>>1].first = u;
				}
			} else {
				if(p != -1) {
					if(p&1) tree[p>>1].second = (int)a.size();	
					else tree[p>>1].first = (int)a.size();	
				}
				a.push_back(x);
				tree.emplace_back(-1, -1);
				Q.emplace_back((int(a.size()-1)<<1)|0, u);
				Q.emplace_back((int(a.size()-1)<<1)|1, v);
			}
		}
		return ans;
	}
	return -1;
}

vector<int> mem_h0s;
array<vi, 2048> mem;
vector<char> inQ;
int h0(const vector<int> &inds, const double GOAL) {
	bool CB = inds.size() == BLOCK && inds.back() == inds[0]+BLOCK-1;
	if(CB && mem_h0s[inds[0]] != -1) return mem_h0s[inds[0]];
	int ans = -1;
	double best = numeric_limits<double>::infinity();
	for(bool uu : {false, true}) {
		priority_queue<int, vector<int>, Cmp> Q;
		int cnt = -1;
		const auto fuse = [&](const int i, const int j) {
			a.push_back(a[i]+a[j]);
			tree.emplace_back(i, j);
			inQ.push_back(false);
			--cnt;
		};
		const auto create = [&](int i) {
			while(true) {
				const int e = getExponant(a[i]);
				if(uu) {
					vi &v0 = mem[e+1];
					bool foundup = false;
					for(int k = 0; k < (int)v0.size(); ++k) {
						if(!inQ[v0[k]]) {
							v0[k] = v0.back();
							v0.pop_back();
							--k;
							continue;
						}
						if(getExponant(a[i]+a[v0[k]]) <= e) {
							inQ[v0[k]] = false;
							fuse(i, v0[k]);
							v0[k] = v0.back();
							v0.pop_back();
							i = (int) tree.size()-1;
							foundup = true;
							break;
						}
					}
					if(foundup) continue;
				}
				vi &v = mem[e];
				while(!v.empty() && !inQ[v.back()]) v.pop_back();
				if(!v.empty() && (a[v.back()] < 0.) != (a[i] < 0.)) {
					inQ[v.back()] = false;
					fuse(i, v.back());
					v.pop_back();
					i = (int) tree.size()-1;
				} else {
					v.push_back(i);
					Q.push(i);
					inQ[i] = true;
					break;
				}
			}
		};
		for(int i : inds) {
			++cnt;
			create(i);
		}
		while(cnt) {
			while(!inQ[Q.top()]) Q.pop();
			const int i = Q.top(); Q.pop();
			while(!inQ[Q.top()]) Q.pop();
			const int j = Q.top(); Q.pop();
			inQ[i] = inQ[j] = false;
			fuse(i, j);
			create((int) tree.size()-1);
		}
		while(!inQ[Q.top()]) Q.pop();
		const int i = Q.top();
		inQ[i] = false;
		const double d = abs(a[i]-GOAL);
		if(d < best) {
			best = d;
			ans = i;
		}
	}
	if(CB) mem_h0s[inds[0]] = ans;
	return ans;
}

pii h2(const int i0) {
	pii ans;
	vector<int> inds;
	for(int i = 1; i+1 < BLOCK; ++i) {
		inds.push_back(i0+i);
		inds.push_back(i0+BLOCK+i);
	}
	sort(inds.begin(), inds.end(), [&](int i, int j) { return abs(a[i]) < abs(a[j]); });
	pii mid[2] = {{i0+BLOCK-1, i0}, {i0+BLOCK, i0+2*BLOCK-1}};
	if(abs(a[mid[0].first] + a[mid[0].second]) > abs(a[mid[1].first] + a[mid[1].second]))
		swap(mid[0], mid[1]);
	for(int J : {0, 1}) {
		int R = mid[J].first;
		priority_queue<int, vector<int>, Cmp> Q;
		int cnt = BLOCK-1;
		const auto fuse = [&](const int i, const int j) {
			a.push_back(a[i]+a[j]);
			if(i == R) {
				tree.emplace_back(i, j);
				R = int(a.size())-1;
			} else if(j == R) {
				tree.emplace_back(j, i);
				R = int(a.size())-1;
			} else {
				tree.emplace_back(i, j);
			}
			inQ.push_back(false);
			--cnt;
		};
		const auto create = [&](int i) {
			while(true) {
				const int e = getExponant(a[i]);
				vi &v = mem[e];
				while(!v.empty() && !inQ[v.back()]) v.pop_back();
				if(!v.empty() && (a[v.back()] < 0.) != (a[i] < 0.)) {
					inQ[v.back()] = false;
					fuse(i, v.back());
					v.pop_back();
					i = (int) tree.size()-1;
				} else {
					v.push_back(i);
					Q.push(i);
					inQ[i] = true;
					break;
				}
			}
		};
		for(int j = 0; j < BLOCK-2; ++j) create(inds[J*(BLOCK-2) + j]);
		create(mid[J].first);
		create(mid[J].second);
		while(cnt) {
			while(!inQ[Q.top()]) Q.pop();
			const int i = Q.top(); Q.pop();
			while(!inQ[Q.top()]) Q.pop();
			const int j = Q.top(); Q.pop();
			inQ[i] = inQ[j] = false;
			fuse(i, j);
			create((int) tree.size()-1);
		}
		while(!inQ[Q.top()]) Q.pop();
		inQ[Q.top()] = false;
		if(J) ans.second = Q.top();
		else ans.first = Q.top();
	}
	return ans;
}

unordered_map<int, double> pen;

double computePen(int i) {
	vector<int> st(1, i);
	int ind = 0, first = 0;
	double p = 0.;
	while(!st.empty()) {
		int i = st.back();
		st.pop_back();
		if(i < N) {
			if(ind == 0) first = i;
			else if(abs(i-first) >= BLOCK) ++ p;
			ind = (ind+1)&0xff;
		} else {
			st.push_back(tree[i].second);
			st.push_back(tree[i].first);
		}
	}
	return p*(p+1)/20'000.;
}

void suffleTree(int i) {
	vector<int> interns(1, i);
	for(int l = 0; l < (int) interns.size(); ++l) {
		const int i = interns[l];
		const auto &[j, k] = tree[i];
		if(tree[j].second != -1) interns.push_back(j);
		if(tree[k].second != -1) interns.push_back(k);
	}
	double p = computePen(i);
	const int mm = max(2, (int)interns.size()/2);
	for(int t = 0; t < 50; ++t) {
		vector<int> shifts;
		const int m = min(t/2+2, mm);
		for(int i : interns) if(rng.rand()%m == 0) {
			shifts.push_back(i);
			swap(tree[i].first, tree[i].second);
		}
		const double p2 = computePen(i);
		if(p2 < p) p = p2;
		else for(int i : shifts) swap(tree[i].first, tree[i].second);
	}
	pen[i] = p;
}

bool special_tree2 = false;
unordered_map<int, char> special_tree;

double score(const int i, const double GOAL) {
	W = 0;
	double x;
	if(special_tree[i] == 'A') {
		const auto &[rkl, ij] = tree[i];
		const auto &[r, kl] = tree[rkl];
		const double mi = rdh(low_eq_sum(low_eq_sum_float(GOAL, a[ij]), a[kl]));
		const double ma = -rdh(low_eq_sum(low_eq_sum_float(-GOAL, -a[ij]), -a[kl]));
		x = toFP16(computeOp(r, mi, ma)) + a[kl];
		op[i] = 'j';
		op[rkl] = 'u';
		op[ij] = 'd';
		W += 12;
		if(b[ij].empty()) b[ij].push_back(a[ij]);
		computeOp(kl, a[kl], a[kl]);
		if(ranges::find(b[rkl], x) == b[rkl].end()) b[rkl].push_back(x);
		x = double(float(x)) + a[ij];
		if(ranges::find(b[i], x) == b[i].end()) b[i].push_back(x);
	} else if(special_tree[i] == 'B' || special_tree[i] == 'C') {
		const auto &[j, k] = tree[i];
		if(special_tree[i] == 'C') {
			const auto &[r, kl] = tree[j];
			const auto &[u, v] = tree[r];
			const double mij = low_eq_sum_float(GOAL, a[k]);
			const double maj = -low_eq_sum_float(-GOAL, -a[k]);
			const double mir = low_eq_sum(mij, a[kl]);
			const double mar = -low_eq_sum(-maj, -a[kl]);
			const auto gotXY = [&](double x, double y) {
				op[i] = 'j';
				op[j] = 'd';
				op[k] = 'd';
				op[r] = 'v';
				W += 16;
				x = toFP16(x) + toFP16(y);
				if(ranges::find(b[r], x) == b[r].end()) b[r].push_back(x);
				x += computeOp(kl, a[kl], a[kl]);
				if(ranges::find(b[j], x) == b[j].end()) b[j].push_back(x);
				if(b[k].empty()) b[k].push_back(a[k]);
				x = double(float(x)) + a[k];
				if(ranges::find(b[i], x) == b[i].end()) b[i].push_back(x);
				const double C = (double(W) + pen[i]) / double(N-1);
				const double D = 10. / sqrt(C + 0.5);
				const double A = pow(max(abs(x-GOAL)/max(abs(GOAL), 1e-200), 1e-20), 0.05);
				const double Score = D/A;
				return Score;
			};
			for(int l = (int) b[u].size() - 1; l > 0; --l) {
				const int l2 = rng.rand()%(l+1);
				if(l != l2) swap(b[u][l], b[u][l2]);
			}
			for(int l = (int) b[v].size() - 1; l > 0; --l) {
				const int l2 = rng.rand()%(l+1);
				if(l != l2) swap(b[v][l], b[v][l2]);
			}
			for(double x : b[u]) for(double y : b[v]) {
				const double s = toFP16(x) + toFP16(y);
				if(mir <= s && s <= mar) {
					if(rng.rand()&1) {
						const double miu = rdh(low_eq_sum(mir, toFP16(y)));
						const double mau = -rdh(low_eq_sum(-mar, -toFP16(y)));
						x = computeOp(u, miu, mau);
						const double miv = rdh(low_eq_sum(mir, toFP16(x)));
						const double mav = -rdh(low_eq_sum(-mar, -toFP16(x)));
						y = computeOp(v, miv, mav);
						return gotXY(x, y);
					} else {
						const double miv = rdh(low_eq_sum(mir, toFP16(x)));
						const double mav = -rdh(low_eq_sum(-mar, -toFP16(x)));
						y = computeOp(v, miv, mav);
						const double miu = rdh(low_eq_sum(mir, toFP16(y)));
						const double mau = -rdh(low_eq_sum(-mar, -toFP16(y)));
						x = computeOp(u, miu, mau);
						return gotXY(x, y);
					}
				}
			}
			if(rng.rand()&1) {
				int l = 0;
				while(l < (int)b[v].size() && abs(b[v][l]) > MAX_FP16) ++l;
				if(l < (int)b[v].size()) {
					double y = b[v][l];
					const double miu = rdh(low_eq_sum(mir, toFP16(y)));
					const double mau = -rdh(low_eq_sum(-mar, -toFP16(y)));
					double x = computeOp(u, miu, mau);
					if(miu <= x && x <= mau) {
						const double miv = rdh(low_eq_sum(mir, toFP16(x)));
						const double mav = -rdh(low_eq_sum(-mar, -toFP16(x)));
						double y = computeOp(v, miv, mav);
						return gotXY(x, y);
					} else W = 0;
				}
			} else {
				int l = 0;
				while(l < (int)b[u].size() && abs(b[u][l]) > MAX_FP16) ++l;
				if(l < (int)b[u].size()) {
					double x = b[u][l];
					const double miv = rdh(low_eq_sum(mir, toFP16(x)));
					const double mav = -rdh(low_eq_sum(-mar, -toFP16(x)));
					double y = computeOp(v, miv, mav);
					if(miv <= y && y <= mav) {
						const double miu = rdh(low_eq_sum(mir, toFP16(y)));
						const double mau = -rdh(low_eq_sum(-mar, -toFP16(y)));
						double x = computeOp(u, miu, mau);
						return gotXY(x, y);
					} else W = 0;
				}
			}
		}
		x = double(float(computeOp(j, low_eq_sum_float(GOAL, a[k]), -low_eq_sum_float(-GOAL, -a[k])))) + a[k];
		op[i] = 'j';
		op[k] = 'd';
		W += 8;
		if(b[k].empty()) b[k].push_back(a[k]);
		if(ranges::find(b[i], x) == b[i].end()) b[i].push_back(x);
	} else if(special_tree2) {
		x = float(computeOp(i, low_eq_sum_float(GOAL, 0.), -low_eq_sum(-GOAL, 0.)));
	} else {
		x = computeOp(i, GOAL, GOAL);
	}
	if(isinf(x) || isnan(x)) return 0.;
	const double C = (double(W) + pen[i]) / double(N-1);
	const double D = 10. / sqrt(C + 0.5);
	const double A = pow(max(abs(x-GOAL)/max(abs(GOAL), 1e-200), 1e-20), 0.05);
	const double Score = D/A;
	return Score;
}

vi orderDP;
void prepareDP(int i) {
	if(i < N) return;
	prepareDP(tree[i].first);
	prepareDP(tree[i].second);
	orderDP.push_back(i);
}

void CtoOp(int i, int l) {
	if(i < N) return;
	const auto &[j, k] = tree[i];
	op[i] = get<3>(c[i][l]);
	CtoOp(j, get<1>(c[i][l]));
	CtoOp(k, get<2>(c[i][l]));
}

int gather(const vector<int> &inds, int R) {
	priority_queue<int, vector<int>, Cmp> Q;
	int cnt = -1;
	const auto fuse = [&](const int i, const int j) {
		a.push_back(a[i]+a[j]);
		if(i == R) {
			tree.emplace_back(j, i);
			R = int(a.size())-1;
		} else if(j == R) {
			tree.emplace_back(i, j);
			R = int(a.size())-1;
		} else {
			tree.emplace_back(i, j);
		}
		inQ.push_back(false);
		--cnt;
	};
	const auto create = [&](int i) {
		while(true) {
			const int e = getExponant(a[i]);
			vi &v = mem[e];
			while(!v.empty() && !inQ[v.back()]) v.pop_back();
			if(!v.empty() && (a[v.back()] < 0.) != (a[i] < 0.)) {
				inQ[v.back()] = false;
				fuse(i, v.back());
				v.pop_back();
				i = (int) tree.size()-1;
			} else {
				v.push_back(i);
				Q.push(i);
				inQ[i] = true;
				break;
			}
		}
	};
	for(int i : inds) {
		++cnt;
		create(i);
	}
	while(cnt) {
		while(!inQ[Q.top()]) Q.pop();
		const int i = Q.top(); Q.pop();
		while(!inQ[Q.top()]) Q.pop();
		const int j = Q.top(); Q.pop();
		inQ[i] = inQ[j] = false;
		fuse(i, j);
		create((int) tree.size()-1);
	}
	while(!inQ[Q.top()]) Q.pop();
	inQ[Q.top()] = false;
	return Q.top();
}

int gather2(const vector<int> &inds, int R) {
	const auto CmpPM = [&](int i, int j) {
		return make_tuple(getPriority(a[i]), abs(a[i])) < make_tuple(getPriority(a[j]), abs(a[j]));
	};
	priority_queue<int, vector<int>, decltype(CmpPM)> Qp(CmpPM), Qm(CmpPM);
	for(int i : inds) {
		if(a[i] < 0.) Qm.push(i);
		else Qp.push(i);
	}
	const auto fuse = [&](int i, int j) {
		a.push_back(a[i]+a[j]);
		if(i == R) {
			tree.emplace_back(j, i);
			R = int(a.size())-1;
		} else if(j == R) {
			tree.emplace_back(i, j);
			R = int(a.size())-1;
		} else {
			tree.emplace_back(i, j);
		}
		inQ.push_back(false);
	};
	while(!Qp.empty() && !Qm.empty()) {
		const int i = Qp.top(); Qp.pop();
		const int j = Qm.top(); Qm.pop();
		fuse(i, j);
		if(a.back() < 0.) Qm.push(int(a.size())-1);
		else Qp.push(int(a.size())-1);
	}
	while(Qp.size() > 1) {
		const int i = Qp.top(); Qp.pop();
		const int j = Qp.top(); Qp.pop();
		fuse(i, j);
		Qp.push(int(a.size())-1);
	}
	while(Qm.size() > 1) {
		const int i = Qm.top(); Qm.pop();
		const int j = Qm.top(); Qm.pop();
		fuse(i, j);
		Qm.push(int(a.size())-1);
	}
	return Qp.empty() ? Qm.top() : Qp.top();
}

int main() {
	scanf("%d", &N);
	const double start_time = get_time();
	a.resize(N);
	for(double &x : a) scanf("%lf", &x);
	tree.resize(N);
	for(int i = 0; i < N; ++i) tree[i] = {i, -1};
	inQ.assign(N, false);

	const double GOAL = kahan_sum(a);
	vector<int> roots;

	string old_op;
	double sc = -1.;
	int ind = 0;
	const auto try_improve = [&](int i) {
		swap(old_op, op);
		op.resize(a.size());
		b.resize(a.size());
		const double sc2 = score(i, GOAL);
		if(sc2 <= sc) {
			swap(old_op, op);
		} else {
			ind = i;
			sc = sc2;
			// fprintf(stderr, "Score: %.4f\n", sc);
		}
		return sc2;
	};
	
	mem_h0s.resize(N, -1);
	if(N > 10 && GOAL != 0.) {
		// auto t0 = get_time();
		int mE = 2048;
		for(double x : a) mE = min(mE, getExponant(x) + countr_zero(*reinterpret_cast<uint64_t*>(&x)));
		int E = getExponant(GOAL);
		const int Me = E + countr_zero(*reinterpret_cast<const uint64_t*>(&GOAL));
		uint64_t g = ((*reinterpret_cast<const uint64_t*>(&GOAL)) & ((1ull<<52)-1ull)) | (1ull<<52);
		uint64_t diff = (1ull<<34) - 1ull;
		if(E >= mE) {
			g <<= 35;
			E -= 35;
		} else {
			const int tsh = 35+(E-mE);
			const int sh = 35+int((E-mE)*.9);
			diff = (1ull<<(34+sh-tsh)) - 1ull;
			g <<= sh;
			E -= sh;
		}
		if(g != 0) {
			const uint64_t g0 = g - diff;
			const uint64_t g1 = g + diff;
			const bool ZZ = g0 > g || g1 < g;
			vector<pair<uint64_t, int>> M;
			for(int i = 0; i < N; ++i) {
				const int e = getExponant(a[i]);
				if(e > Me) continue;
				uint64_t m = (*reinterpret_cast<uint64_t*>(&a[i]) & ((1ull<<52)-1ull)) | (1ull<<52);
				const int d = e-E;
				if(d > 0) m <<= d;
				else m >>= -d;
				if((*reinterpret_cast<uint64_t*>(&a[i])^*reinterpret_cast<const uint64_t*>(&GOAL))&(1ull<<63)) m = -m;
				M.emplace_back(m, i);
			}
			ranges::sort(M);
			for(auto [x, i] : M) {
				auto it = ranges::lower_bound(M, g0-x, less<uint64_t>{}, [&](const pair<uint64_t, int> &x) { return x.first; });
				if(it == M.end()) it = M.begin();
				if(it->second == i) {
					++ it;
					if(it == M.end()) it = M.begin();
				}
				const uint64_t s = x + it->first;
				if(ZZ ? (s <= g1 || g0 <= s) : (g0 <= s && s <= g1)) {
					const int j = it->second;
					const int sss = (int) a.size();
					tree.emplace_back(i, j);
					a.push_back(a[i] + a[j]);
					inQ.push_back(false);
					const double G0 = low_eq_sum_float(GOAL, a[sss]);
					const double G1 = -low_eq_sum_float(-GOAL, -a[sss]);
					if(G0 > G1) {
						tree.pop_back();
						a.pop_back();
						inQ.pop_back();
						continue;
					}
					const double G2f = float(midpoint(G0, G1));
					int r = -1;
					char SPT = 0;
					vector<int> inds, groups;
					vector<double> vals;
					int R = -1;
					int i0 = 0;

					if(0.5*abs(G2f) < MAX_FP16) {
						const int E2 = getExponant(G2f);
						const uint64_t g2 = *reinterpret_cast<const uint64_t*>(&G2f) << 22;
						if(g2 != 0) {
							const uint64_t g0 = g2 - (1ull<<50) + 1ull;
							const uint64_t g1 = g2 + (1ull<<50) - 1ull;
							const bool ZZ = g0 > g2 || g1 < g2;
							vector<pair<uint64_t, int>> M2;
							for(int k = 0; k < N; ++k) if(k != i && k != j) {
								const int e = getExponant(a[k]);
								if(e > E2) continue;
								uint64_t m = (*reinterpret_cast<uint64_t*>(&a[k]) & ((1ull<<52)-1ull)) | (1ull<<52);
								const int d = e-E2+22;
								if(d > 0) m <<= d;
								else m >>= -d;
								if((*reinterpret_cast<uint64_t*>(&a[k])^*reinterpret_cast<const uint64_t*>(&G2f))&(1ull<<63)) m = -m;
								M2.emplace_back(m, k);
							}
							ranges::sort(M2);
								{
								auto it = ranges::upper_bound(M2, g1, less<uint64_t>{}, [&](const pair<uint64_t, int> &x) { return x.first; });
								if(it == M2.begin()) it = M2.end();
								--it;
								uint64_t s = it->first;
								if(ZZ ? (s <= g1 || g0 <= s) : (g0 <= s && s <= g1)) {
									const int k = it->second;
									const double G20 = rdh(low_eq_sum(G0, a[k]));
									const double G21 = -rdh(low_eq_sum(-G1, -a[k]));
									if(G20 <= G21 || abs(G2f) > MAX_FP16) {
										while(i0 < N) {
											vals.clear();
											inds.clear();
											while((int) inds.size() < BLOCK && i0 < N) {
												if(i0 == i || i0 == j || i0 == k) {
													++i0;
													continue;
												}
												vals.push_back(a[i0]);
												inds.push_back(i0++);
											}
											if(!inds.empty()) {
												const double g = sum(vals.begin(), vals.end());
												groups.push_back(h0(inds, g));
												if((int) inds.size() < BLOCK) R = groups.back();
											}
										}
										const int r0 = gather(groups, R);
										r = (int) a.size();
										tree.emplace_back(r0, k);
										a.push_back(a[r0] + a[k]);
										inQ.push_back(false);
										SPT = G20 <= G21 ? 'A' : 'C';
										goto FINISH_TREE;
									}
								}
								}
							for(auto [x, k] : M2) {
								auto it = ranges::upper_bound(M2, g1-x, less<uint64_t>{}, [&](const pair<uint64_t, int> &x) { return x.first; });
								if(it == M2.begin()) it = M2.end();
								--it;
								if(it->second == k) {
									if(it == M2.begin()) it = M2.end();
									-- it;
								}
								const uint64_t s = x + it->first;
								if(ZZ ? (s <= g1 || g0 <= s) : (g0 <= s && s <= g1)) {
									const int l = it->second;
									const int kl = (int) a.size();
									tree.emplace_back(k, l);
									a.push_back(a[k] + a[l]);
									inQ.push_back(false);
									const double G20 = rdh(low_eq_sum(G0, a[kl]));
									const double G21 = -rdh(low_eq_sum(-G1, -a[kl]));
									if(G20 > G21 && abs(G2f) < MAX_FP16) {
										tree.pop_back();
										a.pop_back();
										inQ.pop_back();
										continue;
									}

									while(i0 < N) {
										vals.clear();
										inds.clear();
										while((int) inds.size() < BLOCK && i0 < N) {
											if(i0 == i || i0 == j || i0 == k || i0 == l) {
												++i0;
												continue;
											}
											vals.push_back(a[i0]);
											inds.push_back(i0++);
										}
										if(!inds.empty()) {
											const double g = sum(vals.begin(), vals.end());
											groups.push_back(h0(inds, g));
											if((int) inds.size() < BLOCK)
												R = groups.back();
										}
									}
									const int r0 = gather(groups, R);
									r = (int) a.size();
									tree.emplace_back(r0, kl);
									a.push_back(a[r0] + a[kl]);
									inQ.push_back(false);
									SPT = G20 <= G21 ? 'A' : 'C';
									goto FINISH_TREE;
								}
							}
							M2.clear();
							for(int k = 0; k < N; ++k) if((i < k || k+BLOCK <= i) && (j < k || k+BLOCK <= j)) {
								const int ki = mem_h0s[k];
								if(ki == -1) continue;
								const int e = getExponant(a[ki]);
								if(e > E2) continue;
								uint64_t m = (*reinterpret_cast<uint64_t*>(&a[ki]) & ((1ull<<52)-1ull)) | (1ull<<52);
								const int d = e-E2+22;
								if(d > 0) m <<= d;
								else m >>= -d;
								if((*reinterpret_cast<uint64_t*>(&a[ki])^*reinterpret_cast<const uint64_t*>(&G2f))&(1ull<<63)) m = -m;
								M2.emplace_back(m, k);
							}
							ranges::sort(M2);
							for(auto [x, k] : M2) {
								auto it = ranges::upper_bound(M2, g1-x, less<uint64_t>{}, [&](const pair<uint64_t, int> &x) { return x.first; });
								if(it == M2.begin()) it = M2.end();
								--it;
								while(it->second < k+BLOCK && k < it->second+BLOCK) {
									if(it == M2.begin()) it = M2.end();
									-- it;
								}
								const uint64_t s = x + it->first;
								if(ZZ ? (s <= g1 || g0 <= s) : (g0 <= s && s <= g1)) {
									const int l = it->second;
									const int ki = mem_h0s[k];
									const int li = mem_h0s[l];
									const int kl = (int) a.size();
									tree.emplace_back(ki, li);
									a.push_back(a[ki] + a[li]);
									inQ.push_back(false);
									const double G20 = rdh(low_eq_sum(G0, a[kl]));
									const double G21 = -rdh(low_eq_sum(-G1, -a[kl]));
									if(G20 > G21 && abs(G2f) < MAX_FP16) {
										tree.pop_back();
										a.pop_back();
										inQ.pop_back();
										continue;
									}

									while(i0 < N) {
										vals.clear();
										inds.clear();
										while((int) inds.size() < BLOCK && i0 < N) {
											if(i0 == i || i0 == j || (k <= i0 && i0 < k+BLOCK) || (l <= i0 && i0 < l+BLOCK)) {
												++i0;
												continue;
											}
											vals.push_back(a[i0]);
											inds.push_back(i0++);
										}
										if(!inds.empty()) {
											const double g = sum(vals.begin(), vals.end());
											groups.push_back(h0(inds, g));
											if((int) inds.size() < BLOCK)
												R = groups.back();
										}
									}
									const int r0 = gather(groups, R);
									r = (int) a.size();
									tree.emplace_back(r0, kl);
									a.push_back(a[r0] + a[kl]);
									inQ.push_back(false);
									SPT = G20 <= G21 ? 'A' : 'C';
									goto FINISH_TREE;
								}
							}
						}
					}

					// With just floats
					while(i0 < N) {
						vals.clear();
						inds.clear();
						while((int) inds.size() < BLOCK && i0 < N) {
							if(i0 == i || i0 == j) {
								++i0;
								continue;
							}
							vals.push_back(a[i0]);
							inds.push_back(i0++);
						}
						if(!inds.empty()) {
							const double g = sum(vals.begin(), vals.end());
							groups.push_back(h0(inds, g));
							if((int) inds.size() < BLOCK)
								R = groups.back();
						}
					}
					r = gather(groups, R);
					SPT = 'B';

					FINISH_TREE:
					const int r2 = (int) a.size();
					tree.emplace_back(r, sss);
					a.push_back(a[r] + a[sss]);
					inQ.push_back(false);
					roots.push_back(r2);
					pen[r2] = SPT == 'A' ? 10./20'000. : 3./20'000.;
					special_tree[r2] = SPT;
					if(roots.size() >= 8) break;
				}
			}
			// fprintf(stderr, "First strategy: %.3fs\n", get_time() - t0);
		} else {
			special_tree2 = true;
		}
	}

	if(N <= 14) {
		const int i = bruteForce(GOAL);
		if(i > 0) roots.push_back(i);
	}

	if(roots.empty()) {
		vector<int> inds0, inds1;
		int R0 = -1, R1 = -1;
		vector<int> h0s(N/BLOCK+1), h1s(N/BLOCK+1), indsh;
		vector<pii> h2s(N/BLOCK+1);
		// auto t0 = get_time();
		for(int i0 = 0; i0 < N; i0 += BLOCK) {
			const int j = i0/BLOCK;
			const int m = min(BLOCK, N-i0);
			const double g = sum(a.begin()+i0, a.begin()+i0+m);
			indsh.clear();
			for(int i = 0; i < m; ++i) indsh.push_back(i+i0);
			h1s[j] = h0(indsh, g);
			h0s[j] = int(a.size())-m;
			if(i0+2*BLOCK <= N) h2s[j] = h2(i0);
		}
		// fprintf(stderr, "GOAL and h0: %.3fs\n", get_time() - t0);
		// New way of compute g using sum of all previous a[i] - sum of all previous blocks 
		// t0 = get_time();
		for(int i0 = 0; i0 < N; i0 += BLOCK) {
			const int b = i0/BLOCK;
			if(i0+2*BLOCK <= N) {
				const double g = sum(a.begin()+i0, a.begin()+i0+2*BLOCK);
				const auto [j, k] = h2s[b];
				const int l = h0s[b];
				const int m = h0s[b+1];
				if(abs((a[j]+a[k])-g) < abs((a[l]+a[m])-g)) {
					inds0.push_back(j);
					inds0.push_back(k);
					i0 += BLOCK;
				} else {
					inds0.push_back(l);
				}
			} else {
				const int l = h0s[b];
				inds0.push_back(l);
				if(i0+BLOCK > N) R0 = l;
			}
		}
		for(int i0 = 0; i0 < N; i0 += BLOCK) {
			const int b = i0/BLOCK;
			if(i0+2*BLOCK <= N) {
				const double g = sum(a.begin()+i0, a.begin()+i0+2*BLOCK);
				const auto [j, k] = h2s[b];
				const int l = h1s[b];
				const int m = h1s[b+1];
				if(abs((a[j]+a[k])-g) < abs((a[l]+a[m])-g)) {
					inds1.push_back(j);
					inds1.push_back(k);
					i0 += BLOCK;
				} else {
					inds1.push_back(l);
				}
			} else {
				const int l = h1s[b];
				inds1.push_back(l);
				if(i0+BLOCK > N) R1 = l;
			}
		}
		// fprintf(stderr, "g, h2: %.3fs\n", get_time() - t0);

		// t0 = get_time();
		roots.push_back(gather(inds0, R0));
		roots.push_back(gather(inds1, R1));
		roots.push_back(gather2(inds0, R0));
		roots.push_back(gather2(inds1, R1));
		// fprintf(stderr, "Gather: %.3fs\n", get_time() - t0);

	// auto t0 = get_time();
		for(int i : roots) {
			try_improve(i);
			try_improve(i);
		}
	// fprintf(stderr, "Operations: %.3fs\n", get_time() - t0);
	} else {
	// auto t0 = get_time();
		vector<pair<double, int>> rs;
		for(int i : roots) rs.emplace_back(try_improve(i), i);
		sort(rs.rbegin(), rs.rend());
		rs.resize(min((int)rs.size(), 4));
		for(const auto &[s, i] : rs) try_improve(i);
	// fprintf(stderr, "Operations: %.3fs\n", get_time() - t0);
	}

	roots.clear();
	roots.push_back(ind);

	// const double end_time = start_time + (10. - (1. - pow(N-2.5e5, 2)*4e-13));
	const double end_time = start_time + 9.9 - pow(N-6e5, 2.)*3.3e-12 - .68e-6*N;
	double last_run = 0.;
	while(true) {
		const double t = get_time();
		if(t + 14*last_run > end_time) break;
		try_improve(ind);
		last_run = get_time()-t;
	}

	if(N < 10000) {
		vi rsup, rinf;
		int R = -1;
		for(int i0 = 0; i0 < N; i0 += BLOCK) {
			priority_queue<int, vector<int>, Cmp0> Q;
			for(int i = 0; i < BLOCK && i0+i < N; ++i) Q.emplace(i0+i);
			while(Q.size() > 1) {
				const int i = Q.top(); Q.pop();
				const int j = Q.top(); Q.pop();
				a.push_back(a[i]+a[j]);
				tree.emplace_back(i, j);
				Q.push((int) tree.size()-1);
			}
			if(abs(a[Q.top()]) > abs(GOAL)) rsup.push_back(Q.top());
			else rinf.push_back(Q.top());
			if(i0+BLOCK >= N) R = Q.top();
		}
	
		priority_queue<int, vector<int>, Cmp0> Q(rsup.begin(), rsup.end());
		while(Q.size() > 1) {
			const int i = Q.top(); Q.pop();
			const int j = Q.top(); Q.pop();
			a.push_back(a[i]+a[j]);
			if(i == R) {
				tree.emplace_back(j, i);
				R = int(a.size())-1;
			} else if(j == R) {
				tree.emplace_back(i, j);
				R = int(a.size())-1;
			} else {
				tree.emplace_back(i, j);
			}
			if(abs(a.back()) > abs(GOAL)) Q.push((int) tree.size()-1);
			else rinf.push_back((int) tree.size()-1);
		}
		for(int i : rinf) Q.push(i);
		while(Q.size() > 1) {
			const int i = Q.top(); Q.pop();
			const int j = Q.top(); Q.pop();
			a.push_back(a[i]+a[j]);
			if(i == R) {
				tree.emplace_back(j, i);
				R = int(a.size())-1;
			} else if(j == R) {
				tree.emplace_back(i, j);
				R = int(a.size())-1;
			} else {
				tree.emplace_back(i, j);
			}
			Q.push((int) tree.size()-1);
		}
		int i2 = Q.top();
		b.resize(a.size());
		for(int k = 0; k < 50; ++k) try_improve(i2);
		if(N < 2000) for(int k = 0; k < 50; ++k) try_improve(i2);
		roots.push_back(i2);


		rinf.clear();
		Q = {};
		for(int i = 0; i < N; ++i) {
			if(abs(a[i]) > abs(GOAL)) Q.push(i);
			else rinf.push_back(i);
		}
		while(Q.size() > 1) {
			const int i = Q.top(); Q.pop();
			const int j = Q.top(); Q.pop();
			a.push_back(a[i]+a[j]);
			tree.emplace_back(i, j);
			if(abs(a.back()) > abs(GOAL)) Q.push((int) tree.size()-1);
			else rinf.push_back((int) tree.size()-1);
		}
		for(int i : rinf) Q.push(i);
		while(Q.size() > 1) {
			const int i = Q.top(); Q.pop();
			const int j = Q.top(); Q.pop();
			a.push_back(a[i]+a[j]);
			tree.emplace_back(i, j);
			Q.push((int) tree.size()-1);
		}
		i2 = Q.top();
		suffleTree(i2);
		for(int k = 0; k < 50; ++k) try_improve(i2);
		if(N < 2000) for(int k = 0; k < 50; ++k) try_improve(i2);
		roots.push_back(i2);

		rsup.resize(N);
		iota(rsup.begin(), rsup.end(), 0);
		i2 = gather2(rsup, -1);
		suffleTree(i2);
		for(int k = 0; k < 50; ++k) try_improve(i2);
		if(N < 2000) for(int k = 0; k < 50; ++k) try_improve(i2);
		roots.push_back(i2);
	}

	// t0 = get_time();
	c.resize(b.size());
	for(int i = 0; i < N; ++i) c[i].emplace_back(0, 0, 0, 0);
	for(int i : roots) prepareDP(i);
	const double end_time2 = start_time + 9.8;
	pair<int, int> nb = {-1, -1};
	for(int i : orderDP) {
		if(i%10 == 0 && get_time() > end_time2) goto END_SHOW;
		const auto &[j, k] = tree[i];
		ranges::sort(b[i]);
		c[i].resize(b[i].size(), {4*N, 0, 0, 'd'});
		for(int u = 0; u < (int) b[j].size(); ++u) {
			const double u16 = toFP16(b[j][u]);
			const double u32 = float(b[j][u]);
			for(int v = 0; v < (int) b[k].size(); ++v) {
				const int wuv = get<0>(c[j][u]) + get<0>(c[k][v]);
				const double h = toFP16(u16 + toFP16(b[k][v]));
				const auto hit = ranges::lower_bound(b[i], h);
				if(hit != b[i].end() && *hit == h) {
					const int w = wuv+1;
					const int l = int(hit-b[i].begin());
					if(get<0>(c[i][l]) > w) c[i][l] = make_tuple(w, u, v, 'h');
				}
				const double s = u32 + double(float(b[k][v]));
				const auto sit = ranges::lower_bound(b[i], s);
				if(sit != b[i].end() && *sit == s) {
					const int w = wuv+2;
					const int l = int(sit-b[i].begin());
					if(get<0>(c[i][l]) > w) c[i][l] = make_tuple(w, u, v, 's');
				}
				const double d = b[j][u] + b[k][v];
				const auto dit = ranges::lower_bound(b[i], d);
				if(dit != b[i].end() && *dit == d) {
					const int w = wuv+4;
					const int l = int(dit-b[i].begin());
					if(get<0>(c[i][l]) > w) c[i][l] = make_tuple(w, u, v, 'd');
				}
			}
		}
	}
	for(int i : roots) {
		if(special_tree[i]) {
			vector<int> rs = {i};
			while(rs.size() < 3 && tree[rs.back()].first < N) rs.push_back(tree[rs.back()].first);
			reverse(rs.begin(), rs.end());
			for(int i : rs) {
				const auto &[j, k] = tree[i];
				for(int u = 0; u < (int) b[j].size(); ++u) {
					const double u16 = toFP16(b[j][u]);
					const double u32 = float(b[j][u]);
					for(int v = 0; v < (int) b[k].size(); ++v) {
						const int wuv = get<0>(c[j][u]) + get<0>(c[k][v]);
						const double sj = u32 + b[k][v];
						const auto sjit = ranges::lower_bound(b[i], sj);
						if(sjit != b[i].end() && *sjit == sj) {
							const int w = wuv+4;
							const int l = int(sjit-b[i].begin());
							if(get<0>(c[i][l]) > w) c[i][l] = make_tuple(w, u, v, 'j');
						}
						const double hj = u16 + b[k][v];
						const auto hjit = ranges::lower_bound(b[i], hj);
						if(hjit != b[i].end() && *hjit == hj) {
							const int w = wuv+4;
							const int l = int(hjit-b[i].begin());
							if(get<0>(c[i][l]) > w) c[i][l] = make_tuple(w, u, v, 'u');
						}
						const double hk = u16 + toFP16(b[k][v]);
						const auto hkit = ranges::lower_bound(b[i], hk);
						if(hkit != b[i].end() && *hkit == hk) {
							const int w = wuv+4;
							const int l = int(hkit-b[i].begin());
							if(get<0>(c[i][l]) > w) c[i][l] = make_tuple(w, u, v, 'v');
						}
					}
				}
			}
		}
		for(int l = 0; l < (int) b[i].size(); ++l) {
			const double C = (double(get<0>(c[i][l])) + pen[i]) / double(N-1);
			const double D = 10. / sqrt(C + 0.5);
			const double x = special_tree2 ? float(b[i][l]) : b[i][l];
			const double A = pow(max(abs(x-GOAL)/max(abs(GOAL), 1e-200), 1e-20), 0.05);
			const double sc2 = D/A;
			if(sc2 > sc) {
				ind = i;
				nb = {i, l};
				sc = sc2;
				// fprintf(stderr, "ScoreDP: %.4f\n", sc);
			}
		}
	}
	if(nb.first != -1) CtoOp(nb.first, nb.second);
	// fprintf(stderr, "Last DP: %.3fs\n", get_time() - t0);

	END_SHOW:
	if(special_tree2) {
		printf("{s:");
		show(ind);
		printf("}");
	} else {
		show(ind);
	}
	printf("\n");

	return 0;
}
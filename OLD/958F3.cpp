#include <iostream>
#include <vector>
#include <queue>

#include "../CPHeaders/ntt.h"
#include "../CPHeaders/mod_arith.h"

using namespace std;
const int MOD = 469762049;
const int MOD2 = 167772161;
const int MOD0 = 1009;
typedef mint<MOD> mi;
typedef mint<MOD2> mi2;
const mi G = 3;
const mi2 G2 = 3;
const long long MOD3 = (long long)MOD*MOD2;
const long long u = 17513998953538448;
const long long v = 61298995162979442;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	int n, m, k;
	cin >> n >> m >> k;
	vector<int> count(m, 0);
	while(n--) {
		int c; cin >> c; --c;
		++ count[c];
	}

	vector<vector<mi>> P;
	vector<vector<mi2>> P2;
	for(int c : count) if(c) {
		P.emplace_back(min(c, k)+1, 1);
		P2.emplace_back(min(c, k)+1, 1);
	}
	const auto comp = [&](int i, int j) { return P[i].size() > P[j].size(); };
	priority_queue<int, vector<int>, decltype(comp)> Q(comp);
	for(int i = 0; i < P.size(); ++i) Q.push(i);
	while(Q.size() > 1) {
		int i = Q.top(); Q.pop();
		int j = Q.top(); Q.pop();
		NTT_mult_inplace(P[i], P[j], G);
		NTT_mult_inplace(P2[i], P2[j], G2);
		if(P[i].size() > k+1) {
			P[i].resize(k+1);
			P2[i].resize(k+1);
		}
		for(int k = 0; k < P[i].size(); ++k) {
			int val = (((__uint128_t)int(P[i][k])*u + (__uint128_t)int(P2[i][k])*v) % MOD3) % MOD0;
			P[i][k] = val;
			P2[i][k] = val;
		}
		Q.emplace(i);
	}
	cout << P[Q.top()][k] << endl;

	return 0;
}
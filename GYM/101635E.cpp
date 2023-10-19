#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;
using vi = vector<int>;

struct Arc {
	int a;
	int c;
	int p;
};

vector<vector<Arc>> G;
vector<int> C;
vector<int> P;

void dfs(int i) {
	if(C[i] != -1) return;
	if(G[i].empty()) {
		C[i] = P[i] = 0;
		return;
	}
	C[i] = 1e9;
	P[i] = 0;
	for(auto &[j, c, p] : G[i]) {
		dfs(j);
		int c2 = C[j] + c;
		int p2 = P[j] + p;
		if(c2 < C[i] || (c2 == C[i] && p2 > P[i])) {
			C[i] = c2;
			P[i] = p2;
		}
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int B, N;
	cin >> B >> N;
	unordered_map<string, int> s2i;
	while(N--) {
		string derived, base, ing;
		int c, p;
		cin >> derived >> base >> ing >> c >> p;
		if(!s2i.count(derived)) {
			s2i[derived] = s2i.size();
			G.emplace_back();
		}
		if(!s2i.count(base)) {
			s2i[base] = s2i.size();
			G.emplace_back();
		}
		int ind_derived = s2i[derived];
		int ind_base = s2i[base];
		G[ind_derived].push_back({ind_base, c, p});
	}

	C.assign(G.size(), -1);
	P.assign(G.size(), -1);

	for(int i = 0; i < G.size(); ++i) dfs(i);

	vector<int> dp(B+1, 0);
	for(int i = 0; i < G.size(); ++i) {
		for(int b = B; b >= C[i]; --b) {
			dp[b] = max(dp[b], dp[b-C[i]] + P[i]);
		}
	}
	int b = B;
	while(b > 0 && dp[b-1] == dp[B]) --b;
	cout << dp[B] << '\n' << b << '\n';

	return 0;
}
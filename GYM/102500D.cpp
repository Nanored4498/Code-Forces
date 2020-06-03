#include <iostream> 
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<ll, int> pli;
typedef pair<int, int> pii;
typedef vector<ll> vl;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	vector<vector<pli>> G(n+1);
	while(m --> 0) {
		int a, b;
		ll l;
		cin >> a >> b >> l;
		G[a].emplace_back(l, b);
		G[b].emplace_back(l, a);
	}
	vector<vl> ds(n, vl(n+1, -1));
	ds[0][1] = 0;
	for(int k = 1; k < n; ++k) {
		for(int i = 1; i <= n; ++i) if(ds[k-1][i] >= 0) {
			for(const pli &e : G[i]) {
				int j = e.second;
				ll lj = e.first + ds[k-1][i];
				if(ds[k][j] < 0 || ds[k][j] > lj) ds[k][j] = lj;
			}
		}
	}
	vector<vector<bool>> usefull(n, vector<bool>(n+1, false));
	vector<bool> usefull2(n+1, false);
	int K = n;
	vector<pii> st;
	vector<pli> inter;
	for(int k = 1; k < n; ++k) if(ds[k][n] >= 0) {
		pli in = { 3e12, 1 };
		while(!st.empty()) {
			in = { ds[st.back().first][n] - ds[k][n], k - st.back().first };
			if(in.first*inter.back().second <= inter.back().first*in.second) break;
			else {
				inter.pop_back();
				st.pop_back();
			}
		}
		if(in.first >= 0) {
			inter.push_back(in);
			st.emplace_back(k, n);
			usefull[k][n] = true;
		}
	}
	while(!st.empty()) {
		int k = st.back().first, i = st.back().second; st.pop_back();
		if(!usefull2[i]) {
			usefull2[i] = true;
			-- K;
		}
		if(k < 1) continue;
		for(const pli &e : G[i]) {
			int j = e.second;
			if(usefull[k-1][j]) continue;
			ll l = e.first;
			if(ds[k-1][j] >= 0 && ds[k][i] == ds[k-1][j]+l) {
				usefull[k-1][j] = true;
				st.emplace_back(k-1, j);
			}
		}
	}
	cout << K << "\n";
	for(int i = 1; i <= n; ++i) if(!usefull2[i]) cout << i << " ";
	cout << "\n";

	return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
using ll = long long;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int I, S, L, U;
	cin >> I >> S >> L >> U;
	vector<vector<pair<int, int>>> G(I);
	while(S--) {
		int i, j, l;
		cin >> i >> j >> l;
		G[i].emplace_back(j, l);
		G[j].emplace_back(i, l);
	}
	vector<int> dist(I, 1e9);
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int,int>>> Q;
	Q.emplace(dist[0] = 0, 0);
	while(!Q.empty()) {
		auto [d, i] = Q.top();
		Q.pop();
		if(d != dist[i]) continue;
		for(auto [j, lij] : G[i]) {
			int l = d + lij;
			if(l < dist[j]) {
				dist[j] = l;
				Q.emplace(l, j);
			}
		}
	}

	int ans = 0;
	for(int i = 0; i < I; ++i) for(auto [j, lij] : G[i]) {
		if(2*min(dist[i], dist[j]) < U) ++ ans;
	}
	ans /= 2;
	cout << ans << '\n';

	return 0;
}
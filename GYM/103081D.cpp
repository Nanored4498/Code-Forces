#include <iostream>
#include <vector>
#include <queue>

using namespace std;
typedef pair<int, int> pii;
typedef vector<pii> vii;
const int MAX_I = 100'000;
const int MAX_l = 1'000;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int I, S, L, U;
	cin >> I >> S >> L >> U;
	vector<vii> G(I);
	for(int s = 0; s < S; ++s) {
		int i, j, l;
		cin >> i >> j >> l;
		G[i].emplace_back(j, l);
		G[j].emplace_back(i, l);
	}

	vector<int> d(I, MAX_I*MAX_l);
	priority_queue<pii, vii, greater<pii>> Q;
	d[0] = 0;
	Q.emplace(0, 0);
	while(!Q.empty()) {
		auto [l, i] = Q.top();
		Q.pop();
		if(d[i] != l) continue;
		for(auto [j, lij] : G[i]) {
			int lj = l + lij;
			if(lj < d[j]) {
				d[j] = lj;
				Q.emplace(lj, j);
			}
		}
	}

	int ans = 0;
	for(int i = 0; i < I; ++i)
		for(auto [j, _] : G[i])
			if(j < i && 2*min(d[i], d[j]) < U)
				++ ans;
	cout << ans << '\n';

	return 0;
}
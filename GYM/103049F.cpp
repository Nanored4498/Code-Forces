#include <iostream>
#include <queue>
#include <vector>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n; cin >> n;
	vector<int> nxt(n), prd(n), x(n), v(n);
	const auto comp = [&](const pii &a, const pii &b) {
		const ll dxa =  x[a.second] - x[a.first];
		const ll dva = -v[a.second] + v[a.first];
		const ll dxb =  x[b.second] - x[b.first];
		const ll dvb = -v[b.second] + v[b.first];
		return dxa*dvb > dxb*dva;
	};
	priority_queue<pii, vector<pii>, decltype(comp)> Q(comp);
	for(int i = 0; i < n; ++i) {
		prd[i] = i-1;
		nxt[i] = i+1;
		cin >> x[i] >> v[i];
		if(i && v[i-1] > v[i]) Q.emplace(i-1, i);
	}
	vector<bool> ok(n, true);
	while(!Q.empty()) {
		auto [i, j] = Q.top(); Q.pop();
		if(!ok[i] || !ok[j]) continue;
		ok[i] = ok[j] = false;
		int k = prd[i], l = nxt[j];
		if(k != -1) nxt[k] = l;
		if(l != n) prd[l] = k;
		if(k != -1 && l != n && v[k] > v[l]) Q.emplace(k, l);
	}
	int ans = 0;
	for(int i = 0; i < n; ++i) ans += ok[i];
	cout << ans << '\n';
	for(int i = 0; i < n; ++i) if(ok[i]) cout << i+1 << ' ';
	cout << '\n';

	return 0;
}
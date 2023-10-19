#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;
using ll = long long;
using vi = vector<int>;

vector<vi> children;
vector<char> in;
vi S, cs;

ll f(int i) {
	ll ans = 0;
	int c = 0;
	for(int j : children[i]) ans += f(j);

	map<int, int> mcs;
	for(int j : children[i]) {
		++ mcs[cs[j]];
		c += cs[j];
	}
	const int c2 = c/2;
	int best = 0;
	S.push_back(0);
	for(const auto [cj, ck] : mcs) {
		const int n = S.size();
		int add = cj;
		for(int k = 1; k <= ck; ++k, add+=cj) for(int j = 0; j < n; ++j) if(int x = S[j]+add; x <= c2 && !in[x]) {
			in[x] = 1;
			S.push_back(x);
			best = max(best, x);
		}
	}
	while(!S.empty()) {
		in[S.back()] = 0;
		S.pop_back();
	}
	ans += (ll) best * (c - best);

	cs[i] = c+1;
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n; cin >> n;
	children.resize(n+1);
	for(int i = 2; i <= n; ++i) {
		int p; cin >> p;
		children[p].push_back(i);
	}
	in.resize(n/2+1, 0);
	cs.resize(n+1);

	cout << f(1) << endl;

	return 0;
}
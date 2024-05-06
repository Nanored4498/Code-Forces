#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int k, n;
	cin >> k >> n;
	vi A(k), B(k);
	vi p[4][4];
	for(int &x : A) cin >> x;
	for(int &x : B) cin >> x;
	for(int i = 0; i < k; ++i) p[A[i]][B[i]].push_back(i+1);

	vi ans[3] {vi(n), vi(n), vi(n)};
	for(int i = 0; i < p[2][2].size(); ++i) {
		for(int a : {1, 2}) for(int b : {2*i, 2*i+1})
			ans[a][b] = p[2][2][i];
	}
	const int I = 2*p[2][2].size();

	int i = 0;
	for(int l : {3, 2, 1}) {
		while(i+l <= I && !p[1][l].empty()) {
			int j = p[1][l].back();
			p[1][l].pop_back();
			for(int k = 0; k < l; ++k) ans[0][i++] = j;
		}
	}
	if(i != I) {
		int j = p[1][2].back(); p[1][2].pop_back();
		int k = p[1][2].back(); p[1][2].pop_back();
		ans[0][i] = j;
		++i;
		ans[0][i] = j;
		ans[1][i] = k;
		ans[2][i] = k;
		++i;
	}
	while(!p[1][2].empty()) {
		int j = p[1][2].back(); p[1][2].pop_back();
		if(p[1][1].empty()) {
			int k = p[1][2].back(); p[1][2].pop_back();
			int l = p[1][2].back(); p[1][2].pop_back();
			ans[0][i] = k;
			ans[1][i] = j;
			ans[2][i] = j;
			++i;
			ans[0][i] = k;
			ans[1][i] = l;
			ans[2][i] = l;
			++i;
		} else {
			int k = p[1][1].back(); p[1][1].pop_back();
			ans[0][i] = k;
			ans[1][i] = j;
			ans[2][i] = j;
			++i;
		}
	}
	while(!p[1][1].empty()) {
		for(int k : {0, 1, 2}) {
			int j = p[1][1].back(); p[1][1].pop_back();
			ans[k][i] = j;
		}
		++i;
	}

	for(int l : {1, 2, 3}) for(int j : p[l][3]) {
		for(int k = 0; k < l; ++k) {
			for(int u : {0, 1, 2})
				ans[u][i] = j;
			++i;
		}
	}

	for(const auto &v : ans) {
		for(int i : v) cout << i << ' ';
		cout << '\n';
	}

	return 0;
}
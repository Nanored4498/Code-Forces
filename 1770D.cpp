#include <iostream>
#include <vector>

using namespace std;
using ll = long long;
const int MOD = 998244353;

struct UF {
	vector<int> r;
	UF(int n): r(n, -1) {}
	int find(int i) {
		if(r[i] < 0) return i;
		return r[i] = find(r[i]);
	}
	void merge(int i, int j) {
		i = find(i); j = find(j);
		if(i == j) return;
		if(r[i] > r[j]) swap(i, j);
		r[i] += r[j];
		r[j] = i;
	}
	bool isRoot(int i) { return r[i] < 0; }
	int size(int i) { return -r[find(i)]; }
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n; cin >> n;
		vector<int> a(n), b(n);
		for(int &x : a) { cin >> x; --x; }
		for(int &x : b) { cin >> x; --x; }
		vector<vector<int>> pos(n);
		vector<int> count(n, 0);
		vector<bool> used(n, false);
		vector<int> Q;
		for(int i = 0; i < n; ++i) {
			++ count[a[i]];
			pos[a[i]].push_back(i);
			++ count[b[i]];
			pos[b[i]].push_back(i);
		}
		int ans = 1;
		UF u(n);
		for(int i = 0; i < n; ++i) if(count[i] <= 1) Q.push_back(i);
		while(!Q.empty()) {
			int i = Q.back(); Q.pop_back();
			if(count[i] == 0) {
				ans = 0;
				goto show;
			}
			while(used[pos[i].back()]) pos[i].pop_back();
			int p = pos[i].back();
			if((-- count[a[p]]) <= 1 && a[p] != i) Q.push_back(a[p]);
			if((-- count[b[p]]) <= 1 && b[p] != i) Q.push_back(b[p]);
			used[p] = true;
		}
		for(int i = 0; i < n; ++i) if(!used[i]) {
			if(a[i] == b[i]) ans = (ll) ans * n % MOD;
			else u.merge(a[i], b[i]);
		}
		for(int i = 0; i < n; ++i) if(u.isRoot(i) && u.size(i) >= 2) ans = (ll) ans * 2 % MOD;
		show:
		cout << ans << '\n';
	}

	return 0;
}
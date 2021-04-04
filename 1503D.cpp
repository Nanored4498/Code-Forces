#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<int> g(2*n+1);
	vector<bool> up(2*n+1, false);
	vector<bool> used(2*n+1, false);
	
	for(int i = 0; i < n; ++ i) {
		int a, b;
		cin >> a >> b;
		g[a] = b;
		g[b] = a;
		up[a] = true;
	}

	int a = 1, b = 2*n;
	int ans = 0;
	while(a <= n) {
		int f = 0, c = 0;
		int r = a, l = 2*n+1;
		int r2 = g[a], l2 = 0;
		used[a] = used[g[a]] = true;
		if(up[a]) ++ c;
		else ++ f;
		int mi = a, ma = g[a];
		while(a <= mi || b >= ma) {
			if(a <= mi) {
				if(used[a]) {
					++ a;
					continue;
				}
				used[a] = used[g[a]] = true;
				if(a > r) {
					if(a >= l || g[a] >= r2 || g[a] <= l2) return cout << "-1\n", 0;
					if(up[a]) ++ c;
					else ++ f;
					r = a;
					r2 = g[a];
					ma = min(ma, r2);
				} else { // a > l2
					if(a >= r2 || g[a] <= r || g[a] >= l) return cout << "-1\n", 0;
					if(up[a]) ++ f;
					else ++ c;
					l = g[a];
					l2 = a;
					ma = min(ma, l);
				}
				++ a;
			} else { // b >= ma
				if(used[b]) {
					-- b;
					continue;
				}
				used[b] = used[g[b]] = true;
				if(b < r2) {
					if(b <= l2 || g[b] <= r || g[b] >= l) return cout << "-1\n", 0;
					if(up[b]) ++ f;
					else ++ c;
					r = g[b];
					r2 = b;
					mi = max(mi, r);
				} else { // b < l
					if(b <= r || g[b] >= r2 || g[b] <= l2) return cout << "-1\n", 0;
					if(up[b]) ++ c;
					else ++ f;
					l = b;
					l2 = g[b];
					mi = max(mi, l2);
				}
				-- b;
			}
		}
		ans += min(f, c);
	}
	cout << ans << '\n';

	return 0;
}
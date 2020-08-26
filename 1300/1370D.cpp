#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi p, s;
int find(int i) {
	if(p[i] == i) return i;
	p[i] = find(p[i]);
	return p[i];
}
void merge(int i, int j) {
	int ri = find(i), rj = find(j);
	if(ri == rj) return;
	if(s[ri] < s[rj]) swap(ri, rj);
	p[rj] = ri;
	s[ri] += s[rj];
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	cin >> n >> k;
	vi a(n), order(n);
	p.resize(n);
	s.assign(n, 1);
	vector<bool> used(n, false);

	for(int i = 0; i < n; ++i) {
		cin >> a[i];
		order[i] = i;
		p[i] = i;
	}
	sort(order.begin(), order.end(), [&](int i, int j) { return a[i] < a[j]; });
	int l = k/2, i = 0;
	int ans = 1e9;
	if(k&1) {
		while(l > 0) {
			int x = order[i++];
			while(x == 0 || x == n-1) x = order[i++];
			used[x] = true;
			int w = (x+n-1)%n, y = (x+1)%n;
			if(used[w]) {
				l += (s[find(w)]+1)/2;
				merge(w, x);
			}
			if(used[y]) {
				l += (s[find(y)]+1)/2;
				merge(y, x);
			}
			l -= (s[find(x)]+1)/2;
		}
		used.assign(n, false);
		s.assign(n, 1);
		for(int i = 0; i < n; ++i) p[i] = i;
		ans = a[order[--i]];
		l = (k+1)/2;
		i = 0;
	}
	while(l > 0) {
		int x = order[i++];
		used[x] = true;
		int w = (x+n-1)%n, y = (x+1)%n;
		if(used[w] && (w != n-1 || k%2 == 0)) {
			l += (s[find(w)]+1)/2;
			merge(w, x);
		}
		if(used[y] && find(y) != find(x) && (y != 0 || k%2 == 0)) {
			l += (s[find(y)]+1)/2;
			merge(y, x);
		}
		l -= (s[find(x)]+1)/2;
	}
	ans = min(ans, a[order[--i]]);
	cout << ans << "\n";

	return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<ll> vl;

int n, p = 1, ans = 0;
vi a, b, neck;
vi nodes[1<<20];
vector<bool> used;

void construct(int q, int s=-1) {
	while(!nodes[q].empty()) {
		int i = nodes[q].back(); nodes[q].pop_back();
		if(used[i/2]) continue;
		used[i/2] = true;
		int v = i&1 ? a[i/2] : b[i/2];
		construct(v&p, i);
	}
	if(s != -1) {
		neck.push_back(s^1);
		neck.push_back(s);
	}
}

void init() {
	for(int i = 0; i <= p; ++i) nodes[i].clear();
	for(int i = 0; i < n; ++i) {
		int ap = a[i]&p, bp = b[i]&p;
		nodes[ap].push_back(2*i);
		nodes[bp].push_back(2*i+1);
	}
	neck.clear();
	used.assign(n, false);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n;
	a.resize(n);
	b.resize(n);
	for(int i = 0; i < n; ++i) cin >> a[i] >> b[i];
	while(ans < 20) {
		init();
		bool ok = true;
		for(int i = 0; i <= p; ++i)
			if(nodes[i].size()&1) {
				ok = false;
				break;
			} 
		if(ok) {
			construct(a[0]&p);
			if(neck.size() != 2*n) break;
			p = (p << 1) + 1;
			++ ans;
		} else break;
	}
	cout << ans << "\n";
	p >>= 1;
	init();
	construct(a[0]&p);
	for(int i = 0; i < 2*n; ++i) cout << neck[i]+1 << " \n"[i+1 == 2*n];

	return 0;
}
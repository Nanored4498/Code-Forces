#include <iostream>
#include <vector>
#include <algorithm>
#include "../CPHeaders/segment_tree.h"

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m;
		cin >> n >> m;
		vector<ll> a(n);
		for(ll &x : a) cin >> x;
		sort(a.rbegin(), a.rend());
		a.resize(m);
		vector<ll> s(m, 0);
		vector<vector<int>> b(m);
		vector<int> order(m), ro(m);
		for(int i = 0; i < m; ++i) {
			int k;
			cin >> k;
			b[i].resize(k);
			for(int &x : b[i]) {
				cin >> x;
				s[i] += x;
			}
			order[i] = i;
		}
		sort(order.rbegin(), order.rend(), [&](int i, int j) { return s[i]*b[j].size() < s[j]*b[i].size(); });
		Fenwick<int> f(m), g(m), h(m);
		for(int i = 0; i < m; ++i) {
			int j = order[i];
			if(i > 0 && a[i-1]*b[j].size() < s[j]) f.add(i+1, 1);
			if(a[i]*b[j].size() < s[j]) g.add(i+1, 1);
			if(i+1 < m && a[i+1]*b[j].size() < s[j]) h.add(i+1, 1);
			ro[j] = i;
		}
		for(int i = 0; i < m; ++i) {
			int j = ro[i];
			int size2 = b[i].size()-1;
			for(int x : b[i]) {
				ll s2 = s[i]-x;
				int k = -1;
				for(int add = m/2; add > 0; add>>=1)
					while(k+add < m && s[order[k+add]]*size2 > s2*b[order[k+add]].size())
						k += add;
				if(k < j) ++ k;
				bool bad = a[k]*size2 < s2;
				if(k <= j) bad |= g.querry(k) + h.querry(k+1, j) + g.querry(j+2, m);
				else bad |= g.querry(j) + f.querry(j+2, k+1) + g.querry(k+2, m);
				cout << (bad ? '0' : '1');
			}
		}
		cout << '\n';
	}

	return 0;
}
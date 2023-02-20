#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	int t; cin >> t;
	while(t--) {
		int n, m;
		cin >> n >> m;
		vector<int> a(n);
		for(int &x : a) cin >> x;
		vector<int> b = a;
		sort(a.begin(), a.end());
		int i = 0;
		int s = 0;
		while(i < n && s+a[i] <= m) s += a[i++];
		if(i > 0 && i < n && s + b[i] - a[i-1] <= m) ++i;
		cout << n+1 - i << '\n';
	}

	return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <set>

using namespace std;
using ll = long long;
using vi = vector<int>;

// vector<int> div(int n) {
// 	vector<int> d(1, 1);
// 	for(int i = 2; i*i <= n; ++i) if(n%i == 0) {
// 		const int nd = d.size();
// 		int j = 1;
// 		while(n%i == 0) {
// 			j *= i;
// 			for(int k = 0; k < nd; ++k) d.push_back(d[k]*j);
// 			n /= i;
// 		}
// 	}
// 	if(n > 1) {
// 		const int nd = d.size();
// 		for(int k = 0; k < nd; ++k) d.push_back(d[k]*n);
// 	}
// 	return d;
// }

vector<int> div(int n) {
	int sn = sqrt(n);
	vector<int> d;
	for(int i = 1; i <= sn; ++i) if(n%i == 0) {
		d.push_back(i);
		if(n/i != i) d.push_back(n/i);
	}
	return d;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int l, w;
		cin >> l >> w;
		vector<int> a = div(gcd(w, l-2)), b = div(gcd(w-1, l-1)), c = div(gcd(w-2, l));
		/*
		a.insert(a.end(), b.begin(), b.end());
		a.insert(a.end(), c.begin(), c.end());
		a.push_back(2);
		sort(a.begin(), a.end());
		a.erase(unique(a.begin(), a.end()), a.end());
		cout << a.size();
		for(int x : a) cout << ' ' << x;
		cout << '\n';
		*/
		set<int> S;
		for(int x : a) S.insert(x);
		for(int x : b) S.insert(x);
		for(int x : c) S.insert(x);
		S.insert(2);
		cout << S.size();
		for(int x : S) cout << ' ' << x;
		cout << '\n';
	}

	return 0;
}
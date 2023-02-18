#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vector<int> a(n);
		for(int &x : a) cin >> x;
		int sum = 0;
		for(int x : a) sum += x;
		if(sum) {
			if(sum > 0) sort(a.rbegin(), a.rend());
			else sort(a.begin(), a.end());
			cout << "YES\n";
			for(int x : a) cout << x << ' ';
			cout << '\n';
		} else cout << "NO\n";
	}

	return 0;
}
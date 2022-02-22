#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;
const int MOD = 1e9+7;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, p; cin >> n >> p;
	vector<int> a(n);
	for(int &x : a) cin >> x;

	sort(a.begin(), a.end());
	unordered_set<int> s;
	for(int x : a) {
		bool ok = true;
		int y = x;
		while(ok && y) {
			ok &= !s.count(y);
			if(y&1) y >>= 1;
			else if(y&2) break;
			else y >>= 2;
		}
		if(ok) s.insert(x);
	}

	vector<int> fibo(p+1);
	fibo[0] = fibo[1] = 1;
	for(int i = 2; i <= p; ++i) fibo[i] = (fibo[i-1] + fibo[i-2]) % MOD;
	for(int i = 1; i <= p; ++i) fibo[i] = (fibo[i] + fibo[i-1]) % MOD;

	int ans = 0;
	for(int x : s) {
		int k = 0;
		while(x) {
			++ k;
			x >>= 1;
		}
		if(p >= k) ans = (ans + fibo[p-k]) % MOD;
	}
	cout << ans << '\n';

	return 0;
}
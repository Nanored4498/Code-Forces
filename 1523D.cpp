#include <iostream>
#include <vector>
#include <ctime>
#include <random>

using namespace std;
typedef long long ll;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, p;
	cin >> n >> m >> p;
	int n2 = (n+1)/2;
	vector<ll> mask(n, 0);
	for(int i = 0; i < n; ++i) {
		string s;
		cin >> s;
		ll x = 1;
		for(int j = 0; j < m; ++j, x <<= 1) if(s[j] == '1')
			mask[i] += x;
	}

	ll ans = 0;
	int ansb = 0;
	vi count, nb;
	default_random_engine re(time(nullptr));
	uniform_int_distribution<int> unif(0, n-1);
	for(int s = 0; s < 30; ++s) {
		int ind = unif(re);
		ll x = mask[ind];
		vi ones;
		for(int i = 0; x > 0; x >>= 1, ++i)
			if(x&1) ones.push_back(i);
		count.assign(1<<ones.size(), 0);
		nb.assign(1<<ones.size(), 0);
		for(int i = 0; i < n; ++i) {
			int inter = 0;
			for(int j = 0; j < ones.size(); ++j) if(mask[i] & (1LL << ones[j]))
				inter += 1 << j;
			++ count[inter];
		}
		for(int i = 0; i < ones.size(); ++i) {
			int b = 1<<i;
			for(int j = 0; j < count.size(); ++j)
				if(j&b) ++ nb[j];
				else count[j] += count[j^b];
		}
		for(int i = 0; i < count.size(); ++i) if(count[i] >= n2 && nb[i] > ansb) {
			ansb = nb[i];
			ans = 0;
			for(int j = 0; j < ones.size(); ++j) if(i & (1 << j))
				ans += 1LL << ones[j];
		}
	}

	for(int i = 0; i < m; ++i) {
		if(ans&1) cout << '1';
		else cout << '0';
		ans >>= 1;
	}
	cout << '\n';

	return 0;
}
#include <iostream>
#include <vector>

#include "../CPHeaders/ntt.h"
#include "../CPHeaders/mod_arith.h"

using namespace std;
const int MOD = 167772161;
typedef mint<MOD> mi;
const mi G = 3;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	const int L = 1e6+1;
	vector<int> prime(L, 0);
	for(int i = 2; i < L; ++i) if(!prime[i])
		for(int j = i; j < L; j += i)
			prime[j] = i;

	int n, x, y;
	cin >> n >> x >> y;
	vector<mi> P(x+1, 0), Q(x+1, 0);
	++n;
	while(n--) {
		int a; cin >> a;
		P[a] = 1;
		Q[x-a] = 1;
	}
	NTT_mult_inplace(P, Q, G);
	
	int q;
	cin >> q;
	while(q--) {
		int l; cin >> l;
		const int L = l;
		if(l/2-y <= 0) {
			cout << "-1 ";
			continue;
		}
		vector<int> d = {1};
		while(l != 1) {
			int p = prime[l];
			int e = 0;
			while(prime[l] == p) {
				++ e;
				l /= p;
			}
			int nd = d.size();
			for(int f = 1, pf = p; f <= e; ++f, pf*=p)
				for(int i = 0; i < nd; ++i)
					d.push_back(d[i]*pf);
		}
		int ans = -1;
		for(int i : d) if(i%2==0 && i/2-y > 0 && i/2-y <= x && P[x+i/2-y] != 0 && (ans < i)) ans = i;
		cout << ans << ' '; 
	}
	cout << endl;

	return 0;
}
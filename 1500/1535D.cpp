#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int k;
	cin >> k;
	string w;
	cin >> w;
	int q;
	cin >> q;

	reverse(w.begin(), w.end());
	vector<int> f(1<<(k+1), 1);
	for(int i = (1<<k)-1; i > 0; --i)
		if(w[i-1] == '?') f[i] = f[2*i] + f[2*i+1];
		else f[i] = f[2*i + '1' - w[i-1]];

	while(q--) {
		int p;
		char c;
		cin >> p >> c;
		p = w.size() - p;
		w[p] = c;
		for(++p; p > 0; p /= 2)
			if(w[p-1] == '?') f[p] = f[2*p] + f[2*p+1];
			else f[p] = f[2*p + '1' - w[p-1]];
		cout << f[1] << '\n';
	}

	return 0;
}
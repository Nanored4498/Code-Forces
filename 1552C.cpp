#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		vector<int> to(2*n, -1);
		for(int i = 0; i < k; ++i) {
			int a, b;
			cin >> a >> b;
			to[--a] = --b;
			to[b] = a;
		}
		if(k < n) {
			int r = n-k;
			int j = -1;
			while(r-- > 0)
				while(to[++j] != -1);
			for(int i = 0; i < 2*n; ++i) if(to[i] == -1) {
				while(to[++j] != -1);
				to[i] = j;
				to[j] = i;
			}
		}
		int ans = 0;
		for(int i = 0; i < 2*n; ++i) if(to[i] > i) {
			for(int j = i+1; j < to[i]; ++j)
				if(to[j] < i || to[j] > to[i])
					++ ans;
		}
		cout << ans/2 << endl;
	}

	return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vi p(n);
		for(int i = 0; i < n; ++i) cin >> p[i];
		vi r;
		r.push_back(p[0]);
		for(int i = 1; i < n-1; ++i) {
			if(p[i] > r.back() && p[i+1] > p[i]) continue;
			if(p[i] < r.back() && p[i+1] < p[i]) continue;
			r.push_back(p[i]);
		}
		r.push_back(p.back());
		int k = r.size();
		cout << k << "\n";
		for(int i = 0; i < k; ++i) cout << r[i] << " \n"[i+1==k];
	}

	return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<pii> vii;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n;
	cin >> t;
	while(t--) {
		cin >> n;
		vi a(n), b(n);
		for(int i = 0; i < n; ++i) cin >> a[i];
		for(int i = 0; i < n; ++i) cin >> b[i];
		bool invalid = n&1 && a[n/2] != b[n/2];
		vii pa(n/2), pb(n/2);
		for(int i = 0; i < n/2; ++i) {
			pa[i] = {a[i], a[n-1-i]};
			if(pa[i].first > pa[i].second) swap(pa[i].first, pa[i].second);
		}
		for(int i = 0; i < n/2; ++i) {
			pb[i] = {b[i], b[n-1-i]};
			if(pb[i].first > pb[i].second) swap(pb[i].first, pb[i].second);
		}
		sort(pa.begin(), pa.end());
		sort(pb.begin(), pb.end());
		for(int i = 0; i < n/2; ++i) if(pa[i] != pb[i]) invalid = true;
		if(invalid) cout << "No\n";
		else cout << "yes\n";
	}

	return 0;
}
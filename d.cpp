#include <iostream>
#include <vector>
#include <set>

using namespace std;
typedef vector<int> vi;

void solve() {
	int n;
	cin >> n;
	vi b(n);
	set<int> S;
	cin >> b[0];
	auto it = S.insert(b[0]).first;
	for(int i = 1; i < n; ++i) {
		cin >> b[i];
		if(b[i] > *it) {
			++ it;
			if(it == S.end() || b[i] < *it) it = S.emplace_hint(it, b[i]);
			else if(b[i] > *it) {
				cout << "NO\n";
				while(++i < n) cin >> b[i];
				return;
			}
		} else if(b[i] < *it) {
			if(it == S.begin()) it = S.emplace_hint(it, b[i]);
			else {
				-- it;
				if(b[i] < *it) {
					cout << "NO\n";
					while(++i < n) cin >> b[i];
					return;
				}
				if(b[i] > *it) it = S.emplace_hint(++it, b[i]);
			}
		}
	}
	cout << "YES\n";
	// cout << b[0];
	// S.erase(b[0]);
	// const auto sup = [&](int i)->void {
	// 	auto p = S.upper_bound(b[i-1]);
	// 	if(p != S.end()) {
	// 		cout << ' ' << *p;
	// 		S.erase(p);
	// 	} else cout << ' ' << int(1e9+1);
	// };
	// const auto inf = [&](int i)->void {
	// 	auto p = S.lower_bound(b[i-1]);
	// 	if(p != S.begin()) {
	// 		-- p;
	// 		cout << ' ' << *p;
	// 		S.erase(p); 
	// 	} else cout << ' ' << int(-1e9-1);
	// };
	// for(int i = 1; i < n; ++i) {
	// 	if(b[i] == b[i-1]) {
	// 		inf(i);
	// 		sup(i);
	// 	} else if(b[i] > b[i-1]) {
	// 		sup(i);
	// 		sup(i);
	// 	} else {
	// 		inf(i);
	// 		inf(i);
	// 	}
	// }
	// cout << '\n';
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) solve();

	return 0;
}
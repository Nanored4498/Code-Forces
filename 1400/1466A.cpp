#include <iostream>
#include <vector>
#include <set>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		set<int> s;
		vi x(n);
		for(int i = 0; i < n; ++i) {
			cin >> x[i];
			for(int j = 0; j < i; ++j) s.insert(x[i] - x[j]);
		}
		cout << s.size() << "\n";
	}

	return 0;
}
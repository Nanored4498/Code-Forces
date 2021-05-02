#include <iostream>
#include <vector>

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
		int n, m, k;
		cin >> n >> m >> k;
		if((n-1)*1 + n*(m-1) == k) cout << "YES\n";
		else cout << "NO\n";
	}

	return 0;
}
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
		int r, b, d;
		cin >> r >> b >> d;
		if(r > b) swap(r, b);
		int m = (b-1) / r;
		if(m <= d) cout << "YES\n";
		else cout << "NO\n";
	}

	return 0;
}
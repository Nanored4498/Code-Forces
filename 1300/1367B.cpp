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
		int n, a, b[2]={0,0};
		cin >> n;
		for(int i = 0; i < n; ++i) {
			cin >> a;
			if((i&1) != (a&1)) ++b[i&1];
		}
		if(b[0] == b[1]) cout << b[0] << "\n";
		else cout << "-1\n";
	}

	return 0;
}
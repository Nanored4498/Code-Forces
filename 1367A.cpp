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
		string a, b;
		cin >> b;
		a.push_back(b[0]);
		for(int i = 1; i < b.size(); i += 2) a.push_back(b[i]);
		cout << a << "\n";
	}

	return 0;
}
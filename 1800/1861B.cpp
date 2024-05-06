#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		string a, b;
		cin >> a >> b;
		for(char &c : a) c -= '0';
		for(char &c : b) c -= '0';
		int i = 1, j = 1;
		while(true) {
			while(!a[i]) ++i;
			while(!b[j]) ++j;
			if(i == j) {
				cout << "YES\n";
				break;
			}
			if(i > j) {
				swap(i, j);
				swap(a, b);
			}
			while(i < a.size() && a[i]) ++i;
			if(i == a.size()) {
				cout << "NO\n";
				break;
			}
		}
	}

	return 0;
}
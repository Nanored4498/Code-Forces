#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<bool> X(1000001, true);
	string x;
	for(int i = 0; i < n; ++i) {
		cin >> x;
		if(x.size() > 6) continue;
		int a = atoi(x.c_str());
		if(a > 1000000 || a < 0) continue;
		X[a] = false;
	}
	int i = 0;
	while(!X[i]) ++i;
	cout << i << "\n";

	return 0;
}
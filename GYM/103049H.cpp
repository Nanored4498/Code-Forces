#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n; cin >> n;
	vector<int> t(n);
	for(int &x : t) cin >> x;
	sort(t.begin(), t.end());
	int l=(n-1)/2, r=l+1;
	while(l>=0) {
		cout << t[l--] << ' ';
		if(r>=n) break;
		cout << t[r++] << ' ';
	}
	cout << endl;

	return 0;
}
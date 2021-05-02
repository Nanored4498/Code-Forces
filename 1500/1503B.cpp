#include <iostream>
#include <vector>

using namespace std;

int main() {
	int n, a;
	cin >> n;
	vector<pair<int, int>> x, y;
	for(int i = 1; i <= n; ++i)
		for(int j = 1; j <= n; ++j)
			if((i+j)&1) y.emplace_back(i, j);
			else x.emplace_back(i, j);
	while(!x.empty() && !y.empty()) {
		cin >> a;
		if(a == 1) {
			cout << "2 " << y.back().first << " " << y.back().second << endl;
			y.pop_back();
		} else {
			cout << "1 " << x.back().first << " " << x.back().second << endl;
			x.pop_back();
		}
	}
	while(!x.empty() || !y.empty()) {
		cin >> a;
		if(x.empty()) {
			if(a == 2) cout << "3 " << y.back().first << " " << y.back().second << endl;
			else cout << "2 " << y.back().first << " " << y.back().second << endl;
			y.pop_back();
		} else {
			if(a == 1) cout << "3 " << x.back().first << " " << x.back().second << endl;
			else cout << "1 " << x.back().first << " " << x.back().second << endl;
			x.pop_back();
		}
	}

	return 0;
}
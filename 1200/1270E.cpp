#include <iostream>
#include <vector>

using namespace std;
typedef pair<int, int> pii;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<pii> p(n);
	for(int i = 0; i < n; ++i) cin >> p[i].first >> p[i].second;
	
	while(true) {
		int c[2] = {0, 0};
		for(const pii &a : p) ++ c[(a.first + a.second)&1];
		if(c[0] && c[1]) {
			cout << c[1] << '\n';
			for(int i = 0; i < n; ++i)
				if((p[i].first + p[i].second)&1)
					cout << i+1 << ' ';
			cout << '\n';
			return 0;
		} else {
			if(c[1]) for(pii &a : p) a.first += 1;
			for(pii &a : p) a = { (a.first - a.second) / 2, (a.first + a.second) / 2 };
		}
	}

	return 0;
}
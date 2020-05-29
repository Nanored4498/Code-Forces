#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int X, Y, N;
	cin >> X >> Y >> N;
	vi fst(X, -1), lst(X, -1);
	for(int i = 0; i < N; ++i) {
		int x, y;
		cin >> x >> y;
		if(fst[x] == -1) fst[x] = lst[x] = y;
		else {
			fst[x] = min(fst[x], y);
			lst[x] = max(lst[x], y);
		}
	}
	ll add = 0, d = X-1;
	vi shift(Y, 0);
	for(int x = 0; x < X; ++x) if(fst[x] != -1) {
		add -= 2;
		shift[fst[x]] += 2;
		shift[lst[x]] += 2;
		d += 2*lst[x];
	}
	ll res = d;
	for(int y = 0; y < Y-1; ++y) {
		add += shift[y];
		d += add;
		res = min(res, d);
	}
	cout << res << "\n";

	return 0;
}
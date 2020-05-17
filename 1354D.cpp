#include <iostream>
#include <vector>
#include <set>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> pii;
#define LSB(i) ((i)&(-i))

int n, q, x;
vi a;

void add(int i, int x) {
	while(i < a.size()) {
		a[i] += x;
		i += LSB(i);
	}
}

int querry(int i) {
	int res = 0;
	while(i > 0) {
		res += a[i];
		i -= LSB(i);
	}
	return res;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> q;
	a.assign(n+1, 0);
	for(int i = 0; i < n; ++i) {
		cin >> x;
		add(x, 1);
	}
	for(int i = 0; i < q; ++i) {
		cin >> x;
		if(x > 0) {
			add(x, 1);
		} else {
			x = -x;
			int i = 0;
			for(int add = (n+1)/2; add > 0; add >>= 1) {
				while(i+add <= n && querry(i+add) < x) i += add;
			}
			++i;
			add(i, -1);
		}
	}
	if(querry(n) <= 0) cout << 0;
	else {
		int i = 0;
		for(int add = (n+1)/2; add > 0; add >>= 1) {
			while(i+add <= n && querry(i+add) < 1) i += add;
		}
		++i;
		cout << i;
	}
	cout << "\n";

	return 0;
}
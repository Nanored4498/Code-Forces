#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

struct Node {
	char type;
	int x, y;
	ll size;
};
vector<Node> a(1e5+5, Node{3, 0, 0, -1});

ll compSize(int x) {
	Node &n = a[x];
	if(n.size >= 0) return n.size;
	if(n.type == 0) return n.size = (compSize(n.x)+1)/2;
	else if(n.type == 1) return n.size = compSize(n.x)/2;
	return n.size = compSize(n.x) + compSize(n.y);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, q;
	cin >> m >> n >> q;
	while(n--) {
		char c; int x, y, z;
		cin >> c >> x >> y >> z;
		if(c == 'S') {
			a[y] = {0, x, 0, -1};
			a[z] = {1, x, 0, -1};
		} else a[z] = {2, x, y, -1};
	}
	a[1].size = m;
	for(int x = 0; x < (int)a.size(); ++x) if(a[x].type < 3) compSize(x);
	while(q--) {
		int x;
		ll k;
		cin >> x >> k;
		--k;
		while(x != 1) {
			if(k > (ll)1e14) break;
			if(a[x].type == 0) {
				x =	a[x].x;
				k = 2*k;
			} else if(a[x].type == 1) {
				x =	a[x].x;
				k = 2*k+1;
			} else {
				bool mi = a[a[x].x].size < a[a[x].y].size;
				ll ms = mi ? a[a[x].x].size : a[a[x].y].size;
				if(k >= 2*ms) {
					x = mi ? a[x].y : a[x].x;
					k -= ms;
				} else {
					x =	k&1 ? a[x].y : a[x].x;
					k = k/2;
				}
			}
		}
		if(k >= m) cout << "none\n";
		else cout << k+1 << '\n';
	}

	return 0;
}
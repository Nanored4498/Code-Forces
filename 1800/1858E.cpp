#include <iostream>
#include <vector>

using namespace std;
using ll = long long;
using vi = vector<int>;

constexpr int MAX = 1e6+5;

int first[MAX], ans[MAX], a[MAX];
vector<pair<int*, int>> stack;

void save(int &val) {
	stack.emplace_back(&val, val);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int q; cin >> q;
	int l = 0;
	while(q--) {
		char c;
		cin >> c;
		switch(c) {
		case '+': {
			int x; cin >> x;
			stack.emplace_back(nullptr, 0);
			save(l);
			++ l;
			if(first[a[l]] >= l) {
				save(first[a[l]]);
				first[a[l]] = 0;
			}
			save(a[l]);
			a[l] = x;
			if(!first[x] || first[x] >= l) {
				save(first[x]);
				first[x] = l;
				save(ans[l]);
				ans[l] = ans[l-1]+1;
			} else {
				save(ans[l]);
				ans[l] = ans[l-1];
			}
		} break;
		case '-': {
			int k; cin >> k;
			stack.emplace_back(nullptr, 0);
			save(l);
			l -= k;
		} break;
		case '!': {
			while(stack.back().first) {
				*stack.back().first = stack.back().second;
				stack.pop_back();
			}
			stack.pop_back();
		} break;
		default /*'?*/: {
			cout << ans[l] << endl;
		}
		}
	}

	return 0;
}
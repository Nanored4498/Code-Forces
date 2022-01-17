#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ll x, y;
	string a, b, c;
	cin >> x >> y;
	bool ans = x == y;
	while(x > 0) {
		a.push_back((x&1)+'0');
		x >>= 1;
	}
	while(y > 0) {
		b.push_back((y&1)+'0');
		y >>= 1;
	}
	reverse(a.begin(), a.end());
	c = a + "1";
	while(a.back() == '0') a.pop_back();
	for(auto x : {a, c}) for(int i : {0, 1}) {
		if(x.size() > b.size()) continue;
		for(int j = 0; j <= b.size()-x.size(); ++j)
			ans |= string(j, '1')+x+string(b.size()-x.size()-j, '1') == b;
		reverse(x.begin(), x.end());
	}
	cout << (ans ? "YES" : "NO") << endl;

	return 0;
}
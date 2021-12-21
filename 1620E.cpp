#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct R { bool t; int x, y; };

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int q;
	cin >> q;
	vector<R> rs(q);
	for(R &r : rs) {
		char c;
		cin >> c >> r.x;
		if(r.t = (c == '2')) cin >> r.y; 
	}
	vector<int> ans;
	unordered_map<int, int> m;
	const auto get = [&](int x) {
		auto it = m.find(x);
		return it == m.end() ? x : it->second;
	};
	while(q--)
		if(rs[q].t) m[rs[q].x] = get(rs[q].y);
		else ans.push_back(get(rs[q].x));
	for(auto it = ans.rbegin(); it != ans.rend(); ++it)
		cout << *it << ' ';
	cout << endl;

	return 0;
}
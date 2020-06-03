#include <iostream> 
#include <vector>
#include <map>
#include <tuple>

using namespace std;
typedef pair<int, int> pii;
typedef tuple<int, int, int> t3i;
typedef vector<int> vi;
#define MAX_N 200005

int p[MAX_N], s[MAX_N];

int find(int i) {
	if(p[i] == i) return i;
	p[i] = find(p[i]);
	return p[i];
}

bool merge(int i, int j) {
	int ri = find(i), rj = find(j);
	if(ri == rj) return false;
	if(s[ri] < s[rj]) swap(ri, rj);
	p[rj] = ri;
	s[ri] += s[rj];
	return true;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	map<int, int> a;
	vector<t3i> ans;
	for(int i = 1; i <= n; ++i) {
		int m, d;
		p[i] = i;
		s[i] = 1;
		cin >> m;
		while(m --> 0) {
			cin >> d;
			if(a.count(d) && merge(i, a[d])) ans.emplace_back(a[d], i, d);
			a[d] = i;
		}
	}
	if(ans.size() == n-1) {
		for(const t3i &t : ans)
			cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << "\n";
	} else cout << "impossible\n";

	return 0;
}
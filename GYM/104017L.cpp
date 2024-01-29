#include <iostream>
#include <vector>

using namespace std;
using pii = pair<int, int>;

const int R = 21;
const int T = 360;

bool c[R][T];
bool s[R][T];
bool seen[R][T];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		for(int r = 0; r < R; ++r) for(int t = 0; t < T; ++t) {
			seen[r][t] = c[r][t] = s[r][t] = false;
		}
		while(n--) {
			char type; cin >> type;
			if(type == 'C') {
				int r, t1, t2;
				cin >> r >> t1 >> t2;
				for(int t = t1; t != t2; t = (t+1)%T) c[r][t] = true;
			} else /* c == 'S' */ {
				int r1, r2, t;
				cin >> r1 >> r2 >> t;
				for(int r = r1; r < r2; ++r) s[r][t] = true;
			}
		}
		vector<pii> Q;
		const auto try_add = [&](int r, int t) {
			if(seen[r][t]) return;
			seen[r][t] = true;
			Q.emplace_back(r, t);
		};
		for(int t = 0; t < T; ++t) if(!c[R-1][t]) {
			try_add(R-1, t);
		}
		string ans = "NO";
		while(!Q.empty()) {
			auto [r, t] = Q.back();
			Q.pop_back();
			if(r == 0) {
				ans = "YES";
				break;
			}
			if(r < R-1 && !c[r+1][t]) try_add(r+1, t);
			if(!c[r][t]) try_add(r-1, t);
			const int nt = (t+1) % T;
			const int pt = (t+T-1) % T;
			if(!s[r][t]) try_add(r, pt);
			if(!s[r][nt]) try_add(r, nt);
		}
		cout << ans << '\n';
	}

	return 0;
}
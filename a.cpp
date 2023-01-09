#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int w, h, k;
	cin >> w >> h >> k;
	vector<string> m(h, string(w, '?'));
	vector<vector<int>> m2(h, vector<int>(w, 0));
	vector<pair<int, int>> ws;
	vector<pair<int, int>> Q;
	while(k--) {
		int wx, wy, n;
		cin >> wx >> wy >> n;
		swap(wx, wy);
		ws.emplace_back(wx, wy);
		while(n--) {
			int x, y;
			cin >> x >> y;
			swap(x, y);
			--x; --y;
			m2[x][y] |= 1 << (ws.size()-1);
			if(m[x][y] != '#') {
				m[x][y] = '#';
				Q.emplace_back(x, y);
			}
			int i = x-wx, j = y-wy;
			if(i >= 0 && i < h && j >= 0 && j < w && m[i][j] == '?') {
				m[i][j] = '.';
				Q.emplace_back(i, j);
			}
		}
		for(int i = 0; i < h; ++i) for(int j = 0; j < w; ++j)
			if((i < wx || i >= h+wx || j < wy || j >= w+wy) && m[i][j] == '?') {
				m[i][j] = '.';
				Q.emplace_back(i, j);
			}
	}
	while(!Q.empty()) {
		auto [x, y] = Q.back();
		Q.pop_back();
		if(m[x][y] == '#') {
			for(int k = 0; k < (int) ws.size(); ++k) if(!((m2[x][y]>>k)&1)) {
				int i = x-ws[k].first, j = y-ws[k].second;
				if(i >= 0 && i < h && j >= 0 && j < w && m[i][j] == '?') {
					m[i][j] = '#';
					Q.emplace_back(i, j);
				}
			}
		} else if(m[x][y] == '.') {
			for(auto [wx, wy] : ws) {
				int i = x+wx, j = y+wy;
				if(i >= 0 && i < h && j >= 0 && j < w && m[i][j] == '?') {
					m[i][j] = '.';
					Q.emplace_back(i, j);
				}
			}
		}
	}

	for(string &s : m) {
		for(char c : s) cout << (c == '?' ? '.' : c);
		cout << '\n';
	}
	cout << '\n';
	for(string &s : m) {
		for(char c : s) cout << (c == '?' ? '#' : c);
		cout << '\n';
	}

	return 0;
}
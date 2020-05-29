#include <iostream>
#include <vector>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int W, H;
	cin >> W >> H;
	vector<string> m(H);
	vector<vi> clus(H, vi(W, -1));
	for(int i = 0; i < H; ++i) cin >> m[i];
	int c = 0;
	int A=0, B=0, C=0;
	for(int i = 0; i < H; ++i) {
		for(int j = 0; j < W; ++j) {
			if(m[i][j] == '.' || clus[i][j] >= 0) continue;
			vector<pii> st{{i, j}};
			clus[i][j] = c;
			int i2 = i, j2 = j;
			while(!st.empty()) {
				pii p = st.back(); st.pop_back();
				i2 = max(i2, p.first);
				j2 = max(j2, p.second);
				for(int a = p.first-1; a <= p.first+1; ++a)
					for(int b = p.second-1; b <= p.second+1; ++b)
						if(a >= 0 && a < H && b >= 0 && b < W && m[a][b] == '#' && clus[a][b] < 0) {
							clus[a][b] = c;
							st.emplace_back(a, b);
						}

			}
			if(c > 0 && i2 > i) {
				int mi = (i+i2)/2, mj = (j+j2)/2;
				if(m[mi][j2] == '.' || m[mi][j2-1] == '.') ++C;
				else if(m[i2][mj] == '.' || m[i2][mj-1] == '.') ++A;
				else ++B;
			}
			++ c;
		}
	}
	cout << A << " " << B << " " << C << "\n";

	return 0;
}
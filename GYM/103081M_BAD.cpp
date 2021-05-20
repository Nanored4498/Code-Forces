#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;
const int NQ = 90;

int H, W, nC[2];
vector<string> im[2];
vector<vi> C[2];

void flip(int i, int j) {
	cout << j << ' ' << i << '\n';
	if(im[0][i][j] == '.') im[0][i][j] = '#';
	else im[0][i][j] = '.';
	for(const string &s : im[0]) cerr << s << endl;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> W >> H;
	for(int t = 0; t < 2; ++t) {
		im[t].resize(H);
		C[t].assign(H, vi(W, -1));
		for(int i = 0; i < H; ++i) cin >> im[t][i];
		vi Q[NQ];
		Q[0].push_back(0);
		C[t][0][0] = 0;
		nC[t] = 0;
		for(int c = 0; !Q[c].empty(); ++c) {
			++ nC[t];
			for(int q = 0; q < Q[c].size(); ++q) {
				int x = Q[c][q];
				int i = x/W, j = x%W;
				vi neighb;
				if(i > 0) neighb.push_back(x-W);
				if(i+1 < H) neighb.push_back(x+W);
				if(j > 0) neighb.push_back(x-1);
				if(j+1 < W) neighb.push_back(x+1);
				for(int y : neighb) {
					int i2 = y/W, j2 = y%W;
					if(C[t][i2][j2] != -1) continue;
					int c2 = c + (im[t][i][j] != im[t][i2][j2]);
					Q[c2].push_back(y);
					C[t][i2][j2] = c2;
				}
			}
		}
	}

	if(nC[0] != nC[1] || im[0][0][0] != im[1][0][0]) {
		cout << "IMPOSSIBLE\n";
		return 0;
	}
	int nc = nC[0];

	for(int c = 1; c < nc; ++c) {
		vi Q;
		for(int i = c; i < H-c; ++i)
			for(int j = c; j < W-c; ++j)
				if(C[0][i][j] == c)
					Q.push_back(i*W+j);
		while(!Q.empty()) {
			int x = Q.back();
			Q.pop_back();
			int i = x/W, j = x%W;
			vi neighb;
			if(i > c) neighb.push_back(x-W);
			if(i+1 < H-c) neighb.push_back(x+W);
			if(j > c) neighb.push_back(x-1);
			if(j+1 < W-c) neighb.push_back(x+1);
			for(int y : neighb) {
				int i2 = y/W, j2 = y%W;
				if(C[0][i2][j2] >= c) continue;
				C[0][i2][j2] = c;
				flip(i2, j2);
				Q.push_back(y);
			}
		}
	}

	for(int c = nc-2; c >= 0; --c) {
		vi Q[NQ];
		for(int i = c+1; i < H-c-1; ++i)
			for(int j = c+1; j < W-c-1; ++j)
				if(C[1][i][j] <= c)
					Q[min(min(i, H-1-i), min(j, W-1-j))].push_back(i*W+j);
		for(int d = 0; d < NQ; ++d)
			for(int x : Q[d])
				flip(x/W, x%W);
	}

	return 0;
}
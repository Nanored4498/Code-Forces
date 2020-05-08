#include <iostream>
#include <queue>
#include <vector>
#include <cmath>

using namespace std;

#define MAX_N 1002
#define MAX_B 101
typedef pair<int, int> pii;

int xd, yd, xs[MAX_N], ys[MAX_N];
int B, T, N;
int C[101];
int A[MAX_N][MAX_N];
vector<pii> G[MAX_N];
int D[MAX_N*MAX_B];

int dist(int i, int j) {
	double dx = xs[i] - xs[j], dy = ys[i] - ys[j];
	return sqrt(dx*dx + dy*dy) + .99999999;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int x, y, l, j, mj;
	cin >> xs[0] >> ys[0] >> xs[1] >> ys[1];
	cin >> B >> C[0] >> T;
	for(int i = 1; i <= T; ++i) cin >> C[i];
	cin >> N;
	for(int i = 0; i <= N+1; ++i)
		for(int j = 0; j <= N+1; ++j)
			A[i][j] = -1;
	A[0][1] = C[0];
	for(int i = 2; i <= N+1; ++i) {
		cin >> xs[i] >> ys[i] >> l;
		A[0][i] = C[0];
		A[i][1] = C[0];
		while(l --> 0) {
			cin >> j >> mj;
			j += 2;
			if(A[i][j] < 0 || C[mj] < A[i][j]) A[i][j] = A[j][i] = C[mj];
		}
	}
	for(int i = 0; i <= N+1; ++i)
		for(int j = 0; j <= N+1; ++j)
			if(A[i][j] >= 0) G[i].emplace_back(j, A[i][j]);
	for(int i = 0; i < (N+2)*(B+1); ++i) D[i] = 999999;

	int res = -1;
	priority_queue<pii> Q;
	Q.emplace(0, 0);
	D[0] = 0;
	while(!Q.empty()) {
		pii t = Q.top(); Q.pop();
		int c = - t.first;
		int id = t.second;
		if(D[id] < c) continue;
		int i = id / (B+1), d = id % (B+1);
		if(i == 1) { res = c; break; }
		for(const pii & p : G[i]) {
			int j = p.first, cj = p.second;
			int dj = dist(i, j);
			int d2 = d + dj;
			if(d2 > B) continue;
			int id2 = (B+1)*j + d2;
			int c2 = c + cj*dj;
			if(D[id2] <= c2) continue;
			D[id2] = c2;
			Q.emplace(-c2, id2);
		}
	}
	cout << res << "\n";

	return 0;
}
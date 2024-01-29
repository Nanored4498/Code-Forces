#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;
using ll = long long;
using vi = vector<int>;

struct Point {
	int x, y;
};

int dist(const Point &a, const Point &b) {
	int dx = a.x-b.x;
	int dy = a.y-b.y;
	return ceil(sqrt(dx*dx + dy*dy));
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	Point s, d;
	cin >> s.x >> s.y;
	cin >> d.x >> d.y;	
	int B, C0, T, N;
	cin >> B >> C0 >> T;
	vector<int> C(T);
	for(int &c : C) cin >> c;
	cin >> N;
	vector<Point> ps(N);
	vector<vector<pair<int, int>>> G(N);
	for(int i = 0; i < N; ++i) {
		cin >> ps[i].x >> ps[i].y;
		int l;
		cin >> l;
		while(l--) {
			int j, m;
			cin >> j >> m;
			m = C[m-1];
			G[i].emplace_back(j, m);
			G[j].emplace_back(i, m);
		}
	}

	const int INF = 1e9;
	const int M = B*N;
	vector<int> cost(M+1, INF);
	using QEl = pair<int, int>;
	priority_queue<QEl, vector<QEl>, greater<QEl>> Q;
	for(int i = 0; i < N; ++i) {
		int d = dist(s, ps[i]);
		if(d >= B) continue;
		int id = B*i+d;
		Q.emplace(
			cost[id] = C0 * d,
			id
		);
	}
	int sd = dist(s, d);
	if(sd <= B) Q.emplace(cost[M] = sd * C0, M);

	while(!Q.empty()) {
		auto [c, id] = Q.top();
		Q.pop();
		if(id == M) {
			cout << c << '\n';
			return 0;
		}
		if(c != cost[id]) continue;
		int i = id/B;
		int b = id%B;
		int dd = dist(ps[i], d);
		int b2 = b + dd;
		if(b2 <= B) {
			int c2 = c + C0 * dd;
			if(c2 < cost[M]) Q.emplace(cost[M] = c2, M);
		}
		for(auto [j, m] : G[i]) {
			int dj = dist(ps[i], ps[j]);
			int b2 = b + dj;
			if(b2 >= B) continue;
			int c2 = c + m * dj;
			int jd = B*j+b2;
			if(c2 < cost[jd]) Q.emplace(cost[jd] = c2, jd);
		}
	}
	cout << "-1\n";

	return 0;
}
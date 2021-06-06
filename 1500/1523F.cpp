#include <iostream>
#include <algorithm>

using namespace std;
const int MN = 14;
const int MM = 100;
const int MT = 1e9+5;
#define LSB(i) ((i)&(-(i)))

struct Quest {
	int x, y, t;
};

int min_time[1<<MN][MM+1];
int max_score[1<<MN][MM];
int xa[MN], ya[MN];
Quest q[MM];
bool q2q[MM][MM];
int q2t[MM][MN];
int t2t[1<<MN][MN];
int t2q[1<<MN][MM];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	// Input
	int n, m;
	cin >> n >> m;
	for(int i = 0; i < n; ++i) cin >> xa[i] >> ya[i];
	for(int i = 0; i < m; ++i) cin >> q[i].x >> q[i].y >> q[i].t;
	sort(q, q+m, [](const Quest &a, const Quest &b) {
		return a.t < b.t;
	});
	int st = 1<<n;
	
	// Init
	for(int t = 0; t < st; ++t)
		for(int s = 0; s <= m; ++s)
			min_time[t][s] = MT;
	for(int i = 0; i < n; ++i) min_time[1<<i][0] = 0;
	for(int i = 0; i < m; ++i) max_score[0][i] = 1;

	// q2q
	for(int i = 0; i < m; ++i)
		for(int j = i+1; j < m; ++j)
			q2q[i][j] = q[i].t + abs(q[j].x - q[i].x) + abs(q[j].y - q[i].y) <= q[j].t;
	// q2t
	for(int i = 0; i < m; ++i)
		for(int j = 0; j < n; ++j)
			q2t[i][j] = abs(xa[j] - q[i].x) + abs(ya[j] - q[i].y);
	// t2t
	for(int i = 0; i < n; ++i) t2t[0][i] = MT;
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			t2t[1<<i][j] = abs(xa[j] - xa[i]) + abs(ya[j] - ya[i]);
	for(int t = 1; t < st; ++t)
		for(int j = 0; j < n; ++j)
			t2t[t][j] = min(t2t[t-LSB(t)][j], t2t[LSB(t)][j]);
	// t2q
	for(int i = 0; i < m; ++i) t2q[0][i] = MT;
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < m; ++j)
			t2q[1<<i][j] = abs(q[j].x- xa[i]) + abs(q[j].y - ya[i]);
	for(int t = 1; t < st; ++t)
		for(int j = 0; j < m; ++j)
			t2q[t][j] = min(t2q[t-LSB(t)][j], t2q[LSB(t)][j]);

	const auto upd_time = [&](int t, int s, int v) {
		if(v >= min_time[t][s]) return;
		min_time[t][s] = v;
	};
	const auto upd_score = [&](int t, int i, int v) {
		if(v <= max_score[t][i]) return;
		max_score[t][i] = v;
	};

	int ans = 0;
	for(int t = 0; t < st; ++t) {
		// tower --> ...
		if(t > 0) for(int s = 0; s < m; ++ s) {
			int time = min_time[t][s];
			if(time >= MT) continue;
			// --> tower
			for(int j = 0; j < n; ++j)
				upd_time(t | (1<<j), s, time + t2t[t][j]);
			// --> quest
			for(int j = s; j < m; ++j) if(time + t2q[t][j] <= q[j].t)
				upd_score(t, j, s+1);
		}

		// quest --> ...
		for(int i = 0; i < m; ++i) {	
			int s = max_score[t][i];
			if(s < 1) continue;
			ans = max(ans, s);
			// --> tower
			for(int j = 0; j < n; ++j)
				upd_time(t | (1<<j), s, q[i].t + min(q2t[i][j], t2t[t][j]));
			// --> quest
			for(int j = i+1; j < m; ++j) if(q2q[i][j] || q[i].t + t2q[t][j] <= q[j].t)
				upd_score(t, j, s+1);
		}
	}

	cout << ans << endl;
	return 0;
}
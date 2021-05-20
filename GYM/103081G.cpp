#include <iostream>
#include <vector>
#include <deque>
#include <functional>

using namespace std;
typedef long long ll;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, K;
	cin >> N >> K;

	if(K == 1) {
		cout << 0 << '\n';
		return 0;
	}

	vi nxt(N, 0);
	vector<vi> prv(N);
	for(int i = 1; i < N; ++i) {
		nxt[i] += i;
		for(int j = i; j < N; j += i) ++ nxt[j];
		nxt[i] %= N;
		prv[nxt[i]].push_back(i);
	}

	int ans = -1;
	ll SS = 1e12;
	vector<bool> seen(N, false);
	for(int i = 0; i < N; ++i) if(!seen[i]) {
		deque<int> path;
		int j = i;
		while(!seen[j]) {
			path.push_back(j);
			seen[j] = true;
			j = nxt[j];
		}
		while(path.front() != j) {
			seen[path.front()] = false;
			path.pop_front();
		}
		path.resize(min((size_t) K, path.size()));
		ll S = 0;
		for(int s : path) S += s;

		function<void()> f;
		f = [&]() {
			if(path.size() >= K) {
				if(S < SS) {
					SS = S;
					ans = path.front();
				}
				S -= path[K-1];
			}
			for(int p : prv[path.front()]) if(!seen[p]) {
				seen[p] = true;
				path.push_front(p);
				S += p;
				f();
				S -= p;
				path.pop_front();
			}
			if(path.size() >= K) S += path[K-1];
		};

		do {
			f();
			// next in loop
			S -= path.front();
			path.push_back(nxt[path.back()]);
			path.pop_front();
			S += path.back();
		} while(path.front() != j);
	}

	cout << ans;
	if(ans != -1) for(int i = 1; i < K; ++i)
		cout << ' ' << (ans = nxt[ans]);
	cout << '\n';

	return 0;
}
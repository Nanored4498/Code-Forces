#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n, k;
	string s;
	cin >> t;
	while(t --> 0) {
		cin >> n >> k;
		vector<vi> ss(k, vi(3, 0));
		int ones = 0;
		cin >> s;
		for(int i = 0; i < n; ++i) {
			int ik  = i % k;
			if(s[i] == '1') {
				++ ones;
				ss[ik][2] = min(ss[ik][1], ss[ik][2])+1;
				ss[ik][1] = min(ss[ik][0], ss[ik][1]);
				ss[ik][0] += 1;
			} else {
				ss[ik][2] = min(ss[ik][1], ss[ik][2]);
				ss[ik][1] = min(ss[ik][0], ss[ik][1])+1;
			}
		}
		int res = n;
		for(int i = 0; i < k; ++i)
			res = min(res, ones - ss[i][0] + min(ss[i][0], min(ss[i][1], ss[i][2])));
		cout << res << "\n";
	}

	return 0;
}
#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<int> vi;
const ll MOD = 1e9 + 7;

vi idx, Size;

int find(int i) {
	if(idx[i] == i) return i;
	else return (idx[i] = find(idx[i]));
}

void merge(int i, int j) {
	i = find(i);
	j = find(j);
	if(i == j) return;
	if(Size[i] < Size[j]) swap(i, j);
	Size[i] += Size[j];
	idx[j] = idx[i];
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	int n, m;
	cin >> n >> m;

	idx.resize(m+1);
	for(int i = 0; i < m+1; ++i) idx[i] = i;
	Size.assign(m+1, 1);

	ll T = 1;
	vi S;
	for(int i = 1; i <= n; ++i) {
		int k, x, y;
		cin >> k >> x;
		if(k == 1) {
			if(find(x) == find(0)) continue;
			merge(x, 0);
		} else {
			cin >> y;
			if(find(x) == find(y)) continue;
			merge(x, y);
		}
		T = (T*2LL) % MOD;
		S.push_back(i);
	}
	cout << T << " " << S.size() << "\n";
	for(int i : S) cout << i << " ";
	cout << "\n";

	return 0;
}
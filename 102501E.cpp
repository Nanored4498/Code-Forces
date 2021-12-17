#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

vector<vi> m;
vi s;

void sub(int i, int )

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int K, L;
	cin >> K >> L;
	int n = K*L;
	m.resize(n);
	for(int i = 0; i < n; ++i) m[i].push_back(i);
	s.assign(n, 1);
	for(int i = 0; i < K; ++i) for(int j = 0; j < L; ++j) {

	}
}
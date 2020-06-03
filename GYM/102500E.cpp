#include <iostream> 
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	double te;
	int ts[4];
	for(int i = 0; i < 4; ++i) {
		cin >> te;
		ts[i] = (100*te+.5);
	}
	cin >> te;
	int t = (300*te+.5);
	sort(ts, ts+4);
	int s0 = ts[0]+ts[1]+ts[2];
	int s1 = ts[1]+ts[2]+ts[3];
	if(s0 > t) return cout << "impossible\n", 0;
	if(s1 <= t) return cout << "infinite\n", 0;
	int ans0 = t-ts[1]-ts[2];
	double ans = ans0*.01;
	cout << fixed << setprecision(2) << ans << "\n";

	return 0;
}
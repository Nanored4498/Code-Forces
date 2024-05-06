#include <iostream>

using namespace std;

int main() {
	int a, b, c, d, e;
	cout << "1 0 0" << endl;
	cin >> a;
	cout << "0 1 0" << endl;
	cin >> b;
	cout << "0 0 1" << endl;
	cin >> c;
	cout << "1 1 1" << endl;
	cin >> d;
	cout << "1 2 3" << endl;
	cin >> e;
	if(a+b+c == d) {
		cout << a << ' ' << b << ' ' << c << endl;
	} else if(a+2*b+3*c == e) {
		cout << a << ' ' << b << ' ' << c << endl;
	} else if(d-b-c == e-2*b-3*c) {
		cout << d-b-c << ' ' << b << ' ' << c << endl;
	} else if(2*(d-a-c) == e-a-3*c) {
		cout << a << ' ' << d-a-c << ' ' << c << endl;
	} else if(3*(d-b-a) == e-2*b-a) {
		cout << a << ' ' << b << ' ' << d-a-b << endl;
	}
	return 0;
}
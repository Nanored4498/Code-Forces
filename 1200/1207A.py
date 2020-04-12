t = int(input())
for _ in range(t):
	b, p, f = map(int, input().split())
	h, c = map(int, input().split())
	res = 0
	l = sorted([(-h, p), (-c, f)])
	for price, n in l:
		m = min(b // 2, n)
		b -= 2*m
		res -= m*price
	print(res)
	
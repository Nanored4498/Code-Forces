t = int(input())
for _ in range(t):
	n, x, m = map(int, input().split())
	a, b = x, x
	for _ in range(m):
		l, r = map(int, input().split())
		if r >= a and b >= l:
			a = min(l, a)
			b = max(r, b)
	print(b-a+1)
t = int(input())
for _ in range(t):
	n, m, k = map(int, input().split())
	x = min(n//k, m)
	r = m-x
	y = 0
	while r > 0:
		r -= k-1
		y += 1
	print(x-y)
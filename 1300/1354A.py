t = int(input())
for _ in range(t):
	a, b, c, d = map(int, input().split())
	if b >= a:
		print(b)
	elif d >= c:
		print(-1)
	else:
		res = ((a-b+c-d-1) // (c-d)) * c + b
		print(res)
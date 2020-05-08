t = int(input())
for _ in range(t):
	a, b, x, y = map(int, input().split())
	r = max([a*y, a*(b-y-1), b*x, b*(a-x-1)])
	print(r)